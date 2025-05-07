/* Davide Morazzo - Settembre 2024 

Implementazione di chiamate di sistema per lo user space
- Link1: https://filippo.io/linux-syscall-table/  */
#include "time.h"
#include <stddef.h>
#include "kernel/proc.h"

void isr_0x80_wrapper();
void *syscall_handler(int syscall_id, void *arg1, void* arg2, 
		void *arg3, void *arg4, void *arg5, void *arg6);

size_t sys_read(int fd, void *buf, size_t count);
size_t sys_write(int fd, const void *buf, size_t count);
time_t sys_time(time_t *tloc);

/* Creazione di un processo figlio di quello attuale 
Argomenti:
- `app_name`: nome del processo
- `current_dir`: cartella corrente

Ritorna:
- `PID_t`: PID del processo creato. 0 se fallito
- `proc_info`: informazioni processo creato */
PID_t sys_create_process(char* app_name, char* current_dir);

