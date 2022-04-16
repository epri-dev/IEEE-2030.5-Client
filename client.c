// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup client Client

    Provides client initialization and service discovery events.
    @{
*/

#define SERVICE_FOUND (EVENT_NEW+1) // EVENT_NEW 值属于 EventType

/** @brief Perform polling on the behalf of a client(在……一方（on behalf of）).
    
    Returns SERVICE_FOUND with a pointer to a Service as the event object when
    a the service discovery returns a discovers a new service.

    回复一个SERVICE_FOUND Event，和一个指向一个Service的指针，当一个服务发现过程发现了一个新的服务。
    
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

//查询dnssd返回的数据和网络层面的数据活动
int client_poll (void **any, int timeout) {
  int event;
  static Service *s = NULL;
top:
  if (s = service_next (s)) { //看起来像是在已经获取到的一堆数据中，检索下一个有用的 Service 。
    *any = s; //Returns SERVICE_FOUND with a pointer to a Service as the event object
    return SERVICE_FOUND; //通过 DNSSD 发现了一个新服务
  }

  /* 我的注解：看起来作为client，仅仅关注下面这三种Event类型 */
  switch (event = event_poll (any, timeout)) {
  case TCP_CONNECT:
    return TCP_PORT;
  case UDP_PORT:  //dnssd返回的数据
    if (s = service_receive (*any)) goto top;
  }
  return event;
}

/*client就是相当于我们的FG设备，这里就是这个测试程序*/
void client_init (char *name, const char *cert) {
  if (cert) {
    tls_init (cert, 0);
    security_init (cert);
  }
  discover_init (name);
}

#endif
