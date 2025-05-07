#include "kernel/proc.h"

proc_t* proc_get(PID_t pid){
	for (int i=0; i<MAX_PROC; i++){
		if (__proc_list[i] != NULL && __proc_list[i]->pid == pid)
			return __proc_list[i];
	}

	return NULL;
}