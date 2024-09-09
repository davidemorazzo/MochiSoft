/* File con le funzioni e dati per fare il setup del GDT (Global
Descriptor Table). 

Source: https://wiki.osdev.org/GDT_Tutorial

BASIC GDT ENTRIES
Offset		Use					Content
0x0000 		Null				Base=0
								Limit=0x00000000
								Access byte=0x00
								Flags=0x0

0x0008		Kernel Mode Code	Base= 0x00400000
								Limit=0x003FFFFF
								Access byte=0x9A
								Flags=0xC

0x0010		Kernel Mode Data	Base= 0x00800000
								Limit=0x003FFFFF
								Access byte=0x92
								Flags=0xC

0x0018		Task State Segment	Base= &TSS
								Limit=sizeof(TSS)-1
								Access byte=0x89
								Flags=0x0

*/

#include "kernel/gdt.h"
#include "kernel/microcode.h"

void gdt_init(uint64_t* global_GDT){
	GDT_descriptor kernel_code_seg, kernel_data_seg; //, user_code_seg, user_data_seg, task_state_seg;
    kernel_code_seg.limit = 0xFFFFF;
    kernel_code_seg.access = 0x9A;
    kernel_code_seg.flags = 0xC;
    /*------------------------*/
    kernel_data_seg.limit = 0xFFFFF;
    kernel_data_seg.access = 0x92;
    kernel_data_seg.flags = 0xC;
    /*------------------------*/
    // user_code_seg.limit = 0xFFFFF;
    // user_code_seg.access = 0xFA;
    // user_code_seg.flags = 0xC;
    // /*------------------------*/
    // user_data_seg.limit = 0xFFFFF;
    // user_data_seg.access = 0xF2;
    // user_data_seg.flags = 0xC;
    //TODO: Task state segment

	global_GDT[0] = 0x00; // Null segment
	global_GDT[1] = gdt_encode_desc(kernel_code_seg);
	global_GDT[2] = gdt_encode_desc(kernel_data_seg);
}

void gdt_load(xDTR GDTR)
{
	lgdt(GDTR.base, GDTR.length);
}
char gdt_check(xDTR GDTR){
	xDTR reg = sgdt();
	return (reg.base == GDTR.base);
}

uint64_t gdt_encode_desc(GDT_descriptor desc){
	uint8_t gdt_entry[8] = {0};
	/*Encode limit*/
	gdt_entry[0] = desc.limit & 0xFF;
	gdt_entry[1] = (desc.limit>>8) & 0xFF;
	gdt_entry[6] |= (desc.limit>>16) & 0x0F;
	/*Encode base*/
	gdt_entry[2] = desc.base & 0xFF;
	gdt_entry[3] = (desc.base >> 8) & 0xFF;
	gdt_entry[4] = (desc.base >> 16) & 0xFF;
	gdt_entry[7] = (desc.base >> 24) & 0xFF;

	gdt_entry[5] = desc.access;

	gdt_entry[6] |= (desc.flags << 4);

	// gdt_entry[5] |= 0x9000; // Present flag 
	// gdt_entry[5] |= (desc.access_type & 0x7) << 1;
	// gdt_entry[5] |= (desc.access_DPL & 0x3) << 5;
	// gdt_entry[6] |= (desc.granularity_granularity & 0x1) << 7;
	// gdt_entry[6] |= (desc.granularity_op_size & 0x1) << 5;


	return *((uint64_t*)gdt_entry);
}
