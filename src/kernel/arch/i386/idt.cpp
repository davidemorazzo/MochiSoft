#include "kernel/idt.h"
#include "kernel/microcode.h"

IDT::IDT()
{
	
}

void IDT::load_idt()
{
	//FIXME: capire come disattivare gli interrupt senza generare problemi
	lidt(this->base, this->size);
}

char IDT::add_entry(InterruptDescriptor32 descriptor, uint8_t index)
{
	/*Encode descriptor*/
	uint8_t encoded[8];
	encoded[0] = descriptor.offset_1 & 0xFF;
	encoded[1] = (descriptor.offset_1 >> 8) & 0xFF;
	encoded[2] = descriptor.selector & 0xFF;
	encoded[3] = (descriptor.selector >> 8) & 0xFF;
	encoded[4] = descriptor.zero;
	encoded[5] = descriptor.type_attributes;
	encoded[6] = descriptor.offset_2 & 0xFF;
	encoded[7] = (descriptor.offset_2 >> 8) & 0xFF;

	//FIXME: da controllare se funziona
	this->base[index] = (*(uint64_t*) &encoded); 
	
	return 0;
}

char IDT::check_idt(){
	xDTR reg = sidt();
	return (reg.base == (void *)this->base) && (this->size == reg.length);
}

