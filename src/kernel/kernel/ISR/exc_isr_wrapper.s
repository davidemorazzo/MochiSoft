//  August 2024 - Davide Morazzo
//  Wrapper per gestire correttamente la ISR ed evitare che il
//  compilatore corrompa le funzioni usando solo inline assemble
//  Source: https://wiki.osdev.org/Interrupt_Service_Routines


.globl isr_0x00_wrapper
.globl isr_0x01_wrapper
.align 4

isr_0x00_wrapper:
	pushal
	cld 					// ?
	call isr00 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call					

isr_0x01_wrapper:
	pushal
	cld 					// ?
	call isr01 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call
		