#include "kernel/exceptions.h"
#include "kernel/idt.h"
// #include "kernel/tty.h"
#include "kernel/kstdio.h"

void isr00 (){
    // serial_writestring(STDIO_PORT, "<Exception 0x00> Division by zero\n");
    kprint("<Exception 0x00> Division by zero\n");
}

void isr01 (){
    // serial_writestring(STDIO_PORT, "<Exception 0x01> Debug\n");
    kprint("<Exception 0x01> Debug\n");
}

void setup_exc_it(){
    InterruptDescriptor32 desc;
    SET_IT_VEC(desc, isr_0x00_wrapper, 0x00);
    SET_IT_VEC(desc, isr_0x01_wrapper, 0x01);
    SET_IT_VEC(desc, isr_0x80_wrapper, 0x80);
}