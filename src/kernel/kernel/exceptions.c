#include "kernel/exceptions.h"
#include "kernel/idt.h"
// #include "kernel/tty.h"
#include "kernel/kstdio.h"
#include "kernel/syscall.h"

char exc_lookup[32][40] = {
    "Division Error\0",
    "Debug\0",
    "Non-maskable Interrupt\0",
    "Breakpoint\0",
    "Overflow\0",
    "Bound Range Exceeded\0",
    "Invalid Opcode\0",
    "Device Not Available\0",
    "Double Fault\0",
    "Coprocessor Segment Overrun\0",
    "Invalid TSS\0",
    "Segment Not Present\0",
    "Stack-Segment Fault\0",
    "General Protection Fault\0",
    "Page Fault\0",
    "Reserved\0",
    "x87 Floating-Point Exception\0",
    "Alignment Check\0",
    "Machine Check\0",
    "SIMD Floating-Point Exception\0",
    "Virtualization Exception\0",
    "Control Protection Exception\0",
    "Reserved\0",
    "Hypervisor Injection Exception\0",
    "VMM Communication Exception\0",
    "Security Exception\0",
    "Reserved\0",
    "Triple Fault\0",
};

#define decl_isr(n)                     \
void isr##n (){                         \
    KLOGERROR("<Exception 0x" #n "> %s\n", &exc_lookup[0x##n]);   \
    KLOGERROR("KERNEL PANIC :)"); \
    while(1) asm("hlt"); \
}                                       


void isr00 (){
    KLOGERROR("<Exception 0x00> Division by zero\n");
}
void isr01 (){
    KLOGERROR("<Exception 0x01> Debug\n");
}
void isr02 (){
    KLOGERROR("<Exception 0x02> Non-maskable interrupt\n");
}
void isr03 (){
    KLOGERROR("<Exception 0x03> Breakpoint\n");
}
void isr04 (){
    KLOGERROR("<Exception 0x04> Debug\n");
}

void isr0D(void *error_code){
    KLOGERROR("<Exception 0x0D> %s => Error code: %X\n", &exc_lookup[0x0D], error_code);
    KLOGERROR("KERNEL PANIC :)"); \
    while(1) asm("hlt"); \
}

decl_isr(05);
decl_isr(06);
decl_isr(07);
decl_isr(08);
decl_isr(09);
decl_isr(0A);
decl_isr(0B);
decl_isr(0C);
// decl_isr(0D);
// decl_isr(0E);
void isr0E(void){
    uint32_t addr;
    __asm__("movl %%cr2, %0" : "=r"(addr) :);
    KLOGERROR("<Exception 0x0E> %s => Error accessing address 0x%X", &exc_lookup[0x0E], addr);
    KLOGERROR("KERNEL PANIC :) IAPA IAPA IAPA :D"); \
    while(1) asm("hlt"); \
}
decl_isr(0F);
decl_isr(10);
decl_isr(11);
decl_isr(12);
decl_isr(13);
decl_isr(14);
decl_isr(15);
decl_isr(16);
decl_isr(17);
decl_isr(18);
decl_isr(19);
decl_isr(1A);
decl_isr(1B);
decl_isr(1C);
decl_isr(1D);
decl_isr(1E);
decl_isr(1F);



void setup_exc_it(){
    InterruptDescriptor32 desc;
    SET_IT_VEC(desc, isr_0x00_wrapper, 0x00);
    // SET_IT_VEC(desc, isr_0x01_wrapper, 0x01);
    SET_IT_VEC(desc, isr_0x02_wrapper, 0x02);
    // SET_IT_VEC(desc, isr_0x03_wrapper, 0x03);
    SET_IT_VEC(desc, isr_0x04_wrapper, 0x04);
    SET_IT_VEC(desc, isr_0x05_wrapper, 0x05);
    SET_IT_VEC(desc, isr_0x06_wrapper, 0x06);
    SET_IT_VEC(desc, isr_0x07_wrapper, 0x07);
    SET_IT_VEC(desc, isr_0x08_wrapper, 0x08);
    // SET_IT_VEC(desc, isr_0x09_wrapper, 0x09);
    SET_IT_VEC(desc, isr_0x0A_wrapper, 0x0A);
    SET_IT_VEC(desc, isr_0x0B_wrapper, 0x0B);
    SET_IT_VEC(desc, isr_0x0C_wrapper, 0x0C);
    SET_IT_VEC(desc, isr_0x0D_wrapper, 0x0D);
    SET_IT_VEC(desc, isr_0x0E_wrapper, 0x0E);
    // SET_IT_VEC(desc, isr_0x0F_wrapper, 0x0F);
    SET_IT_VEC(desc, isr_0x10_wrapper, 0x10);
    SET_IT_VEC(desc, isr_0x11_wrapper, 0x11);
    SET_IT_VEC(desc, isr_0x12_wrapper, 0x12);
    SET_IT_VEC(desc, isr_0x13_wrapper, 0x13);
    SET_IT_VEC(desc, isr_0x14_wrapper, 0x14);
    SET_IT_VEC(desc, isr_0x15_wrapper, 0x15);
    // SET_IT_VEC(desc, isr_0x16_wrapper, 0x16);
    // SET_IT_VEC(desc, isr_0x17_wrapper, 0x17);
    // SET_IT_VEC(desc, isr_0x18_wrapper, 0x18);
    // SET_IT_VEC(desc, isr_0x19_wrapper, 0x19);
    // SET_IT_VEC(desc, isr_0x1A_wrapper, 0x1A);
    // SET_IT_VEC(desc, isr_0x1B_wrapper, 0x1B);
    SET_IT_VEC(desc, isr_0x1C_wrapper, 0x1C);
    SET_IT_VEC(desc, isr_0x1D_wrapper, 0x1D);
    SET_IT_VEC(desc, isr_0x1E_wrapper, 0x1E);
    // SET_IT_VEC(desc, isr_0x1F_wrapper, 0x1F);
    


    SET_IT_VEC(desc, isr_0x80_wrapper, 0x80); // System call
}