#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "kernel/tty.h"
#include "kernel/gdt.h"
#include "string.h"


#if defined (__linux__)
//#error "Your are not using a cross compiler. This will cause problems"
#endif

#if !defined(__i386__)
//#error "ix86-elf compiler is needed"
#endif

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/
extern "C"{

void kernel_main (void){
    
    /* ======= GLOBAL DESCRIPTOR TABLE ====== */
    /* setup - flat layout
    all the segments are overlapped in the 0 to 4GB memory space
    */

    GDT_descriptor null_seg, kernel_code_seg, kernel_data_seg, user_code_seg, user_data_seg, task_state_seg;
    kernel_code_seg.limit = 0xFFFFF;
    kernel_code_seg.access = 0x9A;
    kernel_code_seg.flags = 0xC;
    /*------------------------*/
    kernel_data_seg.limit = 0xFFFFF;
    kernel_data_seg.access = 0x92;
    kernel_data_seg.flags = 0xC;
    /*------------------------*/
    user_code_seg.limit = 0xFFFFF;
    user_code_seg.access = 0xFA;
    user_code_seg.flags = 0xC;
    /*------------------------*/
    user_data_seg.limit = 0xFFFFF;
    user_data_seg.access = 0xF2;
    user_data_seg.flags = 0xC;
    /*------------------------*/
    //TODO: Task state segment


    GDT_table main_GDT_table;
    main_GDT_table.base[0] = encode_gdt_descriptor(null_seg);
    main_GDT_table.base[1] = encode_gdt_descriptor(kernel_code_seg);
    main_GDT_table.base[2] = encode_gdt_descriptor(kernel_data_seg);
    main_GDT_table.base[3] = encode_gdt_descriptor(user_code_seg);
    main_GDT_table.base[4] = encode_gdt_descriptor(user_data_seg);
    //TODO: Task state segment
    main_GDT_table.size = 5;
    main_GDT_table.size = main_GDT_table.size*8-1;

    /* Carica il registro con l'istruzione apposita */
    lgdt(main_GDT_table.base, main_GDT_table.size);
    volatile GDTR gdtr_check = sgdt();

    char buffer[100];
    int i= 0 ;

    terminal term;
    term.writestring("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");
    term.writestring("> Ready\n");

    term.writestring("GDT table base address: ");
    term.printhex((uint64_t)&main_GDT_table.base);
    term.putchar('\n');
    term.writestring("GDTR content: 0x");
    term.printhex((void*)&gdtr_check, 6);



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