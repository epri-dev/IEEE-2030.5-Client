// BSD socket interface

#include <stdlib.h>
#include <string.h>

typedef struct _Address {
  union {
    struct {
      uint16_t family;
      uint16_t port;
    };
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
  };
  socklen_t length;
} Address;

#define ADDR_IPv6 AF_INET6
#define ADDR_IPv4 AF_INET

#define address_eq(x, y) (memcmp (x, y, sizeof (Address)) == 0)
#define address_copy(x, y) memcpy (x, y, sizeof (Address))
#define set_port(addr, p) (addr)->port = htons (p)
#define address_port(addr) ntohs ((addr)->port)
#define address_ipv6(addr) (addr)->in6.sin6_addr.s6_addr
#define address_ipv4(addr) ntohl ((addr)->in.sin_addr.s_addr)
#define address_type(addr) (addr)->family

#ifndef HEADER_ONLY

#include "address.c"

int global_if_index;

void net_select (int index) {
  global_if_index = index;
}

Address *new_address () { return calloc (1, sizeof(Address)); }

//将一个原始的32bit的 ipv4 IP地址和端口，规格化为程序内部统一使用的结构体格式。
Address *ipv4_address (Address *addr, uint32_t ip, int port) {
  struct sockaddr_in *in = &addr->in;
  in->sin_addr.s_addr = htonl (ip);
  in->sin_family = AF_INET; in->sin_port = htons (port);
  addr->length = sizeof (struct sockaddr_in);
  return addr;  
}

int ipv6_scope (const char *addr) {
  int prefix = UNPACK16 (addr) & 0xffc0;
  if (prefix == 0xfe80) return global_if_index;
  return 0;
}

//将一个原始的ipv6地址和端口，规格化为程序内部统一使用的结构体格式。
Address *ipv6_address (Address *addr, const char *ip, int port) {
  struct sockaddr_in6 *in = &addr->in6;
  memcpy (in->sin6_addr.s6_addr, ip, 16);
  in->sin6_family = AF_INET6; in->sin6_port = htons (port);
  in->sin6_flowinfo = 0; in->sin6_scope_id = ipv6_scope (ip);
  addr->length = sizeof (struct sockaddr_in6);
  return addr;
}


/*
1、一般来说，一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。

    SO_REUSEADDR用于对TCP套接字处于TIME_WAIT状态下的socket，才可以重复绑定使用。server程序总是应该在调用bind()之前设置SO_REUSEADDR套接字选项。TCP，先调用close()的一方会进入TIME_WAIT状态

2、SO_REUSEADDR和SO_REUSEPORT


详见 ：https://www.cnblogs.com/mydomain/archive/2011/08/23/2150567.html

*/
void reuse_address (int s) {
  int reuse = 1;
  if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,
		  (const char*)&reuse, sizeof(reuse)) < 0)
    print_error ("reuse_address");
}

/*
SO_REUSEPORT使用场景：linux kernel 3.9 引入了最新的SO_REUSEPORT选项，
使得多进程或者多线程创建多个绑定同一个ip:port的监听socket，
提高服务器的接收链接的并发能力,程序的扩展性更好；此时需要设置SO_REUSEPORT（注意所有进程都要设置才生效）

详见：
https://zhuanlan.zhihu.com/p/35367402

*/
void reuse_port (int s) {
#ifdef SO_REUSEPORT
  int reuse = 1;
  if (setsockopt (s, SOL_SOCKET, SO_REUSEPORT,
		  (const char*)&reuse, sizeof(reuse)) < 0) 
    print_error ("reuse_port");
#endif
}

//创建一个socket
int bsd_socket (int family) {
  int s = socket (family, SOCK_STREAM, IPPROTO_TCP);
  if (s < 0) print_error ("bsd_socket");
  else { reuse_address (s); reuse_port (s); }
  return s;
}

//创建socket并且监听
int bsd_listen (Address *address) {
  int s = bsd_socket (address->family);
  if (bind (s, (struct sockaddr *)address, address->length) < 0)
    print_error ("bsd_listen");
  listen (s, 100);
/*
int listen(int sockfd, int backlog);

listen()函数将sockfd标记为被动打开的套接字，并作为accept的参数用来接收到达的连接请求。

backlog参数用来描述sockfd的等待连接队列能够达到的最大值。
当一个请求到达并且该队列为满时，客户端可能会收到一个表示连接失败的错误，
或者如果底层协议支持重传(比如tcp协议),本次请求会被丢弃不作处理，
在下次重试时期望能连接成功(下次重传的时候队列可能已经腾出空间)。 

https://blog.csdn.net/ywy2090/article/details/52613379

*/

  return s;
}


int bsd_connected (int socket) {
  int error; socklen_t n = sizeof (int);
  getsockopt (socket, SOL_SOCKET, SO_ERROR, &error, &n);
  return error == 0;
}

void multicast_join (int socket, const char *addr, int loop) {
  struct ipv6_mreq group; int value = 0;
  // set the local interface
  //  if_index = interface_index (mdns_socket);
  if (setsockopt (socket, IPPROTO_IPV6, IPV6_MULTICAST_IF,
		  (void *)&global_if_index, sizeof (int)) < 0)
    print_error ("setsockopt IPV6_MULTICAST_IF");    
  // add this process to the multicast group
  memcpy (&group.ipv6mr_multiaddr, addr, sizeof (struct in6_addr));
  group.ipv6mr_interface = global_if_index;
  if (setsockopt (socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
		  (void *)&group, sizeof(group)) < 0)
    print_error ("setsockopt IPV6_ADD_MEMBERSHIP error");
  // recieve our own multicasts?
  setsockopt (socket, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
	      (void *)&loop, sizeof(loop));
  // set the time to live
  value = 255;
  if (setsockopt (socket, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
		  (void *)&value, sizeof(value)) < 0)
    print_error ("setsockopt IPV6_MULTICAST_HOPS error");
}

#endif
