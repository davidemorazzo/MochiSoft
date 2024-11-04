// Ottobre 2024 - Davide Morazzo
// Routine inserita su interrupt vector 0x80, usata per gestire le system calls.
// Descrizione delle system calls implementate: docs/syscalls.md

.globl isr_0x80_wrapper

retval:
	.quad 0 


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
	// add %esp, 7			// Remove arguments from stack
	pop %ecx
	pop %ecx
	pop %ecx
	pop %ecx
	pop %ecx
	pop %ecx
	pop %ecx
	popfl
	popal
	movl retval, %eax
	iret
	