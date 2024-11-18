/* Davide Morazzo - Settembre 2024 

Implementazione di chiamate di sistema per lo user space */
#include "time.h"

void isr_0x80_wrapper();
void *syscall_handler(int syscall_id, void *arg1, void* arg2, 
		void *arg3, void *arg4, void *arg5, void *arg6);

time_t sys_time(time_t *tloc);

