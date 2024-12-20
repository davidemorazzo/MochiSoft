/* Dicembre 2024 - Davide Morazzo 

Funzioni per interagire con PCI 
prese da https://wiki.osdev.org/PCI
*/

#include <stdint.h>

#define PCI_ADDR 0xCF8
#define PCI_DATA 0xCFC

void PCIcheckAllBuses(void);

uint16_t PCIconfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

/*
Register	Offset	Bits 31-24	Bits 23-16	Bits 15-8	Bits 7-0
0x0			0x0	    Device ID				Vendor ID
0x1			0x4		Status					Command
0x2			0x8		Class code	Subclass	Prog IF		Revision ID
0x3			0xC		BIST		Header type	Latency Tmr	Cache Line Size
...
*/