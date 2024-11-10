
.global irq_0x20_wrapper
.global irq_0x24_wrapper

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
