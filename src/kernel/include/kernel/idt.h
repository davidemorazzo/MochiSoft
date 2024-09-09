
#ifndef INCLUDE_KERNEL_IDT_H
#define INCLUDE_KERNEL_IDT_H 1

#include <stdint.h>

#define TASK_GATE 			0x5
#define INTERRUPT_GATE16 	0x6
#define TRAP_GATE16			0x7
#define INTERRUPT_GATE32	0xE
#define TRAP_GATE32			0xF
#define GD_PRESENT			0x80
#define DPL0				0x0
#define DPL1				0x1
#define DPL2				0x2
#define DPL3				0x3
				

struct InterruptDescriptor32 {
   uint16_t offset_1 = 0;        // offset bits 0..15
   uint16_t selector = 0;        // a code segment selector in GDT or LDT
   uint8_t  zero = 0;            // unused, set to 0
   uint8_t  type_attributes = 0; // gate type, dpl, and p fields
   uint16_t offset_2 = 0;        // offset bits 16..31
}__attribute__((packed));

class IDT {
public:
	IDT();
	void load_idt();
	char add_entry(InterruptDescriptor32 descriptor, uint8_t index);
	char check_idt();
private:
	uint64_t base[256];
	uint16_t size=256*8-1; /*one less than the size of base in bytes*/
};

extern IDT *global_IDT;

#define SET_IT_VEC(desc, func, idx)                               \
    desc.type_attributes = 0x8E;                            \
    desc.offset_1 = ((uint32_t) func) & 0xFFFF;             \
	desc.offset_2 = (((uint32_t) func) >> 16) & 0xFFFF;     \
    desc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/     \
    global_IDT->add_entry(desc, idx)           

#endif