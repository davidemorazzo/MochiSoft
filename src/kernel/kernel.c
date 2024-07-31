#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "terminal.c"

#if defined (__linux__)
#error "Your are not using a cross compiler. This will cause problems"
#endif

#if !defined(__i386__)
#error "ix86-elf compiler is needed"
#endif

size_t strlen (const char* str){
    size_t size = 0;
    while (str[size])
        size++;
    return size;
}


/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
void kernel_main (void){
    terminal_initialize();
    const char * s = "Hello World from kernel!";
    terminal_write(s, strlen(s));
}