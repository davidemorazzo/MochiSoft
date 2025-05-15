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
	list_node_t *proc_node = list_find(__proc_list, (void*)p);
	if (__proc_active == proc_node){
		__proc_active = NULL;
	}
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
	list_remove(__proc_list, proc_node);
	KLOGINFO("Process exit: PID %d, %s, %s, exit code %d", p->pid, p->name, 
		p->current_dir, p->exit_code);

}