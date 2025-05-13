#include "kernel/syscall.h"
#include "kernel/kheap.h"
#include "kernel/memory.h"
#include "kernel/kstdio.h"
#include "string.h"


PID_t sys_create_process(
	char* app_name,
	char* current_dir
){
	proc_t* new_proc = (proc_t*) kmalloc(sizeof(proc_t));
	if (new_proc == NULL) return 0;

	new_proc->state = PROC_INIT;
	new_proc->exit_code = 0;
	new_proc->pid_parent = 0;	// TODO: Trovare il PID parent
	
	new_proc->name = (char*) kmalloc(strlen(app_name)+1);
	if (new_proc->name == NULL) return 0;
	strcpy(app_name, new_proc->name);
	new_proc->current_dir = (char*)kmalloc(strlen(current_dir)+1);
	if (new_proc->current_dir == NULL) return 0;
	strcpy(current_dir, new_proc->current_dir);
	
	/* Memory */
	new_proc->mem_struct = (procmem_t*) kmalloc(sizeof(procmem_t));
	if (new_proc->mem_struct == NULL) return 0;
	// TODO: Fare il layout dello spazio di indirizzamento virtuale del processo
	// new_proc->mem_struct->paging_dir = (PDE*) kmalloc(sizeof(PDE)*1024);
	// Ora uso spazio indirizzamento del kernel
	new_proc->mem_struct->paging_dir = get_page_dir();
	new_proc->mem_struct->phys_pages = NULL;
	
	/* Thread */
	new_proc->thread_info = (thread_info_t*) kmalloc(sizeof(thread_info_t));
	if (new_proc->thread_info == NULL) return 0;
	new_proc->thread_info->thread_id = 1;		// TODO: assegnare TID
	new_proc->thread_info->state = THREAD_INIT;
	// TODO: sostituire con entry-point programma e contesto CPU corretto
	new_proc->thread_info->regs = (registers_t){0};
	new_proc->thread_info->eflags = (eflags_t){0};
	new_proc->thread_info->instruction_ptr = NULL;
	
	new_proc->pid = proc_register(new_proc);
	if (new_proc->pid == 0){
		KLOGERROR("%s: Process '%s' failed to register ", __func__, app_name);
		proc_exit(new_proc);
		return 0;
	}else{
		new_proc->thread_info->pid = new_proc->pid;
		KLOGINFO("New process registerd: PID %d, %s, %s", new_proc->pid, new_proc->name, new_proc->current_dir);
	}
	// TODO: registrare thread in pool dello scheduler
	

	return new_proc->pid;
}