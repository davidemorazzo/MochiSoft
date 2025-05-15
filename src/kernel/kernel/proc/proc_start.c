#include "kernel/proc.h"
#include "kernel/threads.h"

void proc_start(PID_t pid, void* func){
	proc_t *p = proc_get(pid);
	if (p && p->thread_info->state == THREAD_INIT){
		p->thread_info->instruction_ptr = func;
		p->thread_info->eflags.eflags = 0x286;	//FIXME: capire come inizializzare
		p->thread_info->regs.esp = ((uint32_t)kmalloc(PROC_STACK_SIZE)+PROC_STACK_SIZE); //FIXME: capire dove mettere stack
		uint32_t *proc_esp = (uint32_t*)p->thread_info->regs.esp;
		proc_esp --;
		*proc_esp = (uint32_t) __sched_proc_exit; // funzione chiamata in uscita del thread
		proc_esp--;
		*proc_esp = p->thread_info->eflags.eflags;
		proc_esp--;
		*proc_esp = 0x00000008; //FIXME: capire come inizializzare
		proc_esp --;
		*proc_esp = (uint32_t)p->thread_info->instruction_ptr;
		p->thread_info->regs.esp = (uint32_t)proc_esp;
		p->state = PROC_RUNNING;
		p->thread_info->state = THREAD_RUNNABLE;
	}
}