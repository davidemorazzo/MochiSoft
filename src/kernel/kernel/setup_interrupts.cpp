#include "kernel/core.h"
#include "kernel/interrupt.h"

void kernelCore::setup_interrupts () {

	/* Setup handler for exceptions: from ISR 0 to 32 */

	InterruptDescriptorTable.add_entry(
		InterruptDescriptorTable.generate_descriptor(
			general_exception_handler,0x8E, DPL0 | (1 << 3)), 49);


	/*Enable interrupt flag IF*/
	enable_it();
}