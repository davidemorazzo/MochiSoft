/* Funzioni per l'utilizzo e l'inizializzazione del dispositivo PIC 
(Programmable Interrupt Controller) 8259 presente nell'architettura x86.

Source: https://wiki.osdev.org/8259_PIC */

#include "i386_io.cpp"


#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */



/* Comando da inviare al PIC alla fine della interrupt routine */
void PIC_sendEOI(uint8_t irq){

	if(irq > 8){
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}


