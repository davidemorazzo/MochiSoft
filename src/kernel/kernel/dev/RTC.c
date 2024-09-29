#include "dev/RTC.h"
#include "kernel/microcode.h"
#include <stdbool.h>
#include "time.h"


uint8_t rtc_update_in_prog()
{   
    outb(RTC_ADDRESS, 0x0A);
    return (inb(RTC_DATA) & 0x80);
}

uint8_t rtc_get_reg(uint8_t reg)
{
    outb(RTC_ADDRESS, reg);
    return inb(RTC_DATA);
}

static uint8_t is_leap_year(int year){
    if ((year < 1) || (year > 9999))
        return false;

    if ((year % 4) != 0)
        return false;

    if ((year % 100) == 0)
        return ((year % 400) == 0);

    return true;
}

void rtc_get_time(struct tm * timeptr){
    int s = (int) rtc_get_reg(RTC_REG_sec);     s = ((s / 16) * 10) + (s & 0xf);
    int m = (int) rtc_get_reg(RTC_REG_min);     m = ((m / 16) * 10) + (m & 0xf);
    int h = (int) rtc_get_reg(RTC_REG_hour);    h = ((h / 16) * 10) + (h & 0xf);
    uint8_t mday = rtc_get_reg(RTC_REG_mday);       mday = ((mday / 16) * 10) + (mday & 0xf);
    uint8_t month = rtc_get_reg(RTC_REG_mon);       month = ((month / 16) * 10) + (month & 0xf);
    uint8_t year = rtc_get_reg(RTC_REG_year);       year = ((year / 16) * 10) + (year & 0xf);
    uint8_t century = rtc_get_reg(RTC_REG_century); century = ((century / 16) * 10) + (century & 0xf);

    timeptr->tm_sec = s;
    timeptr->tm_min = m;
    timeptr->tm_hour = h;
    timeptr->tm_mday = mday;
    timeptr->tm_mon = month -1;
    timeptr->tm_year = year + century*100 - 1900;
    timeptr->tm_wday = rtc_get_reg(RTC_REG_wday);
    
    const int month2days365[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
    const int month2days366[13] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
    if (is_leap_year(timeptr->tm_year)){
        timeptr->tm_yday = mday + month2days366[timeptr->tm_mon];
    }else{
        timeptr->tm_yday = mday + month2days365[timeptr->tm_mon];
    }

    
    timeptr->tm_isdst = 0;
}
