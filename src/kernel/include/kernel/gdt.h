#ifndef INCLUDE_KERNEL_GDT_H
#define INCLUDE_KERNEL_GDT_H 1

#include <stdint.h>


struct GDT_descriptor {
	/*
	Base => Indirizzo fisico di inizio del segmento
	Limit => Lunchezza del segmento
	*/
	uint32_t limit = 0x0;
	uint32_t base = 0x0;
	uint8_t access = 0x0;
	uint8_t flags = 0x0;

	uint8_t access_DPL = 0x0;				// 2-bit livello di privilegio rispetto alla CPU
	uint8_t access_system = 0x0;			// 0x1: codice o dati; 0x0: TSS (task state segment)
	uint8_t access_type = 0x0;				// 3-bit diritti del processore sul segmento

	uint8_t granularity_granularity = 0x0;	// Moltiplicatore per 4096 => max size 4GiB
	uint8_t granularity_op_size = 0x0;		// 16-bit oppure 32-bit segment
	uint8_t granularity_limit_high = 0x0;	// 4-bit più alti del campo limit
};

struct GDTR{
	uint16_t length;
	void* base; 		/*32-bit address. Pointer aritmetic needed to extract the correct address*/
}__attribute__((packed));

struct GDT_table {
	uint16_t  size = 0;
	uint64_t  base[50]; //TODO: temporary 50 entries
};

uint64_t encode_gdt_descriptor(GDT_descriptor desc);
void lgdt(void* base, uint16_t size);
GDTR sgdt();

//TODO: rewrite della logica GDT in una sola classe

class GDT {

public:
	void load_gdt();
	char check_gdt();
	char add_entry(GDT_descriptor descriptor);
private:
	void lgdt();
	GDTR sgdt();
	uint64_t base[50]; //TODO: temporary 50 entries
	uint16_t size;
};


#endif