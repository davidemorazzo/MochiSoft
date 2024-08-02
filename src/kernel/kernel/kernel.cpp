#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <tty.h>

#if defined (__linux__)
#error "Your are not using a cross compiler. This will cause problems"
#endif

#if !defined(__i386__)
#error "ix86-elf compiler is needed"
#endif

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
extern "C"{

void kernel_main (void){
    terminal term;
    term.writestring("Welcome in MochiOS! - 2024\n\n");
    term.writestring("> Ready\n");
    
    // Kernel function is exiting here
}

}