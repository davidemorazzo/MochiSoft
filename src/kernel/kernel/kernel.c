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
#include "kernel/kheap.h"
#include "dev/RTC.h"
#include "kernel/syscall.h"

#include "time.h"


uint64_t global_IDT[255] = {0};
uint64_t global_GDT[50] = {0};
/*Heap stuff*/
// #define KHEAP_SIZE 524288000
#define KHEAP_SIZE 1024*1024*10

void *kheap[KHEAP_SIZE];
// _kAllocStatus kHeapStatus;

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/


void uart_isr(){
    __asm__("pushal");
    serial_ISR(STDIO_PORT);
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

    /* ===== SETUP HEAP ====== */
    _kAllocStatus* kHeapStatus = _setupHeap(kheap, KHEAP_SIZE);
    char * array0, *array1;
    array0 = kmalloc(5*sizeof(char));
    array1 = kmalloc(5*sizeof(char));
    array0[0] = 0xFF;
    array1[0] = 0xFF;
    array0[1] = 0xFF;
    array1[1] = 0xFF;
    array0[4] = 0xFF;
    array1[4] = 0xFF;
    _kfree(array0, 5*sizeof(char));
    _kfree(array1, 5*sizeof(char));
    

    /* ===== INTERRUPT DESCRIPTOR TABLE ====== */
    xDTR IDTR;
    IDTR.base = global_IDT;
    IDTR.length = 256*8-1;
    load_idt(IDTR); /*Load IDTR*/

    InterruptDescriptor32 genericIsrDesc;
    genericIsrDesc.zero=0x0;
    genericIsrDesc.type_attributes = 0x8E;
    genericIsrDesc.offset_1 = ((uint32_t) generic_isr) & 0xFFFF;
	genericIsrDesc.offset_2 = (((uint32_t) generic_isr) >> 16) & 0xFFFF;
    genericIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    for (int k=0; k<256; k++){
        global_IDT[k] = (*(uint64_t*) &genericIsrDesc);
    }

    PIC_remap(33, 33+8);
    
    /*Setup logger del kernel*/
    serial_init(STDIO_PORT);
    /*Set UART interrupt routine*/
    InterruptDescriptor32 uartIsrDesc;
    uartIsrDesc.zero=0x0;
    uartIsrDesc.type_attributes = 0x8E;
    uartIsrDesc.offset_1 = ((uint32_t) uart_isr) & 0xFFFF;
	uartIsrDesc.offset_2 = (((uint32_t) uart_isr) >> 16) & 0xFFFF;
    uartIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/
    global_IDT[33+3] = (*(uint64_t*) &uartIsrDesc);
    IRQ_clear_mask(4);

    setup_exc_it();
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/

    /*Boot Welcome text*/
    kprint("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n");
    struct tm now;
    rtc_get_time(&now);
    kprint("%s\n\n", asctime(&now));

    if (gdt_check(GDTR)){
        kprint("GDTR ok, base address: 0x%X\n", &GDTR.base);
    }else{
        kprint("GDTR content NOT CONSISTENT!\n");
    }

    if (check_idt(IDTR)){
        kprint("IDTR ok, base address: 0x%X\n", &IDTR.base);
    }else{
        kprint("IDTR content NOT CONSISTENT!\n");
    }

    //Setup RTC
    outb(0x70, 0x8A);

    // System call 201 (time)

    while(1){
        time_t now;
        time(&now);
        kprint("Syscall: %d\n", &now);
        // for (int f=0;f<3000000;f++){}
        // kprint("End of while\n");
    }
    // Kernel function is exiting here
}