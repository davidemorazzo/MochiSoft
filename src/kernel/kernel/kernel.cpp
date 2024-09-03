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
    uart_term_glbl->uart_ISR();
    PIC_sendEOI(4);
}

extern "C" void isr_0x24_wrapper(void);

void kernel_main (void){
    
    /* ======= GLOBAL DESCRIPTOR TABLE ====== */
    GDT GDTR;
    /* ===== INTERRUPT DESCRIPTOR TABLE ====== */
    IDT IDTR;

    disable_it();
    InterruptDescriptor32 test;
    test.type_attributes = 0x8E;
    test.offset_1 = ((uint32_t) isr_0x24_wrapper) & 0xFFFF;
	test.offset_2 = (((uint32_t) isr_0x24_wrapper) >> 16) & 0xFFFF;
    test.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    // InterruptDescriptor32 uart_it_routine;
    // test.type_attributes = 0x8E;
    // test.offset_1 = ((uint32_t) uart_isr) & 0xFFFF;
	// test.offset_2 = (((uint32_t) uart_isr) >> 16) & 0xFFFF;
    // test.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

	IDTR.load_idt(); /*Load IDTR*/
    // IDTR.add_entry(test, 0x8); // Double fault exception
    // IDTR.add_entry(test, 0xC); //IRQ#4 mapped by default in 0xC for COM1
    // IDTR.add_entry(test, 0x24); // UART
    for (int k=0; k<256; k++){
        IDTR.add_entry(test, k);
    }

    initialize_pic();
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/

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



    while(1){
        char itReg = uart_term_glbl->driver.read_reg(2);
        uart_term_glbl->putchar((itReg&0x01)+'0');
        uart_term_glbl->putchar((itReg>>1 &0x03)+'0');
        uart_term_glbl->putchar((uart_term_glbl->driver.is_byte_received())+'0');
        uart_term_glbl->putchar('\n');
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

