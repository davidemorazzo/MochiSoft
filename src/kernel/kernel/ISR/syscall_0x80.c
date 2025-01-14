#include "kernel/syscall.h"
#include "kernel/kstdio.h"

void *syscall_handler(int syscall_id, void *arg1, void* arg2, 
		void *arg3, void *arg4, void *arg5, void *arg6)
{	
	void * result;
	switch(syscall_id){
		case 001: // write
			result = (void *)sys_write((int)arg1, arg2, (size_t)arg3);
			break;
		case 201: // time
			result = (void *)sys_time(arg1);
			break;
		default:
			kprint("<WARNING> Invalid syscall_id: %d\n\t", syscall_id);
			result = 0x00;
			break;
	}
	return result;
}