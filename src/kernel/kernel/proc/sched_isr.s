/* Maggio 2025 - Davide Morazzo 		*/
/* ------------------------------------	*/
/* Funzione in assembler collegata nel- */
/* la IDT con l'interrupt dedicato all- */
/* o scheduler (direttamente da timer?) */


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

.globl sched_isr

.data
old_esp: .long 0
new_esp: .long 0

.text
sched_isr:
	movl %esp, old_esp 		// Salva OLD ESP
	addl $12, old_esp
	pushal					// arg0: Salva registri+NEW ESP

	call sched_callback

	movl +12(%esp), %eax	// Salva ESP modificato da sched_callback
	movl %eax, new_esp
	popal					// Remove arg0: Carica registri modificati da sched_callback
	movl new_esp, %esp		// Carica SP modificato da sched_callback
	iret
