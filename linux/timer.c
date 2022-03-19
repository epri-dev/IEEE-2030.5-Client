// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <sys/timerfd.h>

typedef struct _Timer {
  PollEvent pe;
} Timer;

/*

有关 timespec
如果需要更高的时间精确度，就需要struct timespec 和 struct timeval来处理：

一、struct timespec 定义：

struct timespec {
               time_t tv_sec;                //Seconds
               long   tv_nsec;               // Nanoseconds 
           };
           struct itimerspec {
               struct timespec it_interval;  //Interval for periodic timer
               struct timespec it_value;     //Initial expiration
           };
           
*/
typedef struct _ClockTime {
  struct timespec spec;
} ClockTime;

/*
timerfd是Linux为用户程序提供的一个定时器接口。
这个接口基于文件描述符，通过文件描述符的可读事件进行超时通知，所以能够被用于select/poll的应用场景。

int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
作用：用来启动或关闭有fd指定的定时器
参数：
fd：timerfd，有timerfd_create函数返回
fnew_value:指定新的超时时间，设定new_value.it_value非零则启动定时器，否则关闭定时器，如果new_value.it_interval为0，则定时器只定时一次，即初始那次，否则之后每隔设定时间超时一次
old_value：不为null，则返回定时器这次设置之前的超时时间
flags：1代表设置的是绝对时间；为0代表相对时间。

https://www.cnblogs.com/mickole/p/3261879.html

*/
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


/*
int timerfd_create(int clockid, int flags);
它是用来创建一个定时器描述符timerfd
第一个参数：clockid指定时间类型，有两个值：
CLOCK_REALTIME :Systemwide realtime clock. 系统范围内的实时时钟
CLOCK_MONOTONIC:以固定的速率运行，从不进行调整和复位 ,它不受任何系统time-of-day时钟修改的影响
第二个参数：flags可以是0或者O_CLOEXEC/O_NONBLOCK。
返回值：timerfd（文件描述符）
*/

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
