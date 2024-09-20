#include "dev/RTC.h"
#include "kernel/microcode.h"

uint8_t rtc_second()
{
    outb(RTC_ADDRESS, 0x00);
    return inb(RTC_DATA);
}

uint8_t rtc_minute()
{
    outb(RTC_ADDRESS, 0x02);
    return inb(RTC_DATA);
}

uint8_t rtc_hour()
{
    outb(RTC_ADDRESS, 0x04);
    return inb(RTC_DATA);
}

uint8_t rtc_update_in_prog()
{   
    outb(RTC_ADDRESS, 0x0A);
    return (inb(RTC_DATA) & 0x80);
}
