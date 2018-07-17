// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup client Client

    Provides client initialization and service discovery events.
    @{
*/

#define SERVICE_FOUND (EVENT_NEW+1)

/** @brief Perform polling on the behalf of a client.

    Returns SERVICE_FOUND with a pointer to a Service as the event object when
    a the service discovery returns a discovers a new service.
    @param any receives the event object pointer.
    @param timeout is the polling timeout in milliseconds, or -1 to indicate
    an infitie timeout.
    @returns the @ref EventType and the associated object in the any parameter.
*/
int client_poll (void **any, int timeout);

/** @brief Perform client initialization.
    @param index is the interface index
    @param cert is the path/name of device used to load the device certificate
    and private key pair. 
*/
void client_init (char *name, const char *cert);

/** @} */

#ifndef HEADER_ONLY

int client_poll (void **any, int timeout) {
  int event; static Service *s = NULL;
 top:
  if (s = service_next (s)) {
    *any = s; return SERVICE_FOUND;
  }
  switch (event = event_poll (any, timeout)) {
  case TCP_CONNECT: return TCP_PORT;
  case UDP_PORT:
    if (s = service_receive (*any)) goto top;
  } return event;
}

void client_init (char *name, const char *cert) {
  if (cert) { tls_init (cert, 0); security_init (cert); }
  discover_init (name);
}

#endif
