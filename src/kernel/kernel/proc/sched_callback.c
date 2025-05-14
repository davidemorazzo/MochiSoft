#include "arch/i386/registers.h"
#include "kernel/proc.h"
#include "kernel/threads.h"
#include "kernel/kstdio.h"

void sched_callback(eflags_t f, void* eip, registers_t r){
	if (__proc_active != NULL){	
		if (((proc_t*)__proc_active->val)->thread_info->state != THREAD_RUNNING){
			KLOGERROR("%s: __proc_active not in running state");
		}
		/* Salvataggio stato thread */
		proc_t *p = (proc_t*)__proc_active->val;
		p->thread_info->eflags = f;
		p->thread_info->regs = r;
		p->thread_info->instruction_ptr = eip;
		p->thread_info->state = THREAD_RUNNABLE;
	}

	/* Trovo nuovo thread da eseguire */

	list_iterator_t *it = list_iterator_new_from_node(__proc_active, LIST_HEAD);
	__proc_active = NULL;
	while (__proc_active == NULL){
		list_node_t *new_node;
		if (!(new_node = list_iterator_next(it))){
			/* Lista finita, riinizio */
			list_iterator_destroy(it);
			it = list_iterator_new(__proc_list, LIST_HEAD);
			new_node = list_iterator_next(it);
		}
		
		if (((proc_t*)new_node->val)->thread_info->state == THREAD_RUNNABLE){
			__proc_active = new_node;
		}
	}
	list_iterator_destroy(it);

	/* Imposto lo stato della CPU per riprendere l'esecuzione */

	proc_t *new_thread = (proc_t*) __proc_active->val;
	new_thread->thread_info->state = THREAD_RUNNING;
	f = new_thread->thread_info->eflags;
	eip = new_thread->thread_info->instruction_ptr;
	r = new_thread->thread_info->regs;


}