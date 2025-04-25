#ifndef _RTC_H_
#define _RTC_H_

#include "types.h"

uint64 rtc_read(void);
uint32 rtc_read_low(void);
uint32 rtc_read_high(void);

#endif // _RTC_H_
