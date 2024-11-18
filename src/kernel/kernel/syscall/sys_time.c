#include "kernel/syscall.h"
#include "kernel/kglobals.h"
#include "dev/RTC.h"
#include <stddef.h>

time_t sys_time(time_t *tloc){
	/* Get time in seconds from 01-01-1970 00:00:00 
	if tloc!=NULL result also copied in the pointer */
	SYS_TIME_RTC;
	if (tloc != NULL){
		*tloc = SYS_TIME_RTC;
	}

	return SYS_TIME_RTC;
}