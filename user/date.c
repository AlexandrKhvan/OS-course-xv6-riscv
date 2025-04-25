#include "kernel/types.h"
#include "user/user.h"

int
is_leap(int year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

void
print2(int n) {
  if (n < 10)
    printf("0%d", n);
  else
    printf("%d", n);
}

void
print_time(uint64 ns) {
  uint64 secs = ns / 1000000000;
  int nsec    = ns % 1000000000;
  int sec  = secs % 60; secs /= 60;
  int min  = secs % 60; secs /= 60;
  int hour = secs % 24; secs /= 24;

  printf("UTC time: day %d, ", (int)secs);
  print2(hour); printf(":");
  print2(min);  printf(":");
  print2(sec);  printf(".%d\n", nsec);
}

void
print_human_date(uint64 ns) {
  uint secs = ns / 1000000000;

  int sec = secs % 60; secs /= 60;
  int min = secs % 60; secs /= 60;
  int hour = secs % 24; secs /= 24;

  int days = secs;
  int year = 1970;

  while (1) {
    int days_in_year = is_leap(year) ? 366 : 365;
    if (days < days_in_year) break;
    days -= days_in_year;
    year++;
  }

  int month = 0;
  static const int days_in_month[] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
  };

  for (month = 0; month < 12; month++) {
    int mlen = days_in_month[month];
    if (month == 1 && is_leap(year)) mlen = 29;
    if (days < mlen) break;
    days -= mlen;
  }

  int day = days + 1;
  month += 1;

  printf("Human-readable: ");
  print2(day); printf("/");
  print2(month); printf("/");
  printf("%d ", year);
  print2(hour); printf(":");
  print2(min);  printf(":");
  print2(sec);  printf("\n");
}

int
main(int argc, char **argv) {
  uint64 ns = rtc();
  print_time(ns);
  print_human_date(ns);
  exit(0);
}
