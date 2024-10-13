#include "kernel/syscall.h"
#include "kernel/kstdio.h"

void syscall_handler(void *retval, int syscall_id, void *arg1, void* arg2, 
		void *arg3, void *arg4, void *arg5, void *arg6)
{	
	switch(syscall_id){
		case 201: // time
			retval = time(arg1);
		default:
			kprint("<WARNING> Invalid syscall_id: %d", &syscall_id);
			break;
	}
}