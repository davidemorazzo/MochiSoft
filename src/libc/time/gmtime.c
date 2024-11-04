/* Novembre 2024 - Davide Morazzo

Funzione copiata da: https://github.com/z88dk/z88dk/blob/master/libsrc/time/gmtime.c*/

#include "time.h"

#define SECS_PER_MINUTE ((time_t)60L)
#define SECS_PER_HOUR	((time_t)(60L * SECS_PER_MINUTE))
#define SECS_PER_DAY	((time_t)(24L * SECS_PER_HOUR))
#define SECS_PER_YEAR	((time_t)(365L * SECS_PER_DAY))
#define SECS_PER_LEAP	((time_t)(SECS_PER_YEAR+SECS_PER_DAY))

static int is_leap( int year )
{
	year += 1900; /* Get year, as ordinary humans know it */

	/*
	 *	 The rules for leap years are not
	 *	 as simple as "every fourth year
	 *	 is leap year":
	 */

	if( (unsigned int)year % 100 == 0 ) {
		return (unsigned int)year % 400 == 0;
	}

	return (unsigned int)year % 4 == 0;
}

int __days_per_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};

struct tm * gmtime(const time_t * tp){
    static struct tm tm2;
	time_t t,secs_this_year;

	t = *tp;
	tm2.tm_sec  = 0;
	tm2.tm_min  = 0;
	tm2.tm_hour = 0;
	tm2.tm_mday = 1;
	tm2.tm_mon  = 0;
	tm2.tm_year = 70;
	tm2.tm_wday = ( t / SECS_PER_DAY + 4 ) % 7;	/* 01.01.70 was Thu */
	tm2.tm_isdst = -1;

	/*
	 *	This loop handles dates in 1970 and later
	 */
	while ( t >= ( secs_this_year =
				   is_leap(tm2.tm_year) ?
				   SECS_PER_LEAP :
				   SECS_PER_YEAR ) ) {
		t -= secs_this_year;
		tm2.tm_year++;
	}

	/*
	 *	This loop handles dates before 1970
	 */
	while ( t < 0 )
		t += is_leap(--tm2.tm_year) ? SECS_PER_LEAP : SECS_PER_YEAR;

	tm2.tm_yday = t / SECS_PER_DAY;				/* days since Jan 1 */


	if ( is_leap(tm2.tm_year) )					/* leap year ? */
		__days_per_month[1]++;

	while ( t >= __days_per_month[tm2.tm_mon] * SECS_PER_DAY ) {
		t -= __days_per_month[tm2.tm_mon++] * SECS_PER_DAY;
	}

	if ( is_leap(tm2.tm_year) )					/* leap year ? restore Feb */
		__days_per_month[1]--;

	while ( t >= SECS_PER_DAY ) {
		t -= SECS_PER_DAY;
		tm2.tm_mday++;
	}
	while ( t >= SECS_PER_HOUR ) {
		t -= SECS_PER_HOUR;
		tm2.tm_hour++;
	}
	while ( t >= SECS_PER_MINUTE ) {
		t -= SECS_PER_MINUTE;
		tm2.tm_min++;
	}
	tm2.tm_sec = t;
	return( &tm2);
    
}