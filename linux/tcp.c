// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <errno.h>
#include <time.h>

typedef struct _TcpPort {
  PollEvent pe;
  struct _TcpPort *prev;
  ClockTime timeout;
} TcpPort;

TcpPort *new_tcp_port () {
  return type_alloc (TcpPort);
}

typedef struct _Acceptor {
  PollEvent pe;
  Queue ports;
} Acceptor;

Acceptor *net_listen (Address *address) {
  Acceptor *a = type_alloc (Acceptor);
  a->pe.type = TCP_ACCEPTOR;
  a->pe.socket = bsd_listen (address);
  event_add (a->pe.socket, a);
  return a;
}

#define event_pending(c) (errno == EAGAIN || errno == EWOULDBLOCK \
			  || errno == EINPROGRESS)

int accepted (TcpPort *p, Acceptor *a) { Address host;
  host.length = sizeof (Address);
  p->pe.socket = accept (a->pe.socket, (struct sockaddr *)&host,
		      &host.length);
  if (p->pe.socket == -1) {
    a->pe.end = 1; return 0;
  }
  p->pe.status = Connected;
  non_block_enable (p->pe.socket); 
  event_add (p->pe.socket, p);
  return 1;
}

void net_accept (void *port, Acceptor *a) {
  TcpPort *p = port;
  p->pe.next = NULL;
  if (accepted (p, a)) {
    p->pe.type = TCP_ACCEPT; 
    queue_add (&_active, p);
  } else queue_add (&a->ports, p);
}

void *accept_queued (void *any) {
  Acceptor *a = any; TcpPort *p;
  if (p = queue_peek (&a->ports)) {
    if (accepted (p, a))
      return queue_remove (&a->ports);
  } return NULL;
}

int net_status (void *port) {
  TcpPort *p = port; return p->pe.status;
}

Queue _tcp_wait = {0};
int _tcp_timeout = 10;

void net_timeout (int seconds) {
  _tcp_timeout = seconds;
}

int compare_timeout (void *a, void *b) {
  TcpPort *p1 = a, *p2 = b;
  int diff = p1->timeout.spec.tv_sec - p2->timeout.spec.tv_sec;
  return diff? diff : p1->timeout.spec.tv_nsec - p2->timeout.spec.tv_nsec;
}

void set_timeout (void *port) {
  TcpPort *p = port;
  // printf ("set_timeout %p\n", port);
  clock_gettime (CLOCK_MONOTONIC, &p->timeout.spec);
  p->timeout.spec.tv_sec += _tcp_timeout;
  p->prev = NULL;
  p->pe.wait = 1;
  _queue_insert (&_tcp_wait, p, &p->prev, compare_timeout);
  p = queue_peek (&_tcp_wait);
  set_timer_ct (_tcp_timer, &p->timeout);
}

// remove connection from either the wait list or old list
void clear_timeout (void *port) {
  TcpPort *p = port;
  // printf ("clear_timeout %p\n", port);
  if (p->pe.wait) {
    p->pe.wait = 0;
    if (p->prev) {
      p->prev->pe.next = p->pe.next;
      p->prev = NULL;
    } else {
      queue_remove (&_tcp_wait);
      if (p = queue_peek (&_tcp_wait))
	set_timer_ct (_tcp_timer, &p->timeout);
      else set_timer (_tcp_timer, 0);
    }
  }
}

void *tcp_expired () { TcpPort *p, *q; 
  if (p = queue_remove (&_tcp_wait)) {
    if (q = queue_peek (&_tcp_wait))
      set_timer_ct (_tcp_timer, &q->timeout);
    else set_timer (_tcp_timer, 0);
    return p;
  } return NULL;
}

void net_close (void *port) {
  PollEvent *pe = port;
  printf ("net_close\n");
  switch (pe->type) {
  case TCP_CONNECT:
  case TCP_PORT:
    pe->status = Closed;
    pe->type = TCP_CLOSED;
    pe->end = 1;
    close (pe->socket);
    clear_timeout (pe);
    queue_add (&_active, pe);
    break;
  case TCP_ACCEPTOR:
    close (pe->socket);
  }
}

void net_connect (void *port, Address *server) {
  TcpPort *p = port;
  p->pe.socket = bsd_socket (server->family);
  event_add (p->pe.socket, p);
  p->pe.type = TCP_CONNECT;
  if (connect (p->pe.socket, (struct sockaddr *)server,
	       server->length) == 0) {
    p->pe.status = Connected;
    queue_add (&_active, p);
  } else if (event_pending (p)) {
    set_timeout (p);
    p->pe.status = InProgress;
  }
}

int net_read (void *port, char *buffer, int size) {
  TcpPort *p = port; int n = -1;
  if (p->pe.status == Connected) {
    n = read (p->pe.socket, buffer, size);
    if (n == 0) net_close (p);
  } p->pe.end = n <= 0;
  return n;
}

int net_write (void *port, const char *data, int length) {
  TcpPort *p = port;
  return p->pe.status == Connected?
    write (p->pe.socket, data, length) : -1;
}

Address *net_remote (Address *addr, void *port) {
  TcpPort *p = port; addr->length = sizeof (Address);
  getpeername (p->pe.socket, (struct sockaddr *)addr, &addr->length);
  return addr;
}

Address *net_local (Address *addr, void *port) {
  TcpPort *p = port; addr->length = sizeof (Address);
  getsockname (p->pe.socket, (struct sockaddr*)addr, &addr->length);
  return addr;
}

