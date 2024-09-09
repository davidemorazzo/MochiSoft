/* August 2024 - Davide Morazzo 

Funzioni per l'utilizzo e l'inizializzazione del dispositivo PIC 
(Programmable Interrupt Controller) 8259 presente nell'architettura x86.
Source: https://wiki.osdev.org/8259_PIC */

#include "kernel/microcode.h"


#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */

void initialize_pic(void)
{
    /* ICW1 - begin initialization */
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
     * In x86 protected mode, we have to remap the PICs beyond 0x20 because
     * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
     */
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    /* ICW3 - setup cascading */
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);

    /* ICW4 - environment info */
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

/* Comando da inviare al PIC alla fine della interrupt routine */
void PIC_sendEOI(uint8_t irq){

	if(irq > 8){
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}


