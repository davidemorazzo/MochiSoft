#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "string.h"

#include "kernel/tty.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/PIC-8259.h"
#include "kernel/microcode.h"

#if defined (__linux__)
//#error "Your are not using a cross compiler. This will cause problems"
#endif

#if !defined(__i386__)
//#error "ix86-elf compiler is needed"
#endif

terminal *uart_term_glbl = nullptr;

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
extern "C"{

void uart_isr(){
    uart_term_glbl->writestring("UART Interrupt!\n");
    uart_term_glbl->uart_ISR();
    PIC_sendEOI(4);
}

void generic_isr(){
    __asm__("pushal");
    // uart_term_glbl->writestring("Generic Interrupt!\n");
    for (int j=0; j<255; j++){
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

    disable_it();
    InterruptDescriptor32 uartIsrDesc;
    uartIsrDesc.type_attributes = 0x8E;
    uartIsrDesc.offset_1 = ((uint32_t) isr_0x24_wrapper) & 0xFFFF;
	uartIsrDesc.offset_2 = (((uint32_t) isr_0x24_wrapper) >> 16) & 0xFFFF;
    uartIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    InterruptDescriptor32 genericIsrDesc;
    genericIsrDesc.type_attributes = 0x8E;
    genericIsrDesc.offset_1 = ((uint32_t) generic_isr) & 0xFFFF;
	genericIsrDesc.offset_2 = (((uint32_t) generic_isr) >> 16) & 0xFFFF;
    genericIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    // IDTR.add_entry(test, 0x8); // Double fault exception
    // IDTR.add_entry(test, 0xC); //IRQ#4 mapped by default in 0xC for COM1
    for (int k=0; k<256; k++){
        IDTR.add_entry(genericIsrDesc, k);
    }
    IDTR.add_entry(uartIsrDesc, 33+4); // UART
    // IDTR.add_entry(uartIsrDesc, 33+3); // UART
	IDTR.load_idt(); /*Load IDTR*/

    PIC_remap(33, 33+8);
    IRQ_clear_mask(4);
    // IRQ_clear_mask(3);

    /* ======= TERMINAL ====== */
    terminal uart_term;
    uart_term_glbl = &uart_term;

    uart_term.writestring("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");

    if (GDTR.check_gdt()){
        uart_term.writestring("<INFO> GDTR content consistent\n");
    }else{
        uart_term.writestring("<WARNING> GDTR content NOT CONSISTENT!\n");
    }

    if (IDTR.check_idt()){
        uart_term.writestring("<INFO> IDTR content consistent\n");
    }else{
        uart_term.writestring("<WARNING> IDTR content NOT CONSISTENT!\n");
    }

    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/


    while(1){
        // char itReg = uart_term_glbl->driver.read_reg(2);
        // uart_term_glbl->putchar((itReg&0x01)+'0');
        // uart_term_glbl->putchar((itReg>>1 &0x03)+'0');
        // uart_term_glbl->putchar((uart_term_glbl->driver.is_byte_received())+'0');
        // uart_term_glbl->putchar('\n');
        // buffer[i] = term.echo();
        // if (buffer[i] == '\r'){
        //     buffer[i+1]= '\0';
        //     term.writestring(buffer);
        //     term.writestring("\n>");
        //     i = -1;
        // }
        // i++;
    }
    
    // Kernel function is exiting here
}



}

