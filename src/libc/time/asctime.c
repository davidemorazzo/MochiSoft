#include "time.h"
#include "string.h"
#include "stdio.h"

char *asctime(const struct tm *timeptr){
    // 000000000011111111112222
	// 012345678901234567890123 
	/* Www Mmm dd hh:mm:ss yyyy */

	char wday_name[][4] = {
		"Sun\0", "Mon\0", "Tue\0", "Wed\0", "Thu\0", "Fri\0", "Sat\0"};

	static const char mon_name[][4] = {
    	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
    	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  	};
	static char result [26];
	int year = timeptr->tm_year+1900;
	sprintf(result, "%s %s %d %d:%d:%d %d",
		wday_name[timeptr->tm_wday],
		mon_name[timeptr->tm_mon],
		timeptr->tm_mday,
		timeptr->tm_hour,
		timeptr->tm_min,
		timeptr->tm_sec,
		year);
	
	return result;
}