// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup dnssd_client Service Discovery

    Provides an implementation of DNS-Based Service Discovery
    (<a href="https://tools.ietf.org/html/rfc6763">RFC 6763</a>).
    @{
*/

typedef struct _Service Service;

/** @brief Initialize a DNS-SD query.

    @param packet is a container for the DNS-SD query
    @returns the destination for the first question within the packet
*/
char *dnssd_query (char *packet);

/** @brief Add a question to a DNS-SD query.
    @param dest is the destination of the question within a DNS-SD packet
    @param name is a pointer to a name in the DNS counted label format
    @param type is the type of resource record that pertains to the question
    @param unicast is the value of the QU bit
    @returns the destination of the next question within the query
*/
char *dnssd_question (char *dest, char *name, int type, int unicast);

/** @brief Receive DNS-SD packets from a UdpPort.
    
    Receive and process packets from a UdpPort and return the discovered
    services.
    @param port is a pointer to a UdpPort
    @param Service is a pointer the first discovered Service
 */
Service *dnssd_receive (UdpPort *port);

/** @brief Return the next Service given a pointer to a discovered Service. 
    @param s is a pointer to a Service
    @returns the next discovered Service
*/
Service *service_next (Service *s);

/** @brief Print description of a Service to stdout.
    @param s is a pointer to a Service
*/
void print_service (Service *s);

/** @} */

void print_dns_name (char *name);

#ifndef HEADER_ONLY

#include <stdio.h>
#include <string.h>

typedef struct _Host {
  struct _Host *next;
  char *name;
  Address addr;
} Host;

typedef struct _Service {
  struct _Service *next;
  char *name, *query;
  int port, ttl;
  unsigned txt_found : 1;
  unsigned srv_found : 1;
  unsigned complete : 1;
  unsigned seen : 1;
  Host *host; char *txt;
} Service;

typedef struct _Question {
  struct _Question *next;
  char *name;
} Question;

Host *dns_host = NULL;
Service *dns_service = NULL;
Question *dns_question = NULL;

#define find_host(name) find_by_name (dns_host, name)
#define get_host(name) get_by_name (&dns_host, name, sizeof (Host))
#define find_service(name) find_by_name (dns_service, name)
#define get_service(name) get_by_name (&dns_service, name, sizeof (Service))
#define find_question(name) find_by_name (dns_question, name)
#define get_question(name) get_by_name (&dns_question, name, sizeof (Question))

// DNS resource record access (RFC 1035)
#define rr_type(data) UNPACK16 (data)
#define rr_class(data) UNPACK16 (data+2)
#define rr_ttl(data) UNPACK32 (data+4)
#define rr_rdlength(data) UNPACK16 (data+8)
#define rr_rdata(data) data+10
#define rr_next(data, length) (data+10+length)

// parse and validate DNS resource record
// return pointer to rr fields, name, and length of rdata
char *parse_rr (char *name, int *length, char *data) { char *next;
  if ((data = dns_name (name, data)) && !truncated (data+10)) {
    *length = rr_rdlength (data);
    next = rr_next (data, *length);
    return truncated (next)? NULL : data;
  } return NULL;
}

// skip DNS section
char *skip_section (char *data, int count) {
  char name[256]; int length;
  while (count--) {
    ok (data = parse_rr (name, &length, data));
    data = rr_next (data, length);
  } return data;
}

char *skip_questions (char *data, int count) {
  char name[256];
  while (count--) {
    ok ((data = dns_name (name, data))
	&& !truncated (data+4));
    data += 4;
  } return data;
}

char *ar_section;

// returns pointer to record if found, NULL otherwise
char *dns_find (char *name, int *length, int type) {
  char rr_name[256]; char *data = ar_section;
  while (1) {
    ok (data = parse_rr (rr_name, length, data));
    if (streq (name, rr_name) && type == rr_type (data))
      return data;
    data = rr_next (data, *length);
  }
}

// validate DNS-SD TXT record
int txt_valid (char *data, int length) {
  if (length < 2) return 0; // empty TXT record
  while (length > 0) { int n = *data;
    if (n) { n++; length -= n; data += n; } else return 0;
  } return length == 0;
}

// extract key=value pair from TXT record
char *txt_pair (char *key, char **value, char *data) {
  int n = *data++, m; char *next = data+n; *value = "";
  if (n) { char *eq = strchr (data, '=');
    if (eq && (m = eq - data) < n) {
      n -= m+1; *value = key+m+1;
      memcpy (*value, eq+1, n);
      (*value)[n] = '\0'; n = m;
    } memcpy (key, data, n); key[n] = '\0';
    return next;
  } return NULL;
}

// return value associated with a TXT key=value pair
char *txt_value (char *buffer, char *data, char *key) {
  if (data) { char *value;
    while (data = txt_pair (buffer, &value, data))
      if (streq (buffer, key)) return value;
  } return NULL;
}

// process TXT resource record
char *dns_txt (Service *s, char *data, int length) {
  data += 10;
  if (txt_valid (data, length)) {
    s->txt = malloc (length+1);
    memcpy (s->txt, data, length);
    s->txt[length] = '\0'; 
    s->txt_found = 1; return data;
  } return NULL;
}

// print key=value pairs contained in a TXT record
void print_txt (char *data) {
  char key[128], *value; int prev = 0;
  while (data = txt_pair (key, &value, data)) {
    if (prev) printf (", ");
    if (value) printf ("%s=%s", key, value);
    else printf ("%s", key);
    prev = 1;
  }
}

void print_dns_name (char *name) {
  char buffer[256];
  strcpy (buffer, name);
  name = buffer;
  while (*name != '\0') {
    int n = *name;
    *name = '.';
    name += n+1;
  }
  printf ("%s", buffer+1);
}

void print_host (Address *host) {
  char buffer[32+10];
  write_address (buffer, host); printf ("%s", buffer);
}

void print_service (Service *s) {
  Host *h = s->host;
  printf ("service found:\n");
  printf ("  name: "); print_dns_name (s->name);
  printf ("\n  target: "); print_dns_name (h->name);
  printf ("\n  host: "); print_host (&h->addr);
  printf ("\n  port: %d\n", s->port);
  printf ("  time to live: %d\n", s->ttl);
  if (s->txt) {
    printf ("  txt: "); print_txt (s->txt); printf ("\n"); 
  }
}

// process A resource record (IPv4)
void *host_a (Host *h, char *data, int length) {
  ok (length == 4); data += 10;
  ipv4_address (&h->addr, UNPACK32 (data), 0);
  return data;
}

// process AAAA resource record (IPv6)
void *host_aaaa (Host *h, char *data, int length) {
  ok (length == 16); data += 10;
  ipv6_address (&h->addr, data, 0);
  return data;
}

// process SRV resource record (RFC 2782)
void *dns_srv (Service *s, char *data) {
  char target[256]; int length;
  s->ttl = rr_ttl (data);
  s->port = UNPACK16 (data+10+4);
  ok (dns_name (target, data+10+6));
  if (!s->host) s->host = get_host (target);
  if (data = dns_find (target, &length, AAAA_RECORD))
    host_aaaa (s->host, data, length);
  else if (data = dns_find (target, &length, A_RECORD))
    host_a (s->host, data, length);
  s->srv_found = 1; return data;
}

// process PTR resource record
void dns_ptr (char *name, char *data) {
  Question *q; Service *s; char instance[256]; int length;
  if ((q = find_question (name))
      && (data = dns_name (instance, data+10))) {
    s = get_service (instance); s->query = q->name;
    if (data = dns_find (instance, &length, SRV_RECORD))
      dns_srv (s, data);
    if (data = dns_find (instance, &length, TXT_RECORD))
      dns_txt (s, data, length);
  }
}

// process answer section
void process_answers (char *data, int count) {
  Service *s; Host *h; int length; char name[256];
  while (count--) {
    if (data = parse_rr (name, &length, data)) {
      switch (rr_type (data)) {
      case PTR_RECORD: dns_ptr (name, data); break;
      case SRV_RECORD: s = find_service (name);
	if (s) dns_srv (s, data); break;
      case TXT_RECORD: s = find_service (name);
	if (s) dns_txt (s, data, length); break;
      case A_RECORD: h = find_host (name);
	if (h) host_a (h, data, length); break;
      case AAAA_RECORD: h = find_host (name);
	if (h) host_aaaa (h, data, length); break;
      }
      data = rr_next (data, length);
    } return;
  }
}

// process a DNS-SD packet
void dnssd_packet (char *data, int length) { DnsHeader header;
  dns_start = data; dns_end = data + length; // set the boundry
  // flags should indicate a standard response with no errors
  if ((data = dns_header (&header, data))
      && (header.flags & 0xf80f) == 0x8000
      && (data = skip_questions (data, header.qdcount))) {
    ar_section = skip_section (data, header.ancount+header.nscount);
    process_answers (data, header.ancount);
  }
}

// add a question to a DNS query packet
char *dnssd_question (char *dest, char *name, int type, int unicast) {
  static char *start, *names[14]; static int count;
  if (name) { count++; get_question (name);
    dest = encode_name (dest, name, start, names);
    PACK16 (dest, type);
    PACK16 (dest+2, (unicast << 15) | INTERNET_CLASS);
    PACK16 (start+4, count); // update number of questions
    return dest+4;
  }
  memset (names, 0, sizeof (char *) * 14);
  start = dest; count = 0; return NULL;
}

// initialize a DNS query
char *dnssd_query (char *packet) {
  memset (packet, 0, 12); // header section
  dnssd_question (packet, NULL, 0, 0);
  return packet+12;
}

// followup query to request any missing records
void dnssd_followup (UdpPort *p) {
  char packet[1500]; int followup = 0;
  Service *s = dns_service;
  char *data = dnssd_query (packet);
  while (s) { int complete = 1;
    if (!s->txt_found || !s->srv_found) {
      data = dnssd_question (data, s->name, ANY_RECORD, 0); complete = 0;
    }
    if (!s->host->addr.length) {
      data = dnssd_question (data, s->host->name, ANY_RECORD, 0); complete = 0;
    }
    followup |= !complete; s->complete = complete; s = s->next;
  }
  if (followup) net_send (p, packet, data - packet, &multicast); 
}

// receive and process DNS-SD packets, return list of services
Service *dnssd_receive (UdpPort *port) {
  char *data; int length;
  while (data = net_receive (port, &length))
    dnssd_packet (data, length);
  dnssd_followup (port); return dns_service;
}

// return next completed service not yet seen
Service *service_next (Service *s) {
  while (s) { // process discovered services
    if (s->complete && !s->seen) {
      s->seen = 1; return s;
    } s = s->next;
  } return NULL;
}

#endif
