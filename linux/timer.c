// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <sys/timerfd.h>

typedef struct _Timer {
  PollEvent pe;
} Timer;

typedef struct _ClockTime {
  struct timespec spec;
} ClockTime;

void set_timer (Timer *timer, int timeout) {
  struct itimerspec it = {0};
  it.it_interval.tv_sec = timeout;
  it.it_value.tv_sec = timeout;
  //printf ("set_timer %x %d %d\n", timer, time (NULL), timeout);
  timerfd_settime (timer->pe.fd, 0, &it, NULL);
}

void set_timer_ms (Timer *timer, int ms) {
  struct itimerspec it = {0};
  int seconds = ms / 1000;
  ms -= seconds * 1000;
  it.it_interval.tv_sec = seconds;
  it.it_interval.tv_nsec = ms * 1000000;
  it.it_value.tv_sec = seconds;
  it.it_value.tv_nsec = ms * 1000000;
  //printf ("set_timer %x %d %d\n", timer, time (NULL), timeout);
  timerfd_settime (timer->pe.fd, 0, &it, NULL);
}

void set_timer_ct (Timer *timer, ClockTime *ct) {
  struct timespec t; struct itimerspec it = {0}; long diff;
  clock_gettime (CLOCK_MONOTONIC, &t);
  //  printf ("set_timer_ct %d %d\n", t.tv_sec, ct->spec.tv_sec);
  t.tv_sec = ct->spec.tv_sec - t.tv_sec;
  diff = ct->spec.tv_nsec - t.tv_nsec;
  if (diff >= 0) t.tv_nsec = diff;
  else {
    t.tv_sec--;
    t.tv_nsec = diff + 1000000000;
  }
  memcpy (&it.it_value, &t, sizeof (struct timespec));
  timerfd_settime (timer->pe.fd, 0, &it, NULL);
}

Timer *add_timer (int id) {
  Timer *timer = malloc (sizeof (Timer));
  timer->pe.type = TIMER_EVENT; timer->pe.id = id;
  timer->pe.fd = timerfd_create (CLOCK_MONOTONIC, 0);
  timer->pe.end = 1; event_add (timer->pe.fd, timer);
  return timer;
}

Timer *new_timer (int id, int timeout) {
  Timer *timer = add_timer (id);
  set_timer (timer, timeout); return timer;
}
