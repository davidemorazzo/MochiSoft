#ifndef INCLUDE_KERNEL_UART_H
#define INCLUDE_KERNEL_UART_H 1

/*
August 2024 - Davide Morazzo
Informazioni principali prese da https://wiki.osdev.org/Serial_Ports
*/
#include <stddef.h>
#include <stdint.h> 

#define UART0 0x3f8


void uart_setup(unsigned short port);
char uart_read(unsigned short port);
void uart_write(unsigned short port, char c);
uint8_t uart_read_reg(unsigned short port, size_t offset);

#endif