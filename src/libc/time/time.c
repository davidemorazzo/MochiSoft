/* Ottobre 2024 - Davide Morazzo 
	Implementazione di time.c usando chiamata di sistema numero 0x201
	Da dentro il kernel usare 'sys_time' e non LIBC questa :) 
*/

#include "time.h"
#include <stddef.h>

time_t time (time_t* timer){
	time_t __retval;
	int timer_addr = (int) (void *) timer;
	
	__asm__(
			"mov $201, %%eax\n\t"
			"mov %1, %%ebx\n\t"
			"int $0x80\n\t"
			"mov %%eax, %0"
		: "=m" (__retval)
		: "m" (timer_addr));
	
	if (timer != NULL){
		*timer = __retval;
	}
	
	return __retval;
}
