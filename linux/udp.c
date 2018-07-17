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
  p->pe.end = *length < 0;
  // printf ("udp_read %d\n", n); fflush (stdout);
  return *length < 0? NULL : p->buffer;
}

int net_send (UdpPort *p, char *buffer, int length, Address *addr) {
  // printf ("udp_write %d\n", length); fflush (stdout);
  return sendto (p->pe.socket, buffer, length, 0,
		 (struct sockaddr *)(addr), addr->length);
}

int net_reply (UdpPort *p, char *buffer, int length) {
  return sendto (p->pe.socket, buffer, length, 0,
 		 (struct sockaddr *)(&p->source), p->source.length);
}

UdpPort *new_udp_port (int size) {
  UdpPort *p = calloc (1, sizeof (UdpPort) + size);
  p->pe.type = UDP_PORT; p->size = size; return p;
}

void net_open (UdpPort *p, Address *address) {
  if ((p->pe.socket = socket (address->family, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    print_error ("udp_open, socket");
  non_block_enable (p->pe.socket);
  reuse_address (p->pe.socket);
  event_add (p->pe.socket, p);
  if (bind (p->pe.socket, (struct sockaddr *)address, address->length) < 0)
    print_error ("upd_open, bind");
}

void net_join_group (UdpPort *p, const char *addr, int loop) {
  multicast_join (p->pe.socket, addr, loop);
}
