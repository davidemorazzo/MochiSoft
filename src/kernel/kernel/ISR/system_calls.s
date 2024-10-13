// Ottobre 2024 - Davide Morazzo
// Routine inserita su interrupt vector 0x80, usata per gestire le system calls.
// Descrizione delle system calls implementate: docs/syscalls.md

.globl isr_0x80_wrapper


isr_0x80_wrapper:
	
	