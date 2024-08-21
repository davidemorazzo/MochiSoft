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

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
extern "C"{

void my_func(){
// void my_func(void *esp){
    volatile int x;
    x += 10;
    x += 10;
    PIC_sendEOI(4);
}

void kernel_main (void){
    
    /* ======= GLOBAL DESCRIPTOR TABLE ====== */
    GDT GDTR;

    /* ===== INTERRUPT DESCRIPTOR TABLE ====== */
    IDT IDTR;

    InterruptDescriptor32 test;
	test.type_attributes = INTERRUPT_GATE32 | GD_PRESENT | (DPL0<<5);
	test.offset_1 = ((uint32_t) my_func) & 0xFFFF;
	test.offset_2 = (((uint32_t) my_func) >> 16) & 0xFFFF;
	test.selector = DPL0;  		// RPL=0 => RING 0
	test.selector |= 0x0;  		// TI=0 => GDT table selected 
	test.selector |= 1 << 3;	// Index=1 =>Kernel code segment

    for (int i=0; i <= 0xFF; i++){
        IDTR.add_entry(test, i);
    }

	IDTR.load_idt(); /*Load IDTR*/
    IDTR.add_entry(test, 0x20+4); //IRQ#4 mapped by default in 0xC for COM1

    initialize_pic();
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/
    interrupt(0x20+4);

    /* ======= TERMINAL ====== */
    char buffer[100];
    int i= 0 ;

    terminal term;
    term.writestring("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");

    if (GDTR.check_gdt()){
        term.writestring("<INFO> GDTR content consistent\n");
    }else{
        term.writestring("<WARNING> GDTR content NOT CONSISTENT!\n");
    }

    if (IDTR.check_idt()){
        term.writestring("<INFO> IDTR content consistent\n");
    }else{
        term.writestring("<WARNING> IDTR content NOT CONSISTENT!\n");
    }



    while(1){
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

