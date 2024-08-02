#ifndef INCLUDE_KERNEL_UART_H
#define INCLUDE_KERNEL_UART_H 1

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