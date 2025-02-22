#include "kernel/modules/tty.h"
#include "kernel/uart.h"
#include "string.h"
#include "stdio.h"
#include "kernel/syscall.h"
#include "kernel/kglobals.h"
#include "kernel/kheap.h"
#include "kernel/kstdio.h"

void serial_init(unsigned short port){
    serial_clear_screen(port);
    if(TTY_CIRC_BUF_RX.buf == NULL){
        TTY_CIRC_BUF_RX.buf=(char*) kmalloc(sizeof(char)*TTY_CIRC_BUF_RX.size);
        if (TTY_CIRC_BUF_RX.buf == NULL){
            KLOGERROR("TTY_CIRC_BUF_RX = NULL");
        }
        TTY_CIRC_BUF_RX.read_ptr = TTY_CIRC_BUF_RX.buf;
        TTY_CIRC_BUF_RX.write_ptr = TTY_CIRC_BUF_RX.buf;
    }
}

void serial_putchar(unsigned short port, char c){
    uart_write(port, c);
}

void serial_write(unsigned short port, const char * data, size_t size){
    for (size_t i = 0; i < size; i++)
    {
        serial_putchar(port, data[i]);
    }
}

void serial_writestring(unsigned short port, const char * data){
    sys_write(stdout, data, strlen(data));
}

void serial_ISR(const unsigned short port){
    uint8_t iir = uart_read_reg(port, 2);    // Read Interrupt Identification Register
    // uint8_t it_pending = iir & 0x1;         // Interrupt pending
    uint8_t it_state = iir >> 1 & 0x3;      // Interrupt state
    // uint8_t it_timeout = iir >> 3 & 0x1;    // Timeout Interrupt Pending (UART 16550) or Reserved
    // uint8_t fifo_state = iir >> 6 & 0x3;    // Fifo buffer state

    switch(it_state){
        case 0:     // MODEM STATUS
            break;  
        case 1:     // TRANSMITTER HOLDING REGISTER EMPTY
            break;
        case 2:     // RECEIVED DATA AVAILABLE
            char data = uart_read(port);

            // Inserire dati in buffer circolare TTY
            unsigned int offset = TTY_CIRC_BUF_RX.write_ptr - TTY_CIRC_BUF_RX.buf;
            offset = (offset + 1) % TTY_CIRC_BUF_RX.size;
            TTY_CIRC_BUF_RX.write_ptr = TTY_CIRC_BUF_RX.buf + offset;  
            *TTY_CIRC_BUF_RX.write_ptr = data; 
            
            // Temporary implementation => "echo"
            // uart_write(port, data);
            // char c[6] = {0};
            // sys_read(stdin, (void *)c, 5);
            // sys_write(stdout, (void *)c, 5);
            // if ((TTY_CIRC_BUF_RX.write_ptr - TTY_CIRC_BUF_RX.read_ptr) >= 5){
            // }
            
            break;
        case 3:     // RECEIVER LINE STATUS
            break;
    }
}

void serial_clear_screen(unsigned short port){
    serial_writestring(port, "\x1b[2J\x1b[H");
}

