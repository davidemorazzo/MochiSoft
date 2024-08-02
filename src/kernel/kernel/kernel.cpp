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
    char buffer[100];
    int i= 0 ;

    terminal term;
    term.writestring("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");
    term.writestring("> Ready\n");

    while(1){
        buffer[i] = term.echo();
        if (buffer[i] == '\r'){
            buffer[i+1]= '\0';
            term.writestring(buffer);
            term.writestring("\n>");
            i = -1;
        }
        i++;
    }
    
    // Kernel function is exiting here
}

}