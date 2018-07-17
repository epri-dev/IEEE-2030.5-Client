// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <errno.h>

int event_poll (void **any, int timeout) {
  PollEvent *pe; TcpPort *p; uint64_t value;
  static struct epoll_event events[MAX_EVENTS];
  static int i = 0, n = 0; int event;
  static PollEvent *prev = NULL;
  if (prev) {
    if (!event_done (prev)) queue_add (&_active, prev);
    prev = NULL;
  }
 poll:
  if (i == n) {
    if (pe = queue_remove (&_active)) {
      event = pe->type;
      switch (pe->type) {
      case TCP_ACCEPTOR: goto accept;
      case TCP_ACCEPT: case TCP_CONNECT:
	pe->type = TCP_PORT;
      case TCP_PORT: case UDP_PORT:
	prev = pe;
      } *any = pe; return event;
    }
  retry:
    n = epoll_wait (poll_fd, events, MAX_EVENTS, timeout); i = 0;
    if (n < 0) goto retry; // perror ("event_poll");
    if (n == 0) return POLL_TIMEOUT;
  }
  event = events[i].events; *any = pe = events[i].data.ptr; i++;
  // printf ("event_poll %x %p %d\n", event, pe, pe->type);
  switch (pe->type) {
  case TCP_CONNECT: p = *any;
    if (event & EPOLLOUT && bsd_connected (pe->socket)) {
      clear_timeout (pe);
      pe->status = Connected; pe->type = TCP_PORT;
      prev = pe; return TCP_CONNECT;
    }
    if (event & EPOLLRDHUP || event & EPOLLHUP)
      net_close (*any);
    goto poll;
  case TCP_PORT: prev = pe;
    clear_timeout (pe);
    if (event & EPOLLIN)
      return TCP_PORT;
    if (event & EPOLLRDHUP || event & EPOLLHUP) {
      pe->status = Closed; prev = NULL;
      return TCP_CLOSED;
    } break;
  accept:
  case TCP_ACCEPTOR:
    if (prev = accept_queued (pe)) {
      queue_add (&_active, pe);
      prev->type = TCP_PORT;
      *any = prev;
      return TCP_ACCEPT;
    } goto poll;
  case TIMER_EVENT:
    read (pe->fd, &value, 8);
    if (pe->id == TCP_TIMEOUT)
      *any = tcp_expired ();
    return pe->id;
  }
  return pe->type;
}
