#include "kernel/tty.h"
#include "string.h"

terminal::terminal(){
    /* Initialization of the temrminal */
    this->driver = uart_driver(0x3f8);
    this->clear_screen();
};

void terminal::putchar(char c){
    this->driver.write_byte(c);
}

void terminal::write(const char * data, size_t size){
    for (size_t i = 0; i < size; i++)
    {
        this->putchar(data[i]);
    }
}

void terminal::clear_screen(){
    this->putchar(27);
    this->putchar('[');
    this->putchar('2');
    this->putchar('J');
    this->putchar(27);
    this->putchar('[');
    this->putchar('H');
}

void terminal::writestring(const char * data){
    this->write(data, strlen(data));
}

char terminal::echo()
{
    char c = this->driver.read_byte();
    this->driver.write_byte(c);
    return c;
}

void terminal::printhex(uint64_t i){
    char high[9], low[9];
    i32tohex(i >> 32, high);
    i32tohex(i & 0xFFFFFFFF, low);

    this->writestring("0x");
    this->writestring(high);
    this->writestring(low);
}

void terminal::printhex(void * mem, size_t bytes){
    /* From a given 32bit memory address bytes will 
    be printed. No checks on validity of address
    are done*/

    for (int i = bytes-1; i >= 0; i--){
        char s[3];
        bytetohex(((char *) mem )[i], s); //TODO: da fixare
        this->writestring(s);
    }
}

void terminal::uart_ISR(void){
    uint8_t iir = this->driver.read_reg(2);    // Read Interrupt Identification Register
    uint8_t it_pending = iir & 0x1;         // Interrupt pending
    uint8_t it_state = iir >> 1 & 0x3;      // Interrupt state
    uint8_t it_timeout = iir >> 3 & 0x1;    // Timeout Interrupt Pending (UART 16550) or Reserved
    uint8_t fifo_state = iir >> 6 & 0x3;    // Fifo buffer state

    // if (it_timeout) return;

    // if (it_pending){
        switch(it_state){
            case 0:     // MODEM STATUS
                break;  
            case 1:     // TRANSMITTER HOLDING REGISTER EMPTY
                break;
            case 2:     // RECEIVED DATA AVAILABLE
                this->putchar(driver.read_byte());
                break;
            case 3:     // RECEIVER LINE STATUS
                break;
        }
    // }    
}