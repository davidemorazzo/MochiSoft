#ifndef INCLUDE_KERNEL_UART_H
#define INCLUDE_KERNEL_UART_H 1

/*
August 2024 - Davide Morazzo
Informazioni principali prese da https://wiki.osdev.org/Serial_Ports
*/

class uart_driver
{
public:
    uart_driver(){uart_driver(0x3f8);};
    uart_driver(unsigned short dev_address);

    char read_byte();
    void write_byte(char c);
    
private:
    int is_byte_received();
    int is_tx_empty();
    unsigned short device_address;
};

#endif