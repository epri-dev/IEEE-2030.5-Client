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
  char type, id;    /*enum EventType*/
  unsigned end : 1; // end of input
  unsigned status : 2; // connection status
  unsigned wait : 1;
  union { int socket; int fd; };    /* 这里把socket和文件当做同一类事情 */
} PollEvent;

#define MAX_EVENTS 10
#define TCP_ACCEPTOR SYSTEM_EVENT

int poll_fd;
Timer *_tcp_timer;

void platform_init () {
/*
1、epoll_create函数
    函数声明：int epoll_create(int size)
   功能：该函数生成一个epoll专用的文件描述符，其中的参数是指定生成描述符的最大范围；

*/    
  poll_fd = epoll_create (MAX_EVENTS);
  _tcp_timer = add_timer (TCP_TIMEOUT);

  /* https://blog.csdn.net/u013286409/article/details/45366075 */
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
  /*其中events表示感兴趣的事件和被触发的事件*/
  ev.events = EPOLLIN | EPOLLOUT
    | EPOLLRDHUP | EPOLLHUP | EPOLLET;
  ev.data.ptr = data;
    /*
    2、epoll_ctl函数
      函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
      功能：用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。
      @epfd：由epoll_create生成的epoll专用的文件描述符
       @op：要进行的操作，EPOLL_CTL_ADD注册、EPOLL_CTL_MOD修改、EPOLL_CTL_DEL删除
       @fd：关联的文件描述符；
      @event：指向epoll_event的指针；
     成功：0；失败：-1

    */  
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
