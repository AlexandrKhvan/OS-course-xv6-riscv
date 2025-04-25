#include "types.h"
#include "memlayout.h"

// Read lower 32 bits of nanoseconds
uint32
rtc_read_low(void) {
  return *(volatile uint32 *)RTC_LOW;
}

// Read upper 32 bits of nanoseconds
uint32
rtc_read_high(void) {
  return *(volatile uint32 *)RTC_HIGH;
}

// Read full 64-bit nanoseconds since epoch
uint64
rtc_read(void) {
  uint32 low  = rtc_read_low();
  uint32 high = rtc_read_high();
  return ((uint64)high << 32) | low;
}