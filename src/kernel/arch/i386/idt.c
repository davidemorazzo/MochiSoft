#include "kernel/idt.h"
#include "kernel/microcode.h"

void load_idt(xDTR idt)
{
	lidt(idt.base, idt.length);
}

// char IDT::add_entry(InterruptDescriptor32 descriptor, uint8_t index)
// {
// 	/*Encode descriptor*/
// 	// uint8_t encoded[8];
// 	// encoded[0] = descriptor.offset_1 & 0xFF;
// 	// encoded[1] = (descriptor.offset_1 >> 8) & 0xFF;
// 	// encoded[2] = descriptor.selector & 0xFF;
// 	// encoded[3] = (descriptor.selector >> 8) & 0xFF;
// 	// encoded[4] = descriptor.zero;
// 	// encoded[5] = descriptor.type_attributes;
// 	// encoded[6] = descriptor.offset_2 & 0xFF;
// 	// encoded[7] = (descriptor.offset_2 >> 8) & 0xFF;

// 	//FIXME: da controllare se funziona
// 	this->base[index] = (*(uint64_t*) &descriptor); 
	
// 	return 0;
// }

char check_idt(xDTR idt){
	xDTR reg = sidt();
	return (reg.base == idt.base) && (idt.length == reg.length);
}

