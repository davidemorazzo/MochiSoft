#include "kernel/proc.h"

proc_t* __proc_list[MAX_PROC] = {0};

PID_t proc_register(proc_t* p){
	PID_t new_pid = 1;
	for (int i=MAX_PROC-1; i>=0; i--){
		if (__proc_list[i]!=NULL && __proc_list[i]->state != PROC_TERMINATED){
			if (new_pid == 0) new_pid = __proc_list[i]->pid+1;
			if (new_pid == __proc_list[i]->pid) {
				new_pid ++;
				i = MAX_PROC;
			}
		}
	}

	for (int i=0; i<MAX_PROC; i++){
		if (__proc_list[i] == NULL){
			__proc_list[i] = p;
			break;
		}
	}
	return new_pid;
}