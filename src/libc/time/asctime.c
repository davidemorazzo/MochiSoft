#include "time.h"
#include "string.h"

char *asctime(const struct tm *timeptr){
    // 000000000011111111112222
	// 012345678901234567890123 
	/* Www Mmm dd hh:mm:ss yyyy */

	char wday_name[][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	static const char mon_name[][4] = {
    	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
    	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  	};
	static char result [26];
	strcpyn(wday_name[timeptr->tm_wday], result, 3); result[3] = ' ';
	strcpyn(mon_name[timeptr->tm_mon], result+4, 3); result[7] = ' ';
	char tmp[6];
	itoa(timeptr->tm_mday, tmp);
	strcpyn(tmp, result+8, 3); result[10] = ' ';
	itoa(timeptr->tm_hour, tmp);
	strcpyn(tmp, result+11, 3); result[13] = ':';
	itoa(timeptr->tm_min, tmp);
	strcpyn(tmp, result+14, 3); result[16] = ':';
	itoa(timeptr->tm_sec, tmp);
	strcpyn(tmp, result+17, 3); result[19] = ' ';
	itoa(timeptr->tm_year+1900, tmp);
	strcpyn(tmp, result+20, 4); 
	result[24] = '\0';

	return result;
}