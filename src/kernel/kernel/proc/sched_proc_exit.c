#include "linked_list/list.h"
#include "kernel/proc.h"
#include "kernel/threads.h"

void __sched_proc_exit(){
	int ret_code;
	__asm__ ("movl %%eax, %0" : "=r"(ret_code));
	proc_t *p = (proc_t*)__proc_active->val;
	p->exit_code = ret_code;
	p->state = PROC_TERMINATED;
	p->thread_info->state = THREAD_TERMINATED;
	asm("int $100"); // call scheduler
}