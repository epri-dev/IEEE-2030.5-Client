// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup se_discover IEEE 2030.5 Service Discovery
    @ingroup dnssd_client

    Provides an implementation of IEEE 2030.5 Service Discovery.
    @{
*/

enum SeServerFlag {
  BILLING_SERVER = (1 << 0 ),      /**< Billing Server */
  DRLC_SERVER = (1 << 1 ),         /**< Demand Response / Load Control Server */
  DER_SERVER = (1 << 2),           /**< Distributed Energy Resources Server */
  FILE_SERVER = (1 << 3),          /**< File Download Server */
  MESSAGE_SERVER = (1 << 4),       /**< Messaging Server */
  PREPAYMENT_SERVER = (1 << 5),    /**< Prepayment Server */
  RESPONSE_SERVER = (1 << 6),      /**< Response Server */
  PRICE_SERVER = (1 << 7),         /**< Pricing Server */
  TIME_SERVER = (1 << 8),          /**< Time Server */
  METER_SERVER = (1 << 9),         /**< Metering Server */
  EDEV_SERVER = (1 << 10),         /**< End Device Server */
  METER_MIRROR_SERVER = (1 << 11), /**< Metering Mirroring Server */
  SELF_DEVICE_SERVER = (1 << 12),  /**< Self Device Server */
  SE_SERVER = (1 << 13)            /**< Any SmartEnergy Server */
};

/** @brief Return a server flag given its subquery name.
    @param name is a subquery name, one of "bill", "dr", "derp", "file",
    "msg", "ppy", "rsps", "tp", "tm", "upt", "edev", "mup", "sdev", or 
    "smartenergy"
    @returns one of the values of SeServerFlag corresponding with the subquery
    name.
*/
int se_subquery (char *name);

/** @brief Send a DNS-SD query for the indicated servers.
    @param server is a an bitwise OR (|) list of SeServerFlag values.
    @param qu is the value of the QU bit, 1 requests that the server send a
    unicast response, 0 requests that a server send a multicast response 
*/
void se_discover (int server, int qu);

void discover_device ();

/** @brief Return the value of the 'path' key if set.
    @param s is a pointer to a Service
    @returns the value of the 'path' key if set, NULL otherwise
*/
char *service_path (Service *s);

/** @brief Return the value of the 'dcap' key if set.
    @param s is a pointer to a Service
    @returns the value of the 'dcap' key if set, NULL otherwise
*/
char *service_dcap (Service *s);

int service_type (Service *s);

/** @brief Connect to a given service, return the connection.
    @param s is a pointer to a Service
    @param secure indicates whether to attempt a secure TLS connection or not,
    this is only possible when the 'https' key is set.
*/
void *service_connect (Service *s, int secure);

Service *service_receive (UdpPort *port);

void discover_init (char *name);

/** @} */

#ifndef HEADER_ONLY

#include <inttypes.h>

const char * const service_name[] = {
  "bill", "dr", "derp", "file", "msg", "ppy", "rsps", "tp", "tm", "upt",
  "edev", "mup", "sdev", "smartenergy"
};

const int service_types[] = {
  SE_CustomerAccountList, SE_DemandResponseProgramList, SE_DERProgramList,
  SE_FileList, SE_MessagingProgramList, SE_PrepaymentList, SE_ResponseSetList,
  SE_TariffProfileList, SE_Time, SE_UsagePointList, SE_EndDeviceList,
  SE_MirrorUsagePointList, SE_SelfDevice, SE_DeviceCapability
};

int se_subquery (char *name) {
  int index = string_index (name, service_name, 14);
  return index < 14? 1 << index : 0;
}

UdpPort *mdns_port = NULL, *mdns_source = NULL, *dns_port = NULL;

void se_discover (int server, int qu) { int i = 0;
  char query[1024], name[64], *packet;
  packet = dnssd_query (query);
  server &= ~(-1 << 14);
  while (server) {
    if (server & 1) { char *n = name;
      if (i < 13) // sub type request
	n += sprintf (name, ".%s._sub", service_name[i]);
      strcpy (n, "._smartenergy._tcp.site");
      write_counted (name);
      packet = dnssd_question (packet, name, PTR_RECORD, qu);
    }
    server >>= 1; i++;
  }
  printf ("se_discover %d\n",
	  net_send (mdns_source, query, packet - query, &multicast));
}

void discover_device () {
  char query[256], name[64], *packet, *n = name;
  packet = dnssd_query (query);
  n += sprintf (name, ".edev-%012" PRIu64 "._sub", device_sfdi);
  strcpy (n, "._smartenergy._tcp.site");
  write_counted (name);
  packet = dnssd_question (packet, name, PTR_RECORD, 1);
  printf ("discover_device %d\n",
	  net_send (mdns_source, query, packet - query, &multicast));
}

Address dns_server;

void se_discover_unicast (char *domain) {
  char query[512], *packet;
  packet = dnssd_query (query);
  strcpy (packet, "._smartenergy._tcp.");
  strcpy (packet+19, domain);
  write_counted (packet); get_question (packet); 
  packet += strlen (packet) + 1;
  PACK16 (packet, DOMAIN_NAME_PTR);
  PACK16 (packet+2, INTERNET_CLASS);
  net_send (dns_port, query, packet+4 - query, &dns_server);
}

char *service_path (Service *s) {
  static char buffer[64];
  return txt_value (buffer, s->txt, "path");
}

char *service_dcap (Service *s) {
  static char buffer[64];
  return txt_value (buffer, s->txt, "dcap");
}

int service_type (Service *s) {
  int n = s->query[0];
  if (strstr (s->query, "_sub")) {
    if (n < 5) {
      char name[5]; strncpy (name, s->query+1, n); name[n] = '\0';
      return service_types[string_index (name, service_name, 13)];
    } return SE_EndDevice;
  } return SE_DeviceCapability;
}

void *service_connect (Service *service, int secure) {
  Address *addr; char buffer[32]; char *https = NULL;
  if (secure) https = txt_value (buffer, service->txt, "https");
  int port = https? (*https == '\0'? 443 : atol (https)) : service->port,
    n_port = htons (port);
  printf ("service_connect: connect on port %d, https = %s, port = %d\n",
	  port, https, service->port); 
  addr = &service->host->addr; addr->port = n_port;
  return se_connect (addr, https != NULL);
}

Service *service_receive (UdpPort *port) {
  if (port == mdns_port || port == mdns_source || port == dns_port)
    return dnssd_receive (port);
  return NULL;
}

void discover_init (char *name) {
  mdns_port = mdns_open (name, 1);
  mdns_source = mdns_client (name);
}

#endif
