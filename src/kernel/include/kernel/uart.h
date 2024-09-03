#ifndef INCLUDE_KERNEL_UART_H
#define INCLUDE_KERNEL_UART_H 1

/*
August 2024 - Davide Morazzo
Informazioni principali prese da https://wiki.osdev.org/Serial_Ports
*/
#include <stddef.h>
#include <stdint.h> 

class uart_driver
{
public:
    uart_driver(){uart_driver(0x3f8);};
    uart_driver(unsigned short dev_address);

    char read_byte();
    void write_byte(char c);
    uint8_t read_reg(size_t offset);
    int is_byte_received();
    
private:
    int is_tx_empty();
    unsigned short device_address;
};

#endif