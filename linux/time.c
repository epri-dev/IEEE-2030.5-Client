// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <time.h>

int dst_time (char *buffer, uint64_t t) {
  struct tm *tm = localtime ((time_t *)&t);
  return sprintf (buffer, "%d", tm->tm_yday);
}

void set_timezone (int tz_offset, int dst_offset,
		   time_t dst_start, time_t dst_end) {
  char tz[64]; int n;
  tz_offset = -(tz_offset/(60*60));
  n = sprintf (tz, "UTC%d", tz_offset);
  dst_offset = tz_offset - (dst_offset/(60*60));
  n += sprintf (tz+n, "UTC%d,", dst_offset);
  n += dst_time (tz+n, dst_start);
  n += sprintf (tz+n, ",");
  dst_time (tz+n, dst_end);
  setenv ("TZ", tz, 1);
}

