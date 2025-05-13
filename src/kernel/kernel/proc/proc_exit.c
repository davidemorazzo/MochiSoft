#include "kernel/proc.h"
#include "kernel/kheap.h"
#include "kernel/memory.h"
#include "kernel/kstdio.h"

void proc_exit(proc_t *p){
	if (p==NULL){
		KLOGERROR("%s: NULL argument", __func__);
		return;
	}
	// proc_t *p = proc_get(pid);
	for (int i=0; i<MAX_PROC; i++){
		if (__proc_list[i] == p){
			kfree(p->name);
			kfree(p->current_dir);
			for (unsigned int m=0; m<p->mem_struct->phys_pages_len; m++){
				phys_page_dealloc(
					p->mem_struct->phys_pages[m].len,
					p->mem_struct->phys_pages[m].base
				);
			}
			kfree(p->mem_struct->phys_pages);
			kfree(p->mem_struct);
			kfree(p->thread_info);
			kfree(p);
			__proc_list[i] = NULL;
			KLOGINFO("Process exit: PID %d, %s, %s", p->pid, p->name, p->current_dir);
		}
	}
}