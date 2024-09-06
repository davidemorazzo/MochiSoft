/* Settembre 2024 - Davide Morazzo 

Funzioni per gestire le eccezioni. sono collegate con i vettori 
di interrupt da zero a 32.
Seguendo : https://wiki.osdev.org/Exceptions#FPU_Error_Interrupt 
*/

#ifndef INCLUDE_KERNEL_EXCEPTIONS_H
#define INCLUDE_KERNEL_EXCEPTIONS_H 1

#include "kernel/idt.h"

void setup_exc_it();

extern "C" {
    void isr_0x00_wrapper();
    void isr00 ();

    void isr_0x01_wrapper();
    void isr01 ();
}



#endif