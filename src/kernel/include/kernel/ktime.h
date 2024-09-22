/* Davide Morazzo - Settembre 2024 
replica la libreria standard ctime */

#ifndef INCLUDE_KERNEL_KTIME_H
#define INCLUDE_KERNEL_KTIME_H 1

struct tm {
    int tm_sec;     // seconds after the minute	    0-60*
    int tm_min;     // minutes after the hour	    0-59
    int tm_hour;    // hours since midnight	        0-23
    int tm_mday;    // day of the month	            1-31
    int tm_mon;     // months since January	        0-11
    int tm_year;    // years since 1900	
    int tm_wday;    // days since Sunday	        0-6
    int tm_yday;    // days since January 1	        0-365
    int tm_isdst;   // Daylight Saving Time flag	
};

int typedef time_t;

time_t ktime (time_t* timer);
time_t mktime(struct tm *timeptr);
struct tm * gmtime(const time_t * timer);

#endif