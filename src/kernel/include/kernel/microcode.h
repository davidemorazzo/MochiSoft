#ifndef INCLUDE_KERNEL_MICROCODE_H
#define INCLUDE_KERNEL_MICROCODE_H 1

#include <stdint.h>

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);

/* Send byte to unused IO port to introduce small delay */
void wait_io();

void disable_it();

void enable_it();

typedef struct{
	uint16_t length;	/*Length of table in bytes*/
	uint32_t* base; 		/*32-bit address*/
}__attribute__((packed)) xDTR;

void lgdt(void* base, uint16_t size);

xDTR sgdt();

void lidt(void* base, uint16_t size);

xDTR sidt();

void interrupt(uint8_t irq);

#endif