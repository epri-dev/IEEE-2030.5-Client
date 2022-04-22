// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <errno.h>
/*
struct epoll_event

这两天在看项目的数据结构定义及关系，遇到一些关于socket的知识点，还有一些C++的知识点，下面总结下：

1. struct epoll_event

   结构体 epoll_event 被用于注册所感兴趣的事件和回传所发生待处理的事件，定义如下：

    typedef union epoll_data {
        void *ptr;  //携带的用户数据本身的指针。
        int fd;     //
        __uint32_t u32;
        __uint64_t u64;
     } epoll_data_t;  //保存触发事件的某个文件描述符相关的数据

     struct epoll_event {
        __uint32_t events;  //epoll event
        epoll_data_t data;  //User data variable
     };

   其中events表示感兴趣的事件和被触发的事件，可能的取值为：
   EPOLLIN：表示对应的文件描述符可以读；
   EPOLLOUT：表示对应的文件描述符可以写；
   EPOLLPRI：表示对应的文件描述符有紧急的数可读；
   EPOLLERR：表示对应的文件描述符发生错误；
   EPOLLHUP：表示对应的文件描述符被挂断；
   EPOLLET：  将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。

 所涉及到的函数有：

1、epoll_create函数
    函数声明：int epoll_create(int size)
   功能：该函数生成一个epoll专用的文件描述符，其中的参数是指定生成描述符的最大范围；
   
2、epoll_ctl 函数
    函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
    功能：用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。
    @epfd：由 epoll_create 生成的 epoll 专用的文件描述符
     @op：要进行的操作 ， EPOLL_CTL_ADD 注册 、 EPOLL_CTL_MOD 修改 、 EPOLL_CTL_DEL 删除
     @fd：关联的文件描述符；
    @event：指向epoll_event的指针；
   成功：0；失败：-1


3、epoll_wait 函数
   函数声明:int epoll_wait(int epfd,struct epoll_event * events,int maxevents,int timeout)
   功能：该函数用于轮询I/O事件的发生；
    @epfd：由 epoll_create 生成的epoll专用的文件描述符；
    @epoll_event：用于回传待处理事件的数组；
    @maxevents：每次能处理的事件数；应当小于等于前面的数组长度。
    @timeout：等待I/O事件发生的超时值；
   成功：返回发生的事件数；失败：-1


应用举例：

#define SERV_PORT 4466   //服务端口号
const char *LOCAL_ADDR = "127.0.0.1";//绑定服务地址

bool setnonblocking(int sock)//设置socket为非阻塞方式
{
    int opts;
    opts=fcntl(sock,);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        return false;
    }
    opts = opts|;
    if(fcntl(sock,,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        return false;
    }
    return true;
}


int main()
{
　　int i, maxi, listenfd, new_fd, sockfd,epfd,nfds;
　　ssize_t n;
　　char line[MAXLINE];
　　socklen_t clilen;
　　struct epoll_event ev,events[20];//ev用于注册事件,数组用于回传要处理的事件
　　struct sockaddr_in clientaddr, serveraddr;
　　listenfd = socket(AF_INET, SOCK_STREAM, 0);//生成socket文件描述符
　　setnonblocking(listenfd);//把socket设置为非阻塞方式

　　epfd=epoll_create(256);//生成用于处理accept的epoll专用的文件描述符
　　ev.data.fd=listenfd;//设置与要处理的事件相关的文件描述符
　　ev.events=EPOLLIN|EPOLLET;//设置要处理的事件类型。EPOLLIN：表示对应的文件描述符可以读；ET的epoll工作模式；

　　epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);//注册epoll事件

    //设置服务器端地址信息
　　bzero(&serveraddr, sizeof(serveraddr));
　　serveraddr.sin_family = AF_INET;
　　char *local_addr= LOCAL_ADDR;
　　inet_aton(local_addr,&(serveraddr.sin_addr));
　　serveraddr.sin_port=htons(SERV_PORT);
　　bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));   //绑定socket连接
　　listen(listenfd, LISTENQ);//监听

　　maxi = 0;
　　for ( ; ; )
      {
         //epoll_wait：等待epoll事件的发生，并将发生的sokct fd和事件类型放入到 events 数组中；
         //nfds：为发生的事件的个数。
         //注：
　　    nfds=epoll_wait(epfd,events,20,500);

　　    //处理所发生的所有事件
　　    for(i=0;i<nfds;++i)
　　    {
　　        if(events[i].data.fd == listenfd)// 事件发生在listenfd上
　　        {
               //获取发生事件端口信息，存于clientaddr中；
               *new_fd：返回的新的socket描述符，用它来对该事件进行recv/send操作
　　            new_fd = accept(listenfd,(struct sockaddr *)&clientaddr, &clilen);
　　            if(connfd<0)
                   {
　　                perror("connfd<0");
　　                exit(1);
　　            }
　　            setnonblocking(connfd);
　　            char *str = inet_ntoa(clientaddr.sin_addr);
　　            ev.data.fd=connfd;//设置用于读操作的文件描述符
　　            ev.events=EPOLLIN|EPOLLET;//设置用于注测的读操作事件
　　            epoll_ctl(epfd,,,&ev);//注册ev
　　        }
　　        else if(events[i].events & EPOLLIN) //有数据可读
　　        {
　　            if ( (sockfd = events[i].data.fd) < 0)
                       continue;
　　            if ( (n = read(sockfd, line, MAXLINE)) < 0)
                   {
　　                if (errno == ECONNRESET)
                      {
　　                    close(sockfd);
　　                    events[i].data.fd = -1;
　　                }
                      else
　　                    std::cout<<"readline error"<<std::endl;
　　            }
                  else if (n == 0)
                  {
　　                close(sockfd);
　　                events[i].data.fd = -1;
　　           }
　　           ev.data.fd=sockfd;//设置用于写操作的文件描述符
　　           ev.events=EPOLLOUT|EPOLLET;//设置用于注测的写操作事件
　　           epoll_ctl(epfd,,sockfd,&ev);//修改sockfd上要处理的事件为EPOLLOUT
　　      }
　　     else if(events[i].events & EPOLLOUT)
　　     {
　　         sockfd = events[i].data.fd;
　　          write(sockfd, line, n);
　　          ev.data.fd=sockfd;//设置用于读操作的文件描述符
　　          ev.events=EPOLLIN|EPOLLET;//设置用于注测的读操作事件
　　          epoll_ctl(epfd,,sockfd,&ev);//修改sockfd上要处理的事件为EPOLIN
　　     }
　　 }
　}
}
*/


/*主要轮询网络中是否有新的数据，以及Timer定时是否到了。*/
int event_poll (void **any, int timeout) {
  PollEvent *pe;
  TcpPort *p;
  uint64_t value;

  static struct epoll_event events[MAX_EVENTS];
  static int i = 0, n = 0;  //静态变量。n是epoll_wait返回的事件个数。
  
  int event;

  static PollEvent *prev = NULL;  //注意这里是一个静态变量。表示的是上一次获取到的 PollEvent 。
  
  if (prev) { //如果这个事件未完结（event_done == false），则继续添加到_active队列，表示这个事件对象上将继续会有数据变动发生。
    if (!event_done (prev)) queue_add (&_active, prev); //往当前活动的_active的queue中添加prev，prev与本函数中后面的代码的执行有关。
    prev = NULL;
  }
  
poll:
  if (i == n) { //i==n有几种情况：1）两者都是0，则说明上次退出的时候是POLL_TIMEOUT，即没有事件发生；2）当发生了n个事件，且在多次调用本函数后处理全部处理完毕了。
    if (pe = queue_remove (&_active)) {//返回首个元素，如果非零，则执行下面的代码。
      event = pe->type; /* EventType 类型*/
      switch (pe->type) {
      case TCP_ACCEPTOR:  //通过查询TCP_ACCEPTOR关键词发现，整个工程中只有 net_listen 函数应用了 TCP_ACCEPTOR 这个类型，应该是面向服务器端的情况
        goto accept;
      case TCP_ACCEPT:  //只有在net_accept函数中才会返回这个值。所以估计是面向服务器的情况。
      case TCP_CONNECT: //最底层的是在 net_connect 函数中返回，应该是刚刚连接上了一个服务器，然后event type就转换成了TCP_PORT，这样一旦这个端口上有数据活动，则将触发事件。
        pe->type = TCP_PORT;  //对于服务器端或者客户端连接上了服务器，都统一以TCP_PORT论。且下次在调用这个函数的时候，prev 将被加入到 _active 中去。
      case TCP_PORT:    //
      case UDP_PORT:
        prev = pe;  //注意这里都没break，所以统一保存最后一个获得的pe到prev中。
      } *any = pe;  //返回的值也是本次的pe。
      return event;
    }

    //如果 _active 中没有获取到，则执行下面的retry。
    
retry:

    /*
      3、 epoll_wait 函数
         函数声明:int epoll_wait(int epfd,struct epoll_event * events,int maxevents,int timeout)
         功能：该函数用于轮询I/O事件的发生；
         @epfd：由epoll_create生成的epoll专用的文件描述符；
         @epoll_event：用于回传待处理事件的数组；
         @maxevents：每次能处理最大数量的事件数；
         @timeout：等待I/O事件发生的超时值；指定超时值为-1会导致epoll_wait（）无限期阻塞，
         而指定的超时时间等于零导致epoll_wait（）立即返回，即使没有可用事件。
         成功：成功将返回触发I0事件的fd个数。错误将返回-1，并设定errno；时将返回0;
    */
    
    //这个函数应该是阻塞式函数，等待 timeout 长的时间。fd产生事件 / 被信号处理函数打断 / 超时。
    n = epoll_wait(poll_fd, events, MAX_EVENTS, timeout);
    
    i = 0;
    if (n < 0) goto retry;    //perror ("event_poll");  //如果发生了错误，则继续等待。由于epoll_wait是阻塞模式运行所以不造成线程过于繁忙。
    if (n == 0) return POLL_TIMEOUT;  //如果返回是0，则表示超时。即没有事件发生。
  }


  /* 如果发生了事件，则依次处理该事件。每次处理一个事件，直到全部处理完毕。 */
  event = events[i].events;
  *any = pe = events[i].data.ptr; //events[i].data.ptr保存的是之前添加事件的时候的 “用户数据” 。
  i++;  //每次处理一次，就加1，直到i==n，则i又从0开始。 下面是处理事件的具体过程。
  
  // printf ("event_poll %x %p %d\n", event, pe, pe->type);
  switch (pe->type) {
  case TCP_CONNECT: // 面向客户端连接上了服务器这个事件。
    p = *any;
    /* EPOLLOUT：表示对应的文件描述符可以写；*/
    if (event & EPOLLOUT && bsd_connected (pe->socket)) {
      clear_timeout (pe);
      pe->status = Connected;
      //后面的case就是针对这个TCP_PORT进行处理。对于一次HTTP传输事务，可能TCP_PORT的事件会发生多次，直到pe中的end元素的值变成1才停止。
      pe->type = TCP_PORT;  //连接上之后，type的值转换成 TCP_PORT 。
      prev = pe;
      return TCP_CONNECT;
    }
    /*在使用 epoll 时，对端正常断开连接（调用 close()），在服务器端会触发一个 epoll 事件。
    在低于 2.6.17 版本的内核中，这个 epoll 事件一般是 EPOLLIN，即 0x1，代表连接可读。
    不过，2.6.17 版本内核中增加了 EPOLLRDHUP 事件，代表对端断开连接，
    关于添加这个事件的理由可以参见 “[Patch][RFC] epoll and half closed TCP connections”。
    在使用 2.6.17 之后版本内核的服务器系统中，
    对端连接断开触发的 epoll 事件会包含 EPOLLIN | EPOLLRDHUP，即 0x2001。
    有了这个事件，对端断开连接的异常就可以在底层进行处理了，不用再移交到上层。
    https://blog.csdn.net/midion9/article/details/49883063
    */
    if (event & EPOLLRDHUP || event & EPOLLHUP) //EPOLLHUP : 文件被挂断。这个事件是一直监控的，即使没有明确指定
      net_close (*any); //如果对端挂断连接，则此时本地端也要同步挂断连接。在net_close函数中可能加入了一个TCP_PORT事件。
    goto poll;
    
  case TCP_PORT:  //针对前面已经发生了 TCP_CONNECT 的pe，接下去就是判断是否有数据可读了。
    prev = pe;
    clear_timeout (pe);
    /*EPOLLIN：表示对应的文件描述符可以读；*/
    if (event & EPOLLIN)
      return TCP_PORT;
    if (event & EPOLLRDHUP || event & EPOLLHUP) { //对应文件描述符被挂断。
      pe->status = Closed;
      prev = NULL;
      return TCP_CLOSED;
    }
    break;
    
accept: //暂时可以不用关注这个
  case TCP_ACCEPTOR:  //这个状态，只有在net_listen函数中返回，所以应该是面向于服务器程序的。
    if (prev = accept_queued (pe)) {
      queue_add (&_active, pe);   //放到“活动的_active”队列中
      prev->type = TCP_PORT;
      *any = prev;
      return TCP_ACCEPT;
    }
    goto poll;
    
  case TIMER_EVENT: //由 add timer 来构建这个 event。
    if(read (pe->fd, &value, 8) <= 0){  //应该是读取到这个时间的值。
      printf("failed to read file\n");
    } //value的长度是64bit。定时器timer的数据长度是64bit吗？
    if (pe->id == TCP_TIMEOUT)
      *any = tcp_expired ();  //超时了，比如无法连接上，则关闭该连接并且返回这个超时的socket对象？？
    return pe->id;  //除了TCP_TIMEOUT之外的情况，正常返回
  }
  return pe->type;
}
