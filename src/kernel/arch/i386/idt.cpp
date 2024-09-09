#include "kernel/idt.h"
#include "kernel/microcode.h"
#include <PIC-8259.h>

IDT::IDT()
{
	initialize_pic();
	this->load_idt();
}

void IDT::load_idt()
{
	lidt(this->base, this->size-1);
}

char IDT::add_entry(InterruptDescriptor32 descriptor, uint8_t index)
{
	this->base[index] = (*(uint64_t*) &descriptor); 
	
	return 0;
}

InterruptDescriptor32 IDT::generate_descriptor(void func(), uint8_t type, uint16_t segment_sel)
{
	InterruptDescriptor32 d;
	d.offset_1 = (uint32_t)func >> 0 & 0xFFFF;
	d.offset_2 = (uint32_t)func >> 16 & 0xFFFF;
	d.type_attributes = type;
	d.selector = segment_sel;
	return d;
}

char IDT::check_idt()
{
	xDTR reg = sidt();
	return (reg.base == (void *)this->base) && (this->size-1 == reg.length);
}
