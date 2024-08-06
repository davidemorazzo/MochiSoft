#ifndef INCLUDE_KERNEL_TTY_H
#define INCLUDE_KERNEL_TTY_H 1

#include <stddef.h>
#include <stdint.h>
#include "uart.h"

class terminal{
public:
    terminal();
    void putchar(char c);
    void write(const char * data, size_t size);
    void writestring(const char * data);
    char echo();
    void printhex(uint64_t i);
private:
    void clear_screen();
    uart_driver driver;
};

#endif