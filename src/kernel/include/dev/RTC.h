#ifndef INCLUDE_DEV_RTC_H
#define INCLUDE_DEV_RTC_H 1

#include <stdint.h>
#include "time.h"

#define RTC_DATA 0X71
#define RTC_ADDRESS 0x70

#define RTC_REG_sec         0x00
#define RTC_REG_min         0x02
#define RTC_REG_hour        0x04
#define RTC_REG_wday        0x06
#define RTC_REG_mday        0x07
#define RTC_REG_mon         0x08
#define RTC_REG_year         0x09
#define RTC_REG_century      0x32
#define RTC_REG_A            0xA
#define RTC_REG_B           0xB

uint8_t rtc_update_in_prog();
uint8_t rtc_get_reg(uint8_t reg);

void rtc_get_time(struct tm * timeptr);

#endif