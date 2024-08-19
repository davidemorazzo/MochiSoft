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