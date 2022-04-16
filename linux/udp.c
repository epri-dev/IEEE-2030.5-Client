// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

typedef struct _UdpPort {
  PollEvent pe;
  Address source;
  int size;
  char buffer[];
} UdpPort;

char *net_receive (UdpPort *p, int *length) {
  p->source.length = sizeof (Address);
  *length = recvfrom (p->pe.socket, p->buffer, p->size, 0,
                      (struct sockaddr *)&p->source, &p->source.length);
  p->pe.end = *length < 0;  //表示不会再有输入？
  // printf ("udp_read %d\n", n); fflush (stdout);
  return *length < 0 ? NULL : p->buffer;
}

/*
定义函数：int sendto(int s, const void * msg, int len, unsigned int flags, const struct sockaddr * to, int tolen);
函数说明：sendto() 用来将数据由指定的socket 传给对方主机.
参数s 为已建好连线的socket, 如果利用UDP协议则不需经过连线操作.
参数msg 指向欲连线的数据内容, 参数flags 一般设0, 详细描述请参考send().
参数to 用来指定欲传送的网络地址, 结构sockaddr 请参考bind(). 参数tolen 为sockaddr 的结果长度.
*/
int net_send (UdpPort *p, char *buffer, int length, Address *addr) {
  // printf ("udp_write %d\n", length); fflush (stdout);
  return sendto (p->pe.socket, buffer, length, 0,
                 (struct sockaddr *)(addr), addr->length);
}


int net_reply (UdpPort *p, char *buffer, int length) {
  return sendto (p->pe.socket, buffer, length, 0,
                 (struct sockaddr *)(&p->source), p->source.length);
}

//创建一个UDP PORT 对象
UdpPort *new_udp_port (int size) {
  UdpPort *p = calloc (1, sizeof (UdpPort) + size);
  p->pe.type = UDP_PORT;  //与Poll Event相互关联。
  p->size = size;
  return p;
}


//打开一个UDP port
void net_open (UdpPort *p, Address *address) {
  if ((p->pe.socket = socket (address->family, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    print_error ("udp_open, socket");
  non_block_enable (p->pe.socket);
  reuse_address (p->pe.socket); //设定UDP port resue。
  event_add (p->pe.socket, p);  //加入到Poll event queue中去，一旦有数据发生，则将产生event poll事件
  if (bind (p->pe.socket, (struct sockaddr *)address, address->length) < 0) //
    print_error ("upd_open, bind");
}


//这个可能是针对IPV6协议的。
void net_join_group (UdpPort *p, const char *addr, int loop) {
  multicast_join (p->pe.socket, addr, loop);
}
