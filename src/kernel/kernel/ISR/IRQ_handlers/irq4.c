/* Novembre 2024 - Davide Morazzo

Handler per Interrupt IRQ4 (UART0)
Indirizzo 0x24 (36)
*/
#include "kernel/PIC-8259.h"
#include "kernel/uart.h"
#include "kernel/tty.h"

void irq4(){
	serial_ISR(UART0);
    PIC_sendEOI(4);
}