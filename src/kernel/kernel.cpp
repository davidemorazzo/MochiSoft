#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "/workspaces/MochiSoft/src/UART-driver/uart-driver.cpp"
#include "/workspaces/MochiSoft/src/utils/string.cpp"

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
    // terminal_initialize();
    // const char * s = "\nHello World from MochiSoft kernel!\n";
    // terminal_write(s, strlen(s));

    uart_driver terminal = uart_driver(0x3f8);
    terminal.write_byte('\n');
    terminal.writeline("Hello World!");
    terminal.writeline("\nMochiSoft Inc. - 2024");

    // Kernel function is exiting here
}

}