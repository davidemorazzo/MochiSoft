#include "kernel/core.h"

kernelCore::kernelCore()
{
	this->setup_interrupts();

	this->serial_term.writestring("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n\n");
	if (! this->GlobalDescriptorTable.check_gdt()){
		this->serial_term.writestring("<WARNING> GDTR content NOT CONSISTENT!\n");
	}

	if (!this->InterruptDescriptorTable.check_idt()){
		this->serial_term.writestring("<WARNING> IDTR content NOT CONSISTENT!\n");
	}

}