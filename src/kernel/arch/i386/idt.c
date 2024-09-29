#include "kernel/idt.h"
#include "kernel/microcode.h"

void load_idt(xDTR idt)
{
	lidt(idt.base, idt.length);
}

char check_idt(xDTR idt){
	xDTR reg = sidt();
	return (reg.base == idt.base) && (idt.length == reg.length);
}

