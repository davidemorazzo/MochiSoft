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


void outl(uint16_t port, uint32_t val){
    __asm__ volatile ( 
        "push %%dx;"
        "mov %w0, %%dx;"
        "mov %d1, %%eax;"
        "out %%eax, %%dx;"
        "pop %%dx;" 
        : "=m" (port), "=m" (val) :
    );
}

uint32_t inl(uint16_t port){
    uint32_t result;
    __asm__ volatile (
        "push %%dx;"
        "mov %w0, %%dx;"
        "inl %%dx, %%eax;"
        "mov %%eax, %d1;" 
        "pop %%dx;"
        : "=m" (port), "=m" (result) :
    );
    return result;
}

uint32_t cr3(){
    uint32_t reg=0;
    __asm__ (
        "mov %%cr3, %d0" : "=r"(reg) :
    );
    return reg;
}

uint8_t bit_test_set(void * addr, uint32_t offset){
    uint32_t eflags_reg;
    // uint32_t addr_cpy = (uint32_t)addr;
    __asm__(
        "bts %1, %2;"
        "pushf;"
        "pop %0;"
        : "=m" (eflags_reg)
        : "r" (offset), "m" (*(uint32_t*)addr)
    );

    return !(eflags_reg & 1);
}