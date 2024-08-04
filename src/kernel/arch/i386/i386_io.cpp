#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val){
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd" (port) : "memory");
}


static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ("inb %w1, %b0": "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

/* Send byte to unused IO port to introduce small delay */
static inline void wait_io(){
	outb(0x80, 0);
}