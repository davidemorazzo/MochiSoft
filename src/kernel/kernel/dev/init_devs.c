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
}