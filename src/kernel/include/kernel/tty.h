#ifndef INCLUDE_KERNEL_TTY_H
#define INCLUDE_KERNEL_TTY_H 1

#include <stddef.h>
#include <stdint.h>
#include "uart.h"

// class terminal{
// public:
//     terminal();
//     void putchar(char c);
//     void write(const char * data, size_t size);
//     void writestring(const char * data);
//     char echo();
//     void printhex(uint64_t i);
//     void printhex(void * mem, size_t bytes);
//     void uart_ISR(void);
// private:
//     uart_driver driver;
//     void clear_screen();
// };

void serial_init(unsigned short port);
void serial_putchar(unsigned short port, char c);
void serial_write(unsigned short port, const char * data, size_t size);
void serial_writestring(unsigned short port, const char * data);
// void serial_printhex(unsigned short port, uint64_t i);
// void serial_printhex(unsigned short port, void * mem, size_t bytes);
void serial_ISR(const unsigned short port);
void serial_clear_screen(unsigned short port);

#endif