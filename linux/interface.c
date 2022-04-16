// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <sys/types.h>
#include <ifaddrs.h>

int interface_index (char *name) {

  /*
  socket（）函数原型：

  　　int socket(int domain, int type, int protocol);

  　　参数说明：　
  　　
      domain：协议域，又称协议族（family）。
      常用的协议族有AF_INET、 AF_INET6、AF_LOCAL（或称AF_UNIX，Unix域Socket）、AF_ROUTE等。
      协议族决定了socket的地址类型，在通信中必须采用对应的地址，如AF_INET决定了要用ipv4地（32位的）与端口号（16位的）的组合、
      AF_UNIX决定了要用一个绝对路径名作为地 址。
  　　
      type：指定Socket类型。常用的sinocket类型有SOCK_STREAM、 SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET等。
      流式Socket（SOCK_STREAM）是一种面向连接的Socket，针对于面向连接的TCP服务应用。
      数据报式Socket（SOCK_DGRAM）是一种无连接的Socket，对应于无连接的UDP服务应用。　　
  　　
      protocol：指定协议。
      常用协议有IPPROTO_TCP、IPPROTO_UDP、IPPROTO_SCTP、IPPROTO_TIPC等，分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议。
  　　注意：1.type和protocol不可以随意组合，如SOCK_STREAM不可以跟IPPROTO_UDP组合。
      当第三个参数为0时，会自动选择第二个参数类型对应的默认协议。

  */
  
  int s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP), ret; //这里是建立一个UDP协议的socket
  
  /*
  2、用法说明
     ifreq结构定义在/usr/include/net/if.h，用来配置ip地址，激活接口，配置MTU等接口信息的。
     其中包含了一个接口的名字和具体内容——（是个共用体，有可能是IP地址，广播地址，子网掩码，MAC号，MTU或其他内容）。
     ifreq包含在ifconf结构中。而 ifconf结构通常是用来保存所有接口的信息的。
  原文链接：https://blog.csdn.net/sin0803/article/details/36456095
  */
  struct ifreq request;
  strcpy(request.ifr_name, name);
  ret = ioctl (s, SIOCGIFINDEX, &request);
  close (s);
  return ret < 0 ? ret : request.ifr_ifindex; //应该是系统分配下来的网络接口的index。
}


int interface_mac (uint8_t *mac, char *name) {
  int s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP), ret;
  struct ifreq request;
  strcpy(request.ifr_name, name);
  ret = ioctl (s, SIOCGIFHWADDR, &request); /* interface index */
  close (s);
  if (ret == 0)
    memcpy (mac, request.ifr_hwaddr.sa_data, 6);
  return ret;
}

#define IPV6 0
#define IPV4 1
#define LINK_LOCAL 2
#define SITE_LOCAL 4

int interface_address (Address *host, char *name, int type) {
  struct ifaddrs *addrs, *a;
  char *addr;
  int family = type & IPV4 ? AF_INET : AF_INET6, prefix, found = 0;
  if (getifaddrs (&addrs)) {
    print_error ("getifaddrs");
    exit (0);
  }
  for (a = addrs; a != NULL; a = a->ifa_next)
    if (a->ifa_addr && a->ifa_addr->sa_family == family) {
      if (name && streq (name, a->ifa_name)) {
        host->length = type & IPV4 ? sizeof (struct sockaddr_in)
                       : sizeof (struct sockaddr_in6);
        memcpy (host, a->ifa_addr, host->length);
        found = 1;
        if (type & IPV4) goto done;
        addr = host->in6.sin6_addr.s6_addr;
        prefix = UNPACK16 (addr) & 0xffc0;
        if (prefix == 0xfe80 && type & LINK_LOCAL) goto done;
        if (prefix == 0xfd00 && type & SITE_LOCAL) goto done;
      }
    }
done:
  freeifaddrs (addrs);
  return found;
}

//struct ifaddrs
//{
//    struct ifaddrs  *ifa_next;    /* Next item in list */
//    char            *ifa_name;    /* Name of interface */
//    unsigned int     ifa_flags;   /* Flags from SIOCGIFFLAGS */
//    struct sockaddr *ifa_addr;    /* Address of interface */
//    struct sockaddr *ifa_netmask; /* Netmask of interface */
//    union
//    {
//        struct sockaddr *ifu_broadaddr; /* Broadcast address of interface */
//        struct sockaddr *ifu_dstaddr; /* Point-to-point destination address */
//    } ifa_ifu;
//    #define              ifa_broadaddr ifa_ifu.ifu_broadaddr
//    #define              ifa_dstaddr   ifa_ifu.ifu_dstaddr
//    void            *ifa_data;    /* Address-specific data */
//};

void print_interfaces (int ipv4) {
  struct ifaddrs *addrs, *a;
  char buffer[32 + 10];
  int family = ipv4 ? AF_INET : AF_INET6;

  /*
  函数getifaddrs（int getifaddrs (struct ifaddrs **__ifap)）获取本地网络接口信息，将之存储于链表中，
  链表头结点指针存储于__ifap中带回，函数执行成功返回0，失败返回-1，且为errno赋值。
  函数getifaddrs用于获取本机接口信息，比如最典型的获取本机IP地址。
  */
  if (getifaddrs (&addrs)) {
    print_error ("getifaddrs");
    exit (0);
  }
  if (ipv4) printf ("IPv4 interfaces:\n");
  else printf ("IPv6 interfaces:\n");
  for (a = addrs; a != NULL; a = a->ifa_next)
    if (a->ifa_addr && a->ifa_addr->sa_family == family) {
      write_address (buffer, (Address *)a->ifa_addr);
      printf ("  %-10s: %-30s\n", a->ifa_name, buffer);
    }
  printf ("\n");
  fflush (stdout);
  freeifaddrs (addrs);
}
