// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup time Time

    Get/set the time based upon the IEEE 2030.5 Time resource.
    @{
*/

/** @brief Set the time and timezone based upon the values of the IEEE 20305.5
    Time object.
    @param tm is a pointer to a SE_Time_t structure
*/
void set_time (SE_Time_t *tm);

/** @brief Get the current (adjusted) time, syncronized with a IEEE 2030.5
    server. 
    @returns the system time with an added offset to match the time of the
    server.
*/
int64_t se_time ();

/** @} */

#include <time.h>

int se_time_offset = 0;

void set_time (SE_Time_t *tm) {
  se_time_offset = tm->currentTime - time (NULL);
  set_timezone (tm->tzOffset, tm->dstOffset,
		tm->dstStartTime, tm->dstEndTime);
}

int64_t se_time () {
  return time (NULL) + se_time_offset;
}
