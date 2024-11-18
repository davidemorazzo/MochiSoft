#include "kernel/syscall.h"
#include "dev/RTC.h"
#include <stddef.h>

time_t sys_time(time_t *tloc){
	/* Get time in seconds from 01-01-1970 00:00:00 
	if tloc!=NULL result also copied in the pointer */
	struct tm timeptr;
	rtc_get_time(&timeptr);
	time_t t = mktime(&timeptr);
	if (tloc != NULL){
		*tloc = t;
	}
	return t;
}