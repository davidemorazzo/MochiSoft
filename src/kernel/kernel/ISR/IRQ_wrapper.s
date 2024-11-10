
.global irq_0x20_wrapper

/* IRQ0 */
irq_0x20_wrapper:
	pushal
	cld 					// ?
	call irq0 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call
