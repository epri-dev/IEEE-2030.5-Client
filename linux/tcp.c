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
  Queue ports;  //看起来像是等待连接的端口？ 一个端口代表一个连接？
} Acceptor;

//构建了一个 “TCP 接收器对象” 当连接建立的时候，将生成一个event。
Acceptor *net_listen (Address *address) {
  Acceptor *a = type_alloc (Acceptor);
  a->pe.type = TCP_ACCEPTOR;
  a->pe.socket = bsd_listen (address);
  event_add (a->pe.socket, a);
  return a;
}

#define event_pending(c) (errno == EAGAIN || errno == EWOULDBLOCK \
			  || errno == EINPROGRESS)


/* 尝试接受一个TCP连接请求，并且返回是否连接上了的结果 */
int accepted (TcpPort *p, Acceptor *a) {
  Address host;
  host.length = sizeof (Address);
  /*

  accept函数由TCP服务器调用，用于从已完成连接队列返回下一个已完成连接。
  如果已完成连接队列为空，那么进程被投入睡眠（假定套接字默为默认的阻塞方式）
  

  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
  
  参数说明：
  sockfd是由socket函数返回的套接字描述符，参数addr和addrlen用来返回已连接的对端进程（客户端）的协议地址。
  如果我们对客户端的协议地址不感兴趣，可以把arrd和addrlen均置为空指针

  返回值：
  返回：若成功则为非负描述符，若出错则为-1

  如果accept成功，那么其返回值是由内核自动生成的一个全新描述符，代表与客户端的TCP连接。
  一个服务器通常仅仅创建一个监听套接字，它在该服务器生命周期内一直存在。
  内核为每个由服务器进程接受的客户端连接创建一个已连接套接字。
  当服务器完成对某个给定的客户端的服务器时，相应的已连接套接字就被关闭。
  
  */
  p->pe.socket = accept (a->pe.socket, (struct sockaddr *)&host,
                         &host.length);

  //如果连接
  if (p->pe.socket == -1) {
    a->pe.end = 1;
    return 0;
  }
  p->pe.status = Connected;
  non_block_enable (p->pe.socket);
  event_add (p->pe.socket, p);
  return 1;
}


//TCP 连接层接受了一个连接 （ 这个函数应该是面向于服务器的 ） 
void net_accept (void *port, Acceptor *a) {
  TcpPort *p = port;
  p->pe.next = NULL;
  if (accepted (p, a)) {
    p->pe.type = TCP_ACCEPT;
    queue_add (&_active, p);
  } else queue_add (&a->ports, p);
}


//如果一个端口连接上了，就从ports队列中移除
void *accept_queued (void *any) {
  Acceptor *a = any;
  TcpPort *p;
  if (p = queue_peek (&a->ports)) { //
    if (accepted (p, a))
      return queue_remove (&a->ports);
  }
  return NULL;
}

//底层TCP连接的状态
int net_status (void *port) {
  TcpPort *p = port;
  return p->pe.status;
}

/* 一个用来等待TCP的queue，如果在等待中的一个连接对象发生了超时，则将通过 tcp_expired 函数退出这个等待队列 */
Queue _tcp_wait = {0};

int _tcp_timeout = 10;

void net_timeout (int seconds) {
  _tcp_timeout = seconds;
}

int compare_timeout (void *a, void *b) {
  TcpPort *p1 = a, *p2 = b;
  int diff = p1->timeout.spec.tv_sec - p2->timeout.spec.tv_sec;
  return diff ? diff : p1->timeout.spec.tv_nsec - p2->timeout.spec.tv_nsec;
}

/*这个函数往往在前面一个步骤中操作了一个有关网络的操作之后，执行该函数，在预期的时刻超时后*/
void set_timeout (void *port) {
  TcpPort *p = port;
  // printf ("set_timeout %p\n", port);
  /*
  函数"clock_gettime"是基于Linux C语言的时间函数,可以用于计算时间，有秒和纳秒两种精度。

  int clock_gettime(clockid_t clk_id, struct timespec *tp);
  
  其中，cld_id类型四种：   
  a、CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变
  b、CLOCK_MONOTONIC,从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
  c、CLOCK_PROCESS_CPUTIME_ID,本进程到当前代码系统CPU花费的时间 
  d、CLOCK_THREAD_CPUTIME_ID,本线程到当前代码系统CPU花费的时间
  */
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
    p->pe.wait = 0; //表示已经不再等待了。
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

//在一个TCP连接超时之后，清理现场。
void *tcp_expired () {
  TcpPort *p, *q;
  if (p = queue_remove (&_tcp_wait)) {
    if (q = queue_peek (&_tcp_wait))
      set_timer_ct (_tcp_timer, &q->timeout);
    else set_timer (_tcp_timer, 0);
    return p;
  }
  return NULL;
}

//关闭网络连接。
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

//底层网络实际连接到服务器。
void net_connect (void *port, Address *server) {
  TcpPort *p = port;
  p->pe.socket = bsd_socket (server->family);
  event_add (p->pe.socket, p);
  p->pe.type = TCP_CONNECT;
  if (connect (p->pe.socket, (struct sockaddr *)server,
               server->length) == 0) {
    p->pe.status = Connected;
    queue_add (&_active, p);
  } else if (event_pending (p)) {       //如果不能立即连接上，那么就等待？
    set_timeout (p);
    p->pe.status = InProgress;
  }
}

int net_read (void *port, char *buffer, int size) {
  TcpPort *p = port;
  int n = -1;
  if (p->pe.status == Connected) {
    n = read (p->pe.socket, buffer, size);
    if (n == 0) net_close (p);
  }
  p->pe.end = n <= 0;
  return n;
}

int net_write (void *port, const char *data, int length) {
  TcpPort *p = port;
  return p->pe.status == Connected ?
         write (p->pe.socket, data, length) : -1;
}

Address *net_remote (Address *addr, void *port) {
  TcpPort *p = port;
  addr->length = sizeof (Address);
  getpeername (p->pe.socket, (struct sockaddr *)addr, &addr->length);
  return addr;
}

Address *net_local (Address *addr, void *port) {
  TcpPort *p = port;
  addr->length = sizeof (Address);
  getsockname (p->pe.socket, (struct sockaddr *)addr, &addr->length);
  return addr;
}

