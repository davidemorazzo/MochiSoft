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
// #include "gdt.h"


// uint64_t encode_gdt_descriptor (GDT_descriptor desc){
// 	uint32_t desc_high=0x0, desc_low=0x0;

// 	desc_low = desc_low | (desc.limit & 0xFFFF);
// 	desc_low = desc_low | ((desc.base & 0xFFFF) << 16);
// 	desc_high = desc_high | (desc.base >> 16);
// 	desc_high = desc_high | (desc.limit & 0xF0000);
// 	desc_high = desc_high | (desc.base & 0xF000000);
// 	desc_high = desc_high | (desc.access_DPL & 0x3) << 13;
// 	desc_high = desc_high | (desc.access_type & 0x7) << 9;
// 	desc_high = desc_high | 0x9000; // Present flag 
// 	desc_high = desc_high | (desc.granularity_granularity & 0x1) << 23;
// 	desc_high = desc_high | (desc.granularity_op_size & 0x1) << 21;

// 	return (uint64_t(desc_high) << 32) | desc_low;

// }


uint64_t encode_gdt_descriptor (GDT_descriptor desc){
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

// void load_gdt_table(uint8_t * table){
// 	/*L'istruzione LGDT è usata per indicare al
// 	processore dove inizia la tabella GDT in memoria*/

// 	__asm__ volatile ("lgdt %0" : : )

// 	// __asm__ volatile (
// 	// 	"cli"
// 	// 	"mov ax, [esp+4]"
// 	// 	"mov [gdtr], ax"
// 	// 	"mov eax, [esp+8]"
// 	// 	"mov [gdtr+2], eax"
// 	// 	"lgdt [gdtr]"
// 	// 	);
// }


void lgdt(void* base, uint16_t size)
{
    // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) GDTR = { size, base };

    asm ( "lgdt %0" : : "m"(GDTR) );  // let the compiler choose an addressing mode
}


GDTR sgdt()
{
	GDTR this_gdtr;
	// struct {
    //     uint16_t length;
    //     void*    base;
    // } __attribute__((packed)) GDTR = { size, base };
	
	asm ("sgdt %0" : "=m"(this_gdtr) : );
	return this_gdtr;
}