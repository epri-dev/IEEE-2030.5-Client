// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#ifndef HEADER_ONLY

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MDNS_PORT 5353

#define DNS_PACKET_SIZE 512
char *dns_start, *dns_end;
Address multicast;

#define truncated(data) ((data) > dns_end)

UdpPort *mdns_open (char *name, int loop) {
  UdpPort *mdns = new_udp_port (1500); 
  Address addr; char zero[16] = {0};
  const char mcast[16] = "\xff\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\xfb";
  net_select (interface_index (name));
  ipv6_address (&multicast, mcast, MDNS_PORT);
  net_open (mdns, ipv6_address (&addr, zero, MDNS_PORT));
  net_join_group (mdns, mcast, loop);
  return mdns;
}

UdpPort *mdns_client (char *name) {
  UdpPort *client = new_udp_port (1500); Address addr;
  if (interface_address (&addr, name, SITE_LOCAL)) {
    set_port (&addr, MDNS_PORT);
    net_open (client, &addr);
  } else net_open (client, &multicast);
  return client;
}

// parse a dns name - counted strings with pointers
// simple pointer loops are detected, complex ones are limited by name length
char *dns_name (char *name, char *data) {
  char *ptr = NULL;
  int n, name_length = 0, prev_length = -1;
  while (!truncated (data+1)) {
    if (*data == '\0') {
      *name = '\0';
      if (ptr) return ptr+2;
      return data+1;
    }
    switch (*data & 0xc0) {
    case 0: // label
      n = *data + 1;
      if ((name_length + n) > 254) return NULL;
      if (truncated (data+n)) return NULL;
      memcpy (name, data, n);
      name += n; name_length += n; data += n;
      break;
    case 0xc0: // pointer
      if (truncated (data+2)) return NULL;
      if (ptr == NULL) ptr = data;
      data = dns_start + (UNPACK16 (data) & 0x3fff);
      if (truncated (data)) return NULL; // exceeds packet length
      if (prev_length == name_length) return NULL; // pointer loop with no data
      prev_length = name_length;
      break;
    default: printf ("dns_name: unknown type in name\n");
      return NULL;
    }
  }
  return NULL;
}

// encode dns name using PTR compression
char *encode_name (char *buffer, char *name, char *start, char **names) {
  while (*name) { int i, n = name[0]+1;
    for (i = 0; names[i] != NULL; i++) {
      char *sub = names[i];
      if (strncmp (name, sub, n) == 0) {
	int ptr = 0xc000 | (sub - start);
	PACK16 (buffer, ptr);
	return buffer + 2;
      }
    }
    names[i] = buffer;
    memcpy (buffer, name, n);
    name += n; buffer += n;
  }
  *buffer = '\0'; return buffer + 1;
}

typedef struct {
  unsigned short id, flags, qdcount, ancount, nscount, arcount;
} DnsHeader;

char *dns_header (DnsHeader *header, char *data) {
  if (truncated (data+12)) return NULL; // no header
  header->id = UNPACK16 (data);
  header->flags = UNPACK16 (data+2);
  header->qdcount = UNPACK16 (data+4);
  header->ancount = UNPACK16 (data+6);
  header->nscount = UNPACK16 (data+8);
  header->arcount = UNPACK16 (data+10);
  return data+12;
}

char *write_counted (char *buffer) {
  strtok (buffer+1, ".");
  do {
    int n = strlen (buffer+1);
    *buffer = n; buffer += n+1;
  } while (strtok (NULL, ".") != NULL);
  return buffer+1;
}

#define DOMAIN_NAME_PTR 12
#define INTERNET_CLASS 1

#define PTR_RECORD 12
#define TXT_RECORD 16
#define AAAA_RECORD 28 
#define SRV_RECORD 33
#define A_RECORD 1
#define ANY_RECORD 255

#endif
