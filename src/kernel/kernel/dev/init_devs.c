/* Novembre 2024 - Davide Morazzo

Funzione per inizializzare tutti i devices della CPU con una sola chiamata.
Dispositivi configurati da questa funzione:
	1. UART
	2. Programmable Interrupt Timer
	3. Real Time Clock
	4. Programmable Interrupt Controller
	
/!\ Richiede GDT e IDT già configurati 

Links: https://web.archive.org/web/20150514082645/http://www.nondot.org/sabre/os/files/MiscHW/RealtimeClockFAQ.txt */

#include "kernel/idt.h"
#include "kernel/microcode.h"

#include "dev/8253/PIT.h"
#include "dev/RTC.h"
#include "dev/PCI/PCI.h"
#include "dev/PCI/AHCI.h"

#include "kernel/kglobals.h"
#include "kernel/kstdio.h"
#include "kernel/kheap.h"
#include "kernel/PIC-8259.h"		// Spostare definizione in cartella dev/
#include "kernel/uart.h"			// Spostare definizione in cartella dev/

/* Variabili di configurazione per l'inizializzazione */
#define RTC_ENABLE_UIE 1
#define RTC_ENABLE_AIE 0 
#define RTC_ENABLE_PIE 0 
#define RTC_PIE_RATE 0b0110  //1024 Hz		
/* ========================================== */
#define UART_FOR_TTY		UART0
/* ========================================== */
#define PIT_INIT_MODE 		PIT_MODE_RATE_GEN
#define PIT_INIT_FREQ_CH0 	100 //Hz 

extern void irq_0x20_wrapper();
extern void irq_0x24_wrapper();
extern void irq_0x28_wrapper();
extern void irq_0x2B_wrapper();

void init_devs(){
	InterruptDescriptor32 d;
	char it_state = read_eflags()>>9 & 0x1;
    disable_it();

	/*==  Programmable Interrupt Controller  ==*/
	PIC_remap(33, 33+8);
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
	IRQ_clear_mask(2);			// Line from PIC2

	/*=========== Real Time Clock =============*/
	// Enable IRQ8:
	// [X] bit 4 reg B - Updated Ended Interrupt
	// [ ] bit 5 reg B - Alarm Interrupt
	// [ ] bit 6 reg B - Periodic Interrupt

	uint8_t reg = rtc_get_reg(RTC_REG_B);
	outb(RTC_ADDRESS, 0x8B);
	outb(RTC_DATA, reg | 
		(RTC_ENABLE_PIE * 0x40) | 
		(RTC_ENABLE_AIE * 0x20) | 
		(RTC_ENABLE_UIE * 0x10) );
	// Set IRQ8 freq to 2 Hz 
	reg = rtc_get_reg(RTC_REG_A);
	outb(RTC_ADDRESS, 0x8A);
	outb(RTC_DATA, (reg&0xF0) | (RTC_PIE_RATE&0xF));
	// Clear register C to ack the interrupt to the RTC
	outb(RTC_ADDRESS, 0x0C);
	inb(RTC_DATA);
	SET_IT_VEC(d, irq_0x28_wrapper, 0x28);
	IRQ_clear_mask(8);

	/*================= UART ==================*/
	uart_setup(UART_FOR_TTY);
	IRQ_clear_mask(4);
	SET_IT_VEC(d, irq_0x24_wrapper, 0x24);

	/*====== Programmable Interrupt Timer =====*/
	pit_init(PIT_INIT_MODE, PIT_INIT_FREQ_CH0);
	IRQ_clear_mask(0);
	SET_IT_VEC(d, irq_0x20_wrapper, 32);

	if(it_state){
		enable_it();
	}

	/* ============= PCI / AHCI ===============*/
	// PCIcheckAllBuses(); // Stampa su KINFO i device trovati
	HBA_MEM *abar = (HBA_MEM *) PCIgetHDDBAR5();
	if (abar == NULL){
		KLOGERROR("No AHCI device found on PCI");
	}
	
	uint8_t pi = abar->pi;
	AHCI_HDD.abar = abar;
	AHCI_HDD.port = NULL;
	for (int i=0; i<32; i++){
		if (pi & 1){
			uint32_t ssts = abar->ports[i].ssts; 
			uint8_t ipm = (ssts >> 8) & 0xF;
			uint8_t det = ssts & 0xF;
			if (ipm == 1 && det == 3){
				
				AHCI_HDD.portIndex = i;
				AHCI_HDD.port = &abar->ports[i];
				AHCI_HDD.port->clb = (uint32_t) &AHCI_HDD.cmd_list;
				AHCI_HDD.port->fb = (uint32_t) &AHCI_HDD.rcv_fis;
				for (int cmd=0; cmd <32; cmd++){
					AHCI_HDD.cmd_list.cmdHeader[cmd].ctba = (uint32_t) &AHCI_HDD.cmd_table[cmd];
				}
				IRQ_clear_mask(11);
				SET_IT_VEC(d, irq_0x2B_wrapper, 0x2B);
				AHCI_HDD.port->ie |= 0xFF;
				AHCI_HDD.abar->ghc |= 0x2; // Interrupt enable
				stop_cmd(AHCI_HDD.port);
				start_cmd(AHCI_HDD.port);

				KLOGINFO("Active HDD found on AHCI port %d", i);
				break;
			}
		}
		pi >>= 1;
	}
	if (AHCI_HDD.port == NULL) {
		KLOGERROR("No active ports found on AHCI conf. space")
	}
	return;
}