#include "kernel/exceptions.h"
#include "kernel/logging.h" 

void isr00 (){
    // global_logger->writestring("<Exception 0x00> Division by zero\n");
}

void isr01 (){
    // global_logger->writestring("<Exception 0x01> Debug\n");
}

void setup_exc_it(){
    InterruptDescriptor32 desc;
    SET_IT_VEC(desc, isr_0x00_wrapper, 0x00);
    SET_IT_VEC(desc, isr_0x01_wrapper, 0x01);
}