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

/*
typedef union epoll_data {
    void *ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
 } epoll_data_t;    //保存触发事件的某个文件描述符相关的数据

struct epoll_event {
    __uint32_t events;      // epoll event
    epoll_data_t data;      //User data variable
 };
 
其中events表示感兴趣的事件和被触发的事件，可能的取值为：
EPOLLIN     表示对应的文件描述符可以读；
EPOLLOUT    表示对应的文件描述符可以写；
EPOLLPRI    表示对应的文件描述符有紧急的数可读；

EPOLLERR    表示对应的文件描述符发生错误；
EPOLLHUP    表示对应的文件描述符被挂断；
EPOLLET     ET的epoll工作模式；
*/
void event_add (int fd, void *data) {
  struct epoll_event ev;
  non_block_enable (fd);
  ev.events = EPOLLIN | EPOLLOUT
    | EPOLLRDHUP | EPOLLHUP | EPOLLET;
  ev.data.ptr = data;
  
/*
2、epoll_ctl函数
函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
功能：用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。
@epfd：由epoll_create生成的epoll专用的文件描述符；
@op：要进行的操作，EPOLL_CTL_ADD注册、EPOLL_CTL_MOD修改、EPOLL_CTL_DEL删除；
@fd：关联的文件描述符；
@event：指向epoll_event的指针；
成功：0；失败：-1
*/

  epoll_ctl (poll_fd, EPOLL_CTL_ADD, fd, &ev);  //注册
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
