/* Maggio 2025 - Davide Morazzo 		*/
/* ------------------------------------	*/
/* Funzioni e definizioni per   		*/
/* gestire i thread. La corrente imple- */
/* mentazione è 1 thread per processo.  */

#ifndef INCLUDE_KERNEL_THREADS_H
#define INCLUDE_KERNEL_THREADS_H

#include "kernel/proc.h"
#include "kernel/memory.h"
#include "arch/i386/registers.h"

enum thread_state{
	THREAD_INIT,
	THREAD_RUNNABLE,
	THREAD_RUNNING,
	THREAD_WAITING,
	THREAD_TERMINATED
};

typedef struct thread_info_s{
	unsigned int thread_id;
	enum thread_state state;
	unsigned int pid;

	registers_t regs;
	eflags_t eflags;
	virt_addr_t instruction_ptr; 

}thread_info_t;

#endif