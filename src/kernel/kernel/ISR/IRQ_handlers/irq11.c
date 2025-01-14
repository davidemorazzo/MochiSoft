/* Dicembre 2024 - Davide Morazzo

Handler per Interrupt IRQ11
Indirizzo 0x2B (43)

Device serviti da questa funzione:
 - PCI
*/
#include <stdint.h>
#include "kernel/PIC-8259.h"
#include "kernel/kstdio.h"
#include "dev/PCI/AHCI.h"

void irq11(){

	AHCI_interrupt_routine();
    PIC_sendEOI(11);
	return;
}