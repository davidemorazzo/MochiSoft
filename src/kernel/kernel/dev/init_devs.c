/* Novembre 2024 - Davide Morazzo

Funzione per inizializzare tutti i devices della CPU con una sola chiamata.
Dispositivi configurati da questa funzione:
	1. UART
	2. Programmable Interrupt Timer
	3. Real Time Clock
	4. Programmable Interrupt Controller
	
/!\ Richiede GDT e IDT già configurati */

#include "kernel/idt.h"
#include "kernel/microcode.h"

#include "dev/8253/PIT.h"
#include "dev/RTC.h"
#include "kernel/PIC-8259.h"		// Spostare definizione in cartella dev/
#include "kernel/uart.h"			// Spostare definizione in cartella dev/

/* Variabili di configurazione per l'inizializzazione */

/* ========================================== */
#define UART_FOR_TTY		UART0
/* ========================================== */
#define PIT_INIT_MODE 		PIT_MODE_RATE_GEN
#define PIT_INIT_FREQ_CH0 	100 //Hz 

extern void irq_0x20_wrapper();
extern void irq_0x24_wrapper();

void init_devs(){
	InterruptDescriptor32 d;
	char it_state = read_eflags()>>9 & 0x1;
    disable_it();

	/*=========== Real Time Clock =============*/

	/*==  Programmable Interrupt Controller  ==*/

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