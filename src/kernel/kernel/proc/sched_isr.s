/* Maggio 2025 - Davide Morazzo 		*/
/* ------------------------------------	*/
/* Funzione in assembler collegata nel- */
/* la IDT con l'interrupt dedicato all- */
/* o scheduler (direttamente da timer?) */

.globl sched_isr

/* Composizione dello stack creato dalla CPU i386

  31          0
╠═══════╦═══════╣
║▒▒▒▒▒▒▒║▒▒▒▒▒▒▒║	 OLD
╠═══════╬═══════╣	SS:ESP
║▒▒▒▒▒▒▒║▒▒▒▒▒▒▒║	  │
╠═══════╩═══════╣◄────┘
║   OLD EFLAGS	║
╠═══════╦═══════╣
║▒▒▒▒▒▒▒║OLD CS ║	 NEW
╠═══════╩═══════╣	SS:ESP
║    OLD EIP	║	  │
╠═══════════════╣◄────┘

*/
.data
old_esp: .long 0
old_eflags: .long 0
old_cs: .short 0
old_eip: .long 0

.text
sched_isr:
	push %eax
	movl +4(%esp), %eax
	movl %eax, old_eip
	movl +8(%esp), %eax
	mov %ax, old_cs
	movl +12(%esp), %eax
	movl %eax, old_eflags
	movl %esp, old_esp
	addl $4, old_esp
	addl $12, old_esp
	pop %eax
	pushal
	cld
	movl old_esp, %eax
	movl %eax, +12(%esp);
	push old_eip
	push old_eflags
	call sched_callback
	popfl
	pop old_eip
	popal
	iret
