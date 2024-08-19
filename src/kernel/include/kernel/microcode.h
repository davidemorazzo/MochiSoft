#ifndef INCLUDE_KERNEL_MICROCODE_H
#define INCLUDE_KERNEL_MICROCODE_H 1

#include <stdint.h>

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);

/* Send byte to unused IO port to introduce small delay */
void wait_io();

void disable_it();

void enable_it();

#endif