#include "time.h"

time_t mktime(struct tm *timeptr){
    time_t t = 0;
    t = timeptr->tm_sec + timeptr->tm_min*60 + timeptr->tm_hour*3600;

    // Calcolare quanti giorni da 01/01/1970
    int days_from_epoch = 0; 
    int previousYear = 1900 + timeptr->tm_year - 1; 
    int daysInPreviousYears = ((((previousYear * 365) + (previousYear / 4)) - (previousYear / 100)) + (previousYear / 400));
    int days_to_epoch = ((((1969 * 365) + (1969 / 4)) - (1969 / 100)) + (1969 / 400));

    days_from_epoch = daysInPreviousYears - days_to_epoch + timeptr->tm_yday;
    t += (days_from_epoch-1) * 86400;

    return t;   
}