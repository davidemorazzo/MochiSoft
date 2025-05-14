#include "arch/i386/registers.h"
#include "kernel/proc.h"
#include "kernel/threads.h"

void sched_callback(eflags_t f, void* eip, registers_t r){
	
	for (int p=0; p< MAX_PROC; p++){
		if (__proc_list[p] != NULL && __proc_list[p]->thread_info->state == THREAD_RUNNING){
			/* Stop current thread and start next one */
			__proc_list[p]->thread_info->eflags = f;
			__proc_list[p]->thread_info->regs = r;
			__proc_list[p]->thread_info->instruction_ptr = eip;
			__proc_list[p]->thread_info->state = THREAD_RUNNABLE;
		}
	}
}