/*Agosto 2024 - Davide Morazzo*/

#ifndef INCLUDE_KERNEL_CORE_H
#define INCLUDE_KERNEL_CORE_H 1

#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/tty.h"

class kernelCore {
public:
	kernelCore();

private:
	void setup_interrupts();

	GDT GlobalDescriptorTable;
	IDT InterruptDescriptorTable;
	terminal serial_term;
};


#endif