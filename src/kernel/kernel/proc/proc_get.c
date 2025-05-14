#include "kernel/proc.h"

proc_t* proc_get(PID_t pid){
	list_node_t *node;
	list_iterator_t *it = list_iterator_new(__proc_list, LIST_HEAD);
	while (node=list_iterator_next(it)){
		proc_t *p = (proc_t*) node->val;
		if (p && p->pid == pid) return p;
	}

	list_iterator_destroy(it);
	return NULL;
}