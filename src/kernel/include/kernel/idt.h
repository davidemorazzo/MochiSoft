
#ifndef INCLUDE_KERNEL_IDT_H
#define INCLUDE_KERNEL_IDT_H 1

#include <stdint.h>
#include "kernel/microcode.h"

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


typedef struct {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  zero;            // unused, set to 0
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
}__attribute__((packed)) InterruptDescriptor32;

InterruptDescriptor32 InterruptDescriptor32Init = {
    .offset_1=0,
    .selector=0,
    .zero=0,
    .type_attributes=0,
    .offset_2=0
} ;

#define SET_IT_VEC(desc, func, idx)                               \
    desc = InterruptDescriptor32Init;                       \
    desc.type_attributes = 0x8E;                            \
    desc.offset_1 = ((uint32_t) func) & 0xFFFF;             \
	desc.offset_2 = (((uint32_t) func) >> 16) & 0xFFFF;     \
    desc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/     \
    global_IDT[idx] = (*(uint64_t*) &desc); //TODO: fix

void load_idt(xDTR idt);
char check_idt(xDTR idt);
extern uint64_t global_IDT[255];


#endif