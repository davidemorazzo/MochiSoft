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
	// strcpyn(wday_name[timeptr->tm_wday], result, 3); result[3] = ' ';
	// strcpyn(mon_name[timeptr->tm_mon], result+4, 3); result[7] = ' ';
	// char tmp[6];
	// itoa(timeptr->tm_mday, tmp);
	// strcpyn(tmp, result+8, 3); result[10] = ' ';
	// itoa(timeptr->tm_hour, tmp);
	// strcpyn(tmp, result+11, 3); result[13] = ':';
	// itoa(timeptr->tm_min, tmp);
	// strcpyn(tmp, result+14, 3); result[16] = ':';
	// itoa(timeptr->tm_sec, tmp);
	// strcpyn(tmp, result+17, 3); result[19] = ' ';
	// itoa(timeptr->tm_year+1900, tmp);
	// strcpyn(tmp, result+20, 4); 
	// result[24] = '\0';
	int year = timeptr->tm_year+1900;
	sprintf(result, "%s %s %d %d:%d:%d %d",
		wday_name[timeptr->tm_wday],
		mon_name[timeptr->tm_mon],
		&timeptr->tm_mday,
		&timeptr->tm_hour,
		&timeptr->tm_min,
		&timeptr->tm_sec,
		&year);
	
	return result;
}