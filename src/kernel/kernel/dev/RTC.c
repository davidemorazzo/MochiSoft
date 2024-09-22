#include "dev/RTC.h"
#include "kernel/microcode.h"


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