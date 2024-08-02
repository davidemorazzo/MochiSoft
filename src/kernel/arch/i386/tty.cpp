#include "tty.h"
// #include "string.h"

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
    // this->write(data, strlen(data));
}