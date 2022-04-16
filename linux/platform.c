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

// control/status 这个是在整个应用中用到的一个“事件”对象，实现了各个事件的异步执行。
typedef struct _PollEvent {
  struct _PollEvent *next;
  char type, id;        //enum EventType event的类型，由本应用程序自己定义，见EventType。id仅仅用于TIMER。
  unsigned end : 1;     //end of input 好像是表示事件已经完结，不会再有新的输入。
  unsigned status : 2;  //connection status 网络连接状态。由TcpStatus这个类型给出值。
  unsigned wait : 1;    //置1表示在等待网络中某个事件的发生。置0表示已经完成了或者超时了，已经没有再等待了。
  union {         //这个PollEvent复用于网络和Timer
    int socket;   //socket
    int fd;       //系统 Timer 的 file handle，不是别的file
  };    /* 这里把socket和文件当做同一类事情 */
} PollEvent;

#define MAX_EVENTS 10 //event对象的最大 handle 值（像是一次能够返回的最大的event数量）
#define TCP_ACCEPTOR SYSTEM_EVENT //这个事件看起来像是应用于服务器程序的

int poll_fd;        //整个系统中，所有的事件 ( event ) 都用到了这个handle。
Timer *_tcp_timer;  //一个专门用于处理TCP上的超时时间的handle，仅仅包含了一个专用的Timer。每次需要设定一个定时时间的时候，都要对这个 handle 作处理。


/* 这个函数在这个Demo程序一开始执行的时候就被调用。*/

void platform_init () {

  /*
  1、epoll_create 函数
    函数声明：int epoll_create(int size)
    功能：该函数生成一个epoll专用的文件描述符，其中的参数是指定生成描述符的最大范围；
  */
  
  poll_fd = epoll_create (MAX_EVENTS);
  _tcp_timer = add_timer (TCP_TIMEOUT);

  /* 
  对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 这就是常说的表示连接关闭。
  但第一次对其调用write方法时, 如果发送缓冲没问题, 会返回正确写入(发送)。
  但发送的报文会导致对端发送RST报文, 因为对端的socket已经调用了close, 完全关闭, 既不发送, 也不接收数据。
  所以, 第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出。
  为了避免进程退出, 可以捕获SIGPIPE信号, 或者忽略它, 给它设置SIG_IGN信号处理函数:  
  signal(SIGPIPE, SIG_IGN);  
  这样, 第二次调用write方法时, 会返回-1, 同时errno置为SIGPIPE. 程序便能知道对端已经关闭。

  原文链接：https://blog.csdn.net/u013286409/article/details/45366075 */
  signal (SIGPIPE, SIG_IGN);
}


//将fd设置成非阻塞模式。
void non_block_enable (int fd) {
  fcntl (fd, F_SETFL, O_NONBLOCK);
}

/*判断Event是否已经完成*/
int event_done (void *any) {
  PollEvent *pe = any;
  return pe->end; /*1表示事件已经完结，不会再有新的输入；0表示未完结。*/
}

//新加入一个event对象。handle固定在 poll_fd 上
void event_add (int fd, void *data) {
  struct epoll_event ev;
  non_block_enable (fd);
  
  /*其中events表示感兴趣的事件和被触发的事件
  EPOLLIN：表示对应的文件描述符可以读；
  EPOLLOUT：表示对应的文件描述符可以写；
  EPOLLPRI：表示对应的文件描述符有紧急的数可读；
  EPOLLERR：表示对应的文件描述符发生错误；
  EPOLLHUP：表示对应的文件描述符被挂断；
  EPOLLET：  ET的epoll工作模式；
  */
  ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLET;
  ev.data.ptr = data;
  
  /*
  2、epoll_ctl函数
    函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
    功能：用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。
    @epfd：由epoll_create生成的epoll专用的文件描述符
     @op：要进行的操作，EPOLL_CTL_ADD 注册、EPOLL_CTL_MOD修改、EPOLL_CTL_DEL删除
     @fd：关联的文件描述符；
    @event：指向epoll_event的指针；
   成功：0；失败：-1

  */
  
  epoll_ctl (poll_fd, EPOLL_CTL_ADD, fd, &ev);  //注册epoll，设置成前面ev中的设置值。
  
}

//看起来像是 “活动的TCP连接” 的一个队列
Queue _active = {0};

#include "time.c"
#include "timer.c"
#include "tcp.c"
#include "udp.c"
#include "event.c"
#include "interface.c"
#include "file.c"

#endif
