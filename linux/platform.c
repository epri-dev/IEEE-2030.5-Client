#include <netinet/tcp.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#define print_error(func) perror (func)

#include "../bsd.c"
#include "../file.c"

#ifndef HEADER_ONLY

// control/status
typedef struct _PollEvent {
  struct _PollEvent *next;
  char type, id;
  unsigned end : 1; // end of input
  unsigned status : 2; // connection status
  unsigned wait : 1;
  union { int socket; int fd; };
} PollEvent;

#define MAX_EVENTS 10
#define TCP_ACCEPTOR SYSTEM_EVENT

int poll_fd;
Timer *_tcp_timer;

void platform_init () {
  poll_fd = epoll_create (MAX_EVENTS);
  _tcp_timer = add_timer (TCP_TIMEOUT);
  signal (SIGPIPE, SIG_IGN);
}

void non_block_enable (int fd) {
  fcntl (fd, F_SETFL, O_NONBLOCK);
}

int event_done (void *any) {
  PollEvent *pe = any; return pe->end;
}

void event_add (int fd, void *data) {
  struct epoll_event ev;
  non_block_enable (fd);
  ev.events = EPOLLIN | EPOLLOUT
    | EPOLLRDHUP | EPOLLHUP | EPOLLET;
  ev.data.ptr = data;
  epoll_ctl (poll_fd, EPOLL_CTL_ADD, fd, &ev);
}

Queue _active = {0};

#include "time.c"
#include "timer.c"
#include "tcp.c"
#include "udp.c"
#include "event.c"
#include "interface.c"
#include "file.c"

#endif
