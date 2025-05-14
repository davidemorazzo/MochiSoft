#include "kernel/proc.h"

list_t *__proc_list = NULL;
list_node_t *__proc_active = NULL;

PID_t proc_register(proc_t* p){
	PID_t new_pid = 1;
	
	for (int i=0; i<MAX_PROC; i++){
		if (!proc_get(new_pid)){
			break;
		}else{
			new_pid++;
		}
	}

	list_node_t *node = list_node_new((void*)p);
	list_rpush(__proc_list, node);

	return new_pid;
}