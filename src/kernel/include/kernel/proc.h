/* Maggio 2025 - Davide Morazzo 		*/
/* ------------------------------------	*/
/* Funzioni e definizioni per   		*/
/* allocare, deallocare i processi in 	*/
/* memoria.								*/

#ifndef INCLUDE_KERNEL_PROC_H
#define INCLUDE_KERNEL_PROC_H

#include "kernel/memory.h"
#include "kernel/threads.h"

typedef unsigned int PID_t;
enum process_state {
	PROC_TERMINATED = 0,
	PROC_INIT = 1,
	PROC_RUNNING = 2
};

typedef struct procmem_s {
	phys_addr_t paging_dir;		// To be used to load CR3 reg. for paging
	pages_range_t* phys_pages;	// Lista di slab di pagine fisiche allocate al processo
								// da rilasciare quando il processo termina
	unsigned int phys_pages_len;
} procmem_t;

/* Struttura che identifica un processo */
typedef struct proc_s {
	char *name;
	enum process_state state;		// Stato del processo (non dei thread)
	PID_t pid;					// Process IDentifier
	PID_t pid_parent;			// PID processo parent
	procmem_t* mem_struct;		// Struttura per la gestione della memoria
	thread_info_t* thread_info;	// Struttura per informazioni sul thread
	char* current_dir;			// Cartella del FS dove è stato aperto il processo.
	short exit_code;			// Codice di uscita del processo.
} proc_t;

#define MAX_PROC 32
extern proc_t* __proc_list[MAX_PROC];	// Internal process list. Limited to 32

/* Registra un nuovo processo `p` nella lista `__proc_list`. Ritorna il PID assegnato */
PID_t proc_register(proc_t* p);
/* Esce dal processo `pid` rilasciando tutta la memoria allocata e rimuovendo i thread 
dall'esecuzione */
void proc_exit(proc_t* p);
/* Ottiene la struttura `proc_t` desiderata se registrata in `__proc_list` */
proc_t* proc_get(PID_t pid);


#endif