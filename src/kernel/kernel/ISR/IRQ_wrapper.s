
.global irq_0x20_wrapper
.global irq_0x24_wrapper
.global irq_0x28_wrapper
.global irq_0x2B_wrapper

/* IRQ0 */
irq_0x20_wrapper:
	pushal
	cld 					// ?
	call irq0 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call

/* IRQ4 */
irq_0x24_wrapper:
	pushal
	cld 					// ?
	call irq4 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call

/* IRQ8 */
irq_0x28_wrapper:
	pushal
	cld 					// ?
	call irq8 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call

/* IRQ11 */
irq_0x2B_wrapper:
	pushal
	cld 					// ?
	call irq11 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call