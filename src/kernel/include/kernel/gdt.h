#ifndef INCLUDE_KERNEL_GDT_H
#define INCLUDE_KERNEL_GDT_H 1

#include <stdint.h>
#include "kernel/microcode.h"

typedef struct  {
	/*
	Base => Indirizzo fisico di inizio del segmento
	Limit => Lunchezza del segmento
	*/
	uint32_t limit;
	uint32_t base;
	uint8_t access;
	uint8_t flags;

	uint8_t access_DPL;				// 2-bit livello di privilegio rispetto alla CPU
	uint8_t access_system;			// 0x1: codice o dati; 0x0: TSS (task state segment)
	uint8_t access_type;				// 3-bit diritti del processore sul segmento

	uint8_t granularity_granularity;	// Moltiplicatore per 4096 => max size 4GiB
	uint8_t granularity_op_size;		// 16-bit oppure 32-bit segment
	uint8_t granularity_limit_high;	// 4-bit più alti del campo limit
} GDT_descriptor;



// class GDT {

// public:
// 	GDT();
// 	void load_gdt();
// 	char check_gdt();
// 	char add_entry(GDT_descriptor descriptor);
// private:
// 	uint64_t encode_gdt_descriptor(GDT_descriptor desc);
// 	uint64_t base[50]; //TODO: temporary 50 entries
// 	uint16_t size;		/*Number of bytes in GDT table*/
// };

void gdt_init(uint64_t* global_GDT);
char gdt_check(xDTR GDTR);
void gdt_load(xDTR GDTR);
uint64_t gdt_encode_desc(GDT_descriptor desc);

#endif