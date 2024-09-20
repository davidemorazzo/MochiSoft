#ifndef INCLUDE_DEV_RTC_H
#define INCLUDE_DEV_RTC_H 1

#include <stdint.h>
#define RTC_DATA 0X71
#define RTC_ADDRESS 0x70

uint8_t rtc_second();
uint8_t rtc_minute();
uint8_t rtc_hour();
uint8_t rtc_update_in_prog();

#endif