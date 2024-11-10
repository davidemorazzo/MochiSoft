#include "kernel/microcode.h"

void outb(uint16_t port, uint8_t val){
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd" (port) : "memory");
}


uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ("inb %w1, %b0": "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

/* Send byte to unused IO port to introduce small delay */
void wait_io(){
	outb(0x80, 0);
}

void disable_it(){
    asm("cli;");
}

void enable_it(){
    asm("sti;");
}

void lgdt(void* base, uint16_t size){
	struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) GDTR = { size, base };

    asm ( "lgdt %0" : : "m"(GDTR) );  // let the compiler choose an addressing mode
}

xDTR sgdt(){
    xDTR this_gdtr;	
	asm ("sgdt %0" : "=m"(this_gdtr) : );
	return this_gdtr;
}

void lidt(void* base, uint16_t size){
	struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };

    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

xDTR sidt(){
    xDTR this_idtr;	
	asm ("sidt %0" : "=m"(this_idtr) : );
	return this_idtr;
}

void interrupt(uint8_t irq){
    // __asm__ volatile ("int %0" : : "m"(irq) :);
    // asm volatile ("int $0x24;");
}

uint32_t read_eflags(){
    uint32_t eflags;
    __asm__("pushf; pop %0" : "=m" (eflags) :);
    return eflags;
}