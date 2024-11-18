// Ottobre 2024 - Davide Morazzo
// Routine inserita su interrupt vector 0x80, usata per gestire le system calls.
// Descrizione delle system calls implementate: docs/syscalls.md

.globl isr_0x80_wrapper

.section .data
.align 4
retval:
	.long 0  


isr_0x80_wrapper:
	pushal					// Preserve general purpose registers
	pushfl					// Preserve EFLAGS
	cld						// ?
	push %ebp				// Push arguments for 'syscall_handler'
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax
	call syscall_handler	// Call system call handler
	movl %eax, retval			// Syscall return value
	add $28, %esp			// Remove arguments from stack
	popfl
	popal
	movl retval, %eax
	iret
	