/* Settembre 2024 - Davide Morazzo 

Funzioni per gestire le eccezioni. sono collegate con i vettori 
di interrupt da zero a 32.
Seguendo : https://wiki.osdev.org/Exceptions#FPU_Error_Interrupt 
*/

#ifndef INCLUDE_KERNEL_EXCEPTIONS_H
#define INCLUDE_KERNEL_EXCEPTIONS_H 1

void setup_exc_it();

void isr00();
void isr01();
void isr02();
void isr03();
void isr04();
void isr05();
void isr06();
void isr07();
void isr08();
void isr09();
void isr0A();
void isr0B();
void isr0C();
void isr0D();
void isr0E();
void isr0F();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr1A();
void isr1B();
void isr1C();
void isr1D();
void isr1E();
void isr1F();



void isr_0x00_wrapper();
void isr_0x01_wrapper();
void isr_0x02_wrapper();
void isr_0x03_wrapper();
void isr_0x04_wrapper();
void isr_0x05_wrapper();
void isr_0x06_wrapper();
void isr_0x07_wrapper();
void isr_0x08_wrapper();
void isr_0x09_wrapper();
void isr_0x0A_wrapper();
void isr_0x0B_wrapper();
void isr_0x0C_wrapper();
void isr_0x0D_wrapper();
void isr_0x0E_wrapper();
void isr_0x0F_wrapper();
void isr_0x10_wrapper();
void isr_0x11_wrapper();
void isr_0x12_wrapper();
void isr_0x13_wrapper();
void isr_0x14_wrapper();
void isr_0x15_wrapper();
void isr_0x16_wrapper();
void isr_0x17_wrapper();
void isr_0x18_wrapper();
void isr_0x19_wrapper();
void isr_0x1A_wrapper();
void isr_0x1B_wrapper();
void isr_0x1C_wrapper();
void isr_0x1D_wrapper();
void isr_0x1E_wrapper();
void isr_0x1F_wrapper();



#endif