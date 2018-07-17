// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <sys/types.h>
#include <ifaddrs.h>

int interface_index (char *name) {
  int s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP), ret;
  struct ifreq request;
  strcpy(request.ifr_name, name);
  ret = ioctl (s, SIOCGIFINDEX, &request); close (s);
  return ret < 0? ret : request.ifr_ifindex;
}

int interface_mac (uint8_t *mac, char *name) {
  int s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP), ret;
  struct ifreq request;
  strcpy(request.ifr_name, name);
  ret = ioctl (s, SIOCGIFHWADDR, &request); close (s);
  if (ret == 0)
    memcpy (mac, request.ifr_hwaddr.sa_data, 6);
  return ret;
}

#define IPV6 0
#define IPV4 1
#define LINK_LOCAL 2
#define SITE_LOCAL 4

int interface_address (Address *host, char *name, int type) {
  struct ifaddrs *addrs, *a; char *addr;
  int family = type & IPV4? AF_INET : AF_INET6, prefix, found = 0;
  if (getifaddrs (&addrs)) {
    print_error ("getifaddrs"); exit (0);
  }
  for (a = addrs; a != NULL; a = a->ifa_next)
    if (a->ifa_addr && a->ifa_addr->sa_family == family) {
      if (name && streq (name, a->ifa_name)) {
	host->length = type & IPV4? sizeof (struct sockaddr_in)
	  : sizeof (struct sockaddr_in6);
	memcpy (host, a->ifa_addr, host->length); found = 1;
	if (type & IPV4) goto done;
	addr = host->in6.sin6_addr.s6_addr;  
	prefix = UNPACK16 (addr) & 0xffc0;
	if (prefix == 0xfe80 && type & LINK_LOCAL) goto done;
	if (prefix == 0xfd00 && type & SITE_LOCAL) goto done;
      }
    }
 done:
  freeifaddrs (addrs); return found;
}

void print_interfaces (int ipv4) {
  struct ifaddrs *addrs, *a; char buffer[32+10];
  int family = ipv4? AF_INET : AF_INET6;
  if (getifaddrs (&addrs)) {
    print_error ("getifaddrs"); exit (0);
  }
  if (ipv4) printf ("IPv4 interfaces:\n");
  else printf ("IPv6 interfaces:\n");
  for (a = addrs; a != NULL; a = a->ifa_next)
    if (a->ifa_addr && a->ifa_addr->sa_family == family) {
      write_address (buffer, (Address *)a->ifa_addr);
      printf ("  %-10s: %-30s\n", a->ifa_name, buffer);
    }
  printf ("\n"); fflush (stdout); freeifaddrs (addrs);
}
