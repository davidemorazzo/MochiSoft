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
    uart_driver terminal = uart_driver(0x3f8);
    terminal.clear_screen();
    terminal.writeline("MochiSoft Inc. - 2024\n\n");
    while(1){
        terminal.writeline("> Insert username:");
        char s[50] = {0};
        terminal.readline(s);
        terminal.writeline("> Line received => ", 0);
        terminal.writeline(s);
    }

    // Kernel function is exiting here
}

}