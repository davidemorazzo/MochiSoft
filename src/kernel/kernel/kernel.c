#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "string.h"

#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/microcode.h"
#include "kernel/modules/tty.h"
#include "kernel/kstdio.h"
#include "kernel/exceptions.h"
#include "kernel/kheap.h"
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


void generic_isr(){
    __asm__("pushal");
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

    /*Setup logger del kernel*/
    serial_init(UART0);

    setup_exc_it();

    extern void init_devs();
    init_devs(); 
    KLOGINFO("Devices inizializzati")
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/


    if (gdt_check(GDTR)){
        KLOGINFO("GDTR ok, base address: 0x%X", &GDTR.base);
    }else{
        KLOGERROR("GDTR content not consistent!");
    }

    if (check_idt(IDTR)){
        KLOGINFO("IDTR ok, base address: 0x%X", &IDTR.base);
    }else{
        KLOGERROR("IDTR content not consistent!");
    }

    // uint32_t res = 0;
    // __asm__(
    //     "mov $1, %%eax\n\t"
    //     "mov $1, %%ebx\n\t"
    //     "mov %1, %%ecx\n\t"
    //     "mov $20, %%edx\n\t"
    //     "int $0x80\n\t"
    //     "mov %%eax, %0"
    //     : "=m" (res)
    //     : "m" (buf)
    // );

 
    /*Boot Welcome text*/
    time_t now;
    do{
        sys_time(&now);
    }while(now == 0);
    kprint("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n");
    kprint("%s\n\n", asctime(gmtime(&now)));
    KLOGINFO("Avvio MochiOS completato")

    while(1){}
    // Kernel function is exiting here
}