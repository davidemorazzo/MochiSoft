#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#include "kernel/gdt.h"
#include "kernel/idt.h"
extern "C"{
    #include "kernel/microcode.h"
    #include "string.h"
    #include "kernel/PIC-8259.h"
    #include "kernel/tty.h"
}
#include "kernel/kstdio.h"
#include "kernel/exceptions.h"

#if defined (__linux__)
//#error "Your are not using a cross compiler. This will cause problems"
#endif

#if !defined(__i386__)
//#error "ix86-elf compiler is needed"
#endif

unsigned short stdout_port = UART0;
IDT *global_IDT;

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
extern "C"{


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

extern "C" void isr_0x24_wrapper(void);

void kernel_main (void){
    
    /* ======= GLOBAL DESCRIPTOR TABLE ====== */
    GDT GDTR;
    /* ===== INTERRUPT DESCRIPTOR TABLE ====== */
    IDT IDTR;
    global_IDT = &IDTR;

    disable_it();    

    InterruptDescriptor32 genericIsrDesc;
    genericIsrDesc.type_attributes = 0x8E;
    genericIsrDesc.offset_1 = ((uint32_t) generic_isr) & 0xFFFF;
	genericIsrDesc.offset_2 = (((uint32_t) generic_isr) >> 16) & 0xFFFF;
    genericIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    for (int k=0; k<256; k++){
        IDTR.add_entry(genericIsrDesc, k);
    }
	IDTR.load_idt(); /*Load IDTR*/

    PIC_remap(33, 33+8);
    
    /*Setup logger del kernel*/
    serial_init(stdout_port);
    /*Set UART interrupt routine*/
    InterruptDescriptor32 uartIsrDesc;
    uartIsrDesc.type_attributes = 0x8E;
    uartIsrDesc.offset_1 = ((uint32_t) uart_isr) & 0xFFFF;
	uartIsrDesc.offset_2 = (((uint32_t) uart_isr) >> 16) & 0xFFFF;
    uartIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/
    IDTR.add_entry(uartIsrDesc, 33+3); // UART
    IRQ_clear_mask(4);

    /*Boot Welcome text*/
    serial_writestring(stdout_port,"MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");

    if (GDTR.check_gdt()){
        serial_writestring(stdout_port,"<INFO> GDTR content consistent\n");
    }else{
        serial_writestring(stdout_port,"<WARNING> GDTR content NOT CONSISTENT!\n");
    }

    if (IDTR.check_idt()){
        serial_writestring(stdout_port,"<INFO> IDTR content consistent\n");
    }else{
        serial_writestring(stdout_port,"<WARNING> IDTR content NOT CONSISTENT!\n");
    }

    uint32_t x = 0xC1A0;
    kprint("Prova di kprint: %X", &x);
    setup_exc_it();
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/
    asm("div %edx");
    asm("int $1");

    while(1){}
    // Kernel function is exiting here
}



}

