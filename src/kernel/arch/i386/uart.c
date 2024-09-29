#include "kernel/uart.h"
#include "kernel/microcode.h"

/*
August 2024 - Davide Morazzo
Informazioni principali prese da https://wiki.osdev.org/Serial_Ports
*/


char uart_read(unsigned short port){
    return inb(port);
}

void uart_write(unsigned short port, char c){
    while((uart_read_reg(port, 5)&0x20) == 0); //Check TX empty
    outb(port, c);
}

uint8_t uart_read_reg(unsigned short port, size_t offset){
    return inb(port + offset);
}

void uart_setup(unsigned short port){
    outb(port + 3, 0x00);    // Disable DLAB (set baud rate divisor)
    outb(port + 1, 0x01);    // Enable interrupt "received data available"

    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E);    // Set in loopback mode, test the serial chip

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(port + 4, 0x0F);
}

// /*Check if there is data to read in the FIFO*/
// int uart_driver::is_byte_received(){
//     return inb(device_address + 5) & 1;
// }

// char uart_driver::read_byte(){
//     while (is_byte_received() == 0);
//     return inb(device_address);
// }

// int uart_driver::is_tx_empty(){
//     return inb(device_address + 5) & 0x20;
// }

// void uart_driver::write_byte(char c){
//     while(is_tx_empty() == 0);
//     outb(device_address, c);
// }

// uint8_t uart_driver::read_reg(size_t offset){
//     return inb(this->device_address + offset);
// }