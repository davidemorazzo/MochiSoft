#include "kernel/core.h"

#if defined (__linux__)
//#error "Your are not using a cross compiler. This will cause problems"
#endif

#if !defined(__i386__)
//#error "ix86-elf compiler is needed"
#endif

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
extern "C"{

// extern "C" void isr_0x24_wrapper(void);

void kernel_main (void){
    /*Starts the kernel class*/
    kernelCore kernel;
}



}

