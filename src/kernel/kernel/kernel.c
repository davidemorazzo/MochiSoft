#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "string.h"

#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/microcode.h"
#include "kernel/PIC-8259.h"
#include "kernel/tty.h"
#include "kernel/kstdio.h"
#include "kernel/exceptions.h"


unsigned short stdout_port = UART0;
uint64_t global_IDT[255] = {0};
uint64_t global_GDT[50] = {0};

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/


void uart_isr(){
    __asm__("pushal");
    serial_ISR(stdout_port);
    PIC_sendEOI(4);
    __asm__("popal; leave; iret");
}

void generic_isr(){
    __asm__("pushal");
    // uart_term_glbl->writestring("Generic Interrupt!\n");
    for (int j=0; j<7; j++){
        if (j!=4) PIC_sendEOI(j);
    }
    __asm__("popal; leave; iret"); /* BLACK MAGIC! */
}

void kernel_main (void){
    
    disable_it();    
    /* ======= GLOBAL DESCRIPTOR TABLE ====== */
    gdt_init(global_GDT);
    xDTR GDTR;
    GDTR.base=global_GDT;
    GDTR.length=3*8-1;
    gdt_load(GDTR);
    /* ===== INTERRUPT DESCRIPTOR TABLE ====== */
    xDTR IDTR;
    IDTR.base = global_IDT;
    IDTR.length = 256*8-1;
    load_idt(IDTR); /*Load IDTR*/

    InterruptDescriptor32 genericIsrDesc = InterruptDescriptor32Init;
    genericIsrDesc.type_attributes = 0x8E;
    genericIsrDesc.offset_1 = ((uint32_t) generic_isr) & 0xFFFF;
	genericIsrDesc.offset_2 = (((uint32_t) generic_isr) >> 16) & 0xFFFF;
    genericIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    for (int k=0; k<256; k++){
        global_IDT[k] = (*(uint64_t*) &genericIsrDesc);
    }

    PIC_remap(33, 33+8);
    
    /*Setup logger del kernel*/
    serial_init(stdout_port);
    /*Set UART interrupt routine*/
    InterruptDescriptor32 uartIsrDesc = InterruptDescriptor32Init;
    uartIsrDesc.type_attributes = 0x8E;
    uartIsrDesc.offset_1 = ((uint32_t) uart_isr) & 0xFFFF;
	uartIsrDesc.offset_2 = (((uint32_t) uart_isr) >> 16) & 0xFFFF;
    uartIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/
    global_IDT[33+3] = (*(uint64_t*) &uartIsrDesc);
    IRQ_clear_mask(4);

    /*Boot Welcome text*/
    serial_writestring(stdout_port,"MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");

    if (gdt_check(GDTR)){
        serial_writestring(stdout_port,"<INFO> GDTR content consistent\n");
    }else{
        serial_writestring(stdout_port,"<WARNING> GDTR content NOT CONSISTENT!\n");
    }

    if (check_idt(IDTR)){
        serial_writestring(stdout_port,"<INFO> IDTR content consistent\n");
    }else{
        serial_writestring(stdout_port,"<WARNING> IDTR content NOT CONSISTENT!\n");
    }


    uint32_t x = 0xC1A0;
    kprint("Prova di kprint: %X", &x);
    // setup_exc_it();
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/
    // asm("div %edx");
    // asm("int $1");

    while(1){}
    // Kernel function is exiting here
}