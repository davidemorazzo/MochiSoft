#include "kernel/uart.h"
#include "kernel/microcode.h"


uart_driver::uart_driver(unsigned short dev_address){
    device_address = dev_address;
    outb(device_address + 1, 0x00);    // Disable all interrupts
    outb(device_address + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(device_address + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(device_address + 1, 0x00);    //                  (hi byte)
    outb(device_address + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(device_address + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(device_address + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(device_address + 4, 0x1E);    // Set in loopback mode, test the serial chip

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(device_address + 4, 0x0F);
}

int uart_driver::is_byte_received(){
    return inb(device_address + 5) & 1;
}

char uart_driver::read_byte(){
    while (is_byte_received() == 0);
    return inb(device_address);
}

int uart_driver::is_tx_empty(){
    return inb(device_address + 5) & 0x20;
}

void uart_driver::write_byte(char c){
    while(is_tx_empty() == 0);
    outb(device_address, c);
}


// #include <stdint.h>
// #include "/workspaces/MochiSoft/src/utils/string.cpp"

// class uart_driver{
// public:
//     uart_driver(unsigned short dev_address){
//         device_address = dev_address;
//         outb(device_address + 1, 0x00);    // Disable all interrupts
//         outb(device_address + 3, 0x80);    // Enable DLAB (set baud rate divisor)
//         outb(device_address + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
//         outb(device_address + 1, 0x00);    //                  (hi byte)
//         outb(device_address + 3, 0x03);    // 8 bits, no parity, one stop bit
//         outb(device_address + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
//         outb(device_address + 4, 0x0B);    // IRQs enabled, RTS/DSR set
//         outb(device_address + 4, 0x1E);    // Set in loopback mode, test the serial chip

//         // If serial is not faulty set it in normal operation mode
//         // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
//         outb(device_address + 4, 0x0F);
//     }

//     int is_byte_received(){
//         return inb(device_address + 5) & 1;
//     }

//     /* read character from the UART buffer and returns it, popping it
//     from the buf */
//     char read_byte(){
//         while (is_byte_received() == 0);
//         return inb(device_address);
//     }

//     int is_tx_empty(){
//         return inb(device_address + 5) & 0x20;
//     }

//     /* write character to the UART buffer */
//     void write_byte(char c){
//         while(is_tx_empty() == 0);
//         outb(device_address, c);
//     }

//     static inline void outb(uint16_t port, uint8_t val){
//         __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd" (port) : "memory");
//     }

//     static inline uint8_t inb(uint16_t port){
//         uint8_t ret;
//         __asm__ volatile ("inb %w1, %b0": "=a"(ret) : "Nd"(port) : "memory");
//         return ret;
//     }

//     void writeline(const char *s, char end='\n'){
//         for(size_t i =0; i<strlen(s); i++){
//             write_byte(s[i]);
//         }
//         if (end!=0) write_byte(end);
//     }

//     void clear_screen(){
//         write_byte(27);
//         write_byte('[');
//         write_byte('2');
//         write_byte('J');
//         write_byte(27);
//         write_byte('[');
//         write_byte('H');
//     }

//     void readline(char * buf){
//         char c;
//         // char buf[50];
//         size_t len = 0;
//         do{
//             c = read_byte();
//             buf[len] = c;
//             len++;
//             // writeline("\n> Line received => ", 0);
//             // writeline(&c);
//         }while(c != '0' && len < 50-1);

//         buf[len-1] = '\0';
//         // writeline("Returning Buffer!");
//         // return buf;
//     }

// private:
//     unsigned short device_address;
// };