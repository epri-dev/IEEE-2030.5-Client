// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup se_connection SeConnection
    @ingroup http_connection

    An SeConnection extends an HttpConnection and provides support for the
    IEEE 2030.5 media types "application/sep+xml" and "application/sep-exi".
    
    这个文件中的函数基本上是跟SE服务器之间的交互过程的API。

    @{
*/

#define SE_CONNECTION EVENT_NEW

#define SE_ERROR (HTTP_RESPONSE+1)
#define SE_INCOMPLETE (HTTP_RESPONSE+2)

#include "debug_log.h"

//回复给服务器的有关Event的状态变化的类型。

enum ResponseType {EventReceived = 1, EventStarted, EventCompleted,
                   EventOptOut, EventOptIn, EventCanceled, EventSuperseded,
                   EventPartialOptOut, EventPartialOptIn, EventCompleteOptOut,
                   EventAcknowledge, EventNoDisplay, EventAbortedServer,
                   EventAbortedProgram,
                   EventInapplicable = 252, EventInvalid, EventExpired
                  };

//
typedef struct _SeConnection SeConnection;

//
enum SeMediaType {SE_EXI, SE_XML, APPLICATION_XML};

//向服务器发送各种HTTP格式数据
#define se_post(conn, data, type, href)		\
  se_send (conn, data, type, href, HTTP_POST)

//将SE类型值为type的data对象，PUT到href指定的位置（在服务器中的文件位置）上去
#define se_put(conn, data, type, href)		\
  se_send (conn, data, type, href, HTTP_PUT)


/*
以method的HTTP方法发送一个Link类型的数据（对象名中包含了Link字样的类型的数据）。
该项数据的SE类型类type。数据存放在data地址指向的空间中。该data属于obj对象。
*/

#define link_send(conn, obj, data, type, method)			\
  if ((data) && se_exists (obj, type##Link))				\
    se_send (conn, data, SE_##type, (obj)->type##Link.href, method)
    
//在conn连接中输出一个obj对象中高的data数据，这个数据的SE基本类型是type。HTTP方法是PUT方法。
#define link_put(conn, obj, data, type)		\
  link_send (conn, obj, data, type, HTTP_PUT)
  
#define link_post(conn, obj, data, type)		\
  link_send (conn, obj, data, type, HTTP_POST)

/** @brief PUT or POST an IEEE 2030.5 object to a server. 

    Use the conn parameter to send the object if the host address matches the
    server specified in the the href parameter, otherwise attempt a new
    connection and send the object on that connection.
    @param conn is a pointer to an SeConnection
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the schema type of the object
    @param href is a URI string, the location to send the object to
    @param method is the HTTP method to use, either HTTP_POST or HTTP_PUT
*/
void *se_send (void *conn, void *obj, int type, char *href, int method);

/** @brief Initialize a Response to an Event.
    初始化一个对于Event的Response。
    
    @param resp is a pointer to an SE_Response_t object or dervied type.
    @param ev is a pointer to an SE_Event_t object
    @param lfdi is the lfdi of an EndDevice.
    @param status updates the event with a ResponseType status是一个 event 的状态值（ResponseType）
*/
void se_response (void *resp, SE_Event_t *ev, char *lfdi, int status);

/** @brief Get the SFDI of the server or client.

    When a secure connection is established, certificates are exchanged between
    the client and server. The SFDI is a hash reduction of a device's
    certificate. Use this function to
*/
uint64_t *se_sfdi (void *conn);

/** @brief Return the negotiated content type. 返回经过协商的内容类型
    @param conn is pointer to an SeConnection
    @param type is pointer to the returned content type
    @returns the negotiated content type as a string and integer
 */
const char *se_content_type (void *conn, int *type);

/** @brief Returns the message body as an IEEE 2030.5 object (if any) . 

    返回 HTTP 消息中的 IEEE 2030.5 对象部分
    
    If @ref se_receive returns an HTTP method or an HTTP response, this
    function returns the HTTP message body as an IEEE 2030.5 object if any
    were present or NULL to indicate the message body was empty. Caller is
    responsible for freeing the object with @ref free_se_object.  如果返回的是NULL，则说明HTTP消息体是空的。
    @param conn is a pointer to an SeConnection
    @param type is a pointer to the returned type of IEEE 2030.5 object  type是指向返回的对象的类型
    @returns an IEEE 2030.5 object present in the HTTP message body (if any),
    otherwise NULL
*/
void *se_body (void *conn, int *type);

/** @brief Free the IEEE 2030.5 object parsed from the HTTP message
    body (if any).
    
    释放掉从HTTP返回的消息中解析出来的对象IEEE 2030.5

    @param conn is a pointer to an SeConnection
*/
void free_se_body (void *conn);

/** @brief Receive an IEEE 2030.5 message. 接收一个 IEEE 2030.5 消息
    @param conn is a pointer to a SeConnection
    @returns the HTTP method on success (see @ref http_receive) 返回HTTP类型如果接收是成功的话。

    SE_INCOMPLETE to indicate an incomplete message SE_INCOMPLETE 表示这个是一个不完整的消息体

    SE_ERROR to indicate an error in the message  SE_ERROR 表示这个函数一个错误。
*/
int se_receive (void *conn);

/** @brief Connect to an IEEE 2030.5 server. 连接到IEEE 2030.5 服务器

    Only one connection is maintained per server address/port, so this function
    first searches a list of existing connection for a matching Address, before
    creating a new connection. 每一个服务器地址+端口维护一个连接，所以这个函数首先搜索一个已经存在的连接表用于匹配地址，在建立一个新的连接之前。
    @param addr is a pointer to Address of the server
    @param secure is 1 for a encrypted TLS connection, 0 for an unencrypted
    TCP connection     secure表示是否启用加密
    @returns a pointer to an SeConnection
*/
void *se_connect (Address *addr, int secure);

/** @brief Connect to an IEEE 2030.5 server using a Uri parameter.

    The same as @ref se_connect, but uses a Uri as a parameter. The URI scheme
    determines whether to attempt a secure connection, "https" for an encrypted
    TLS connection, "http" for an unencrypted TCP connection.
    @param uri is a Uri with a host and a HTTP scheme
    @returns a pointer to an SeConnection
*/
void *se_connect_uri (Uri *uri);

/** @brief Accept a connection request from an IEEE 2030.5 client.
    @param a is a pointer to an Acceptor
    @param secure is 1 for an encrypted TLS connection, 0 for an unencypted
    TCP connection
    @returns a pointer to a SeConnection.
*/
void *se_accept (Acceptor *a, int secure);

void *find_conn (Address *addr);
void *get_conn (Address *addr);

/** @} */

#ifndef HEADER_ONLY

typedef struct _SeConnection {
  HttpConnection http;  //HttpConnection 这个结构放在SeConnection的开头位置，这样不同的两个结构体数据可以共享一个地址了。
  Address host;
  Parser parser;
  int state, media; //这里的state只有 SE_START 和 SE_DATA 两种状态。
  uint64_t sfdi;
  struct _SeConnection *next;
} SeConnection;

const char *const se_ranges[] = {
  "application/sep-exi", "application/sep+xml",
  "application/xml", "application/*", "*/*"
};

int se_range (char *range) {
  return string_index (to_lower (range), se_ranges, 5);
}

const char *se_content_type (void *conn, int *type) {
  SeConnection *s = conn;
  *type = s->media;
  return se_ranges[s->media];
}

// content negotiation
int select_media (char *range) {
  MediaType media;
  int type = SE_XML, quality = 0;
  while (range = media_range (&media, range)) {
    int index = se_range (media.type);
    switch (index) {
    case SE_EXI:
    case SE_XML:
    case APPLICATION_XML:
      if (media.quality > quality) {
        quality = media.quality;
        type = index;
      }
      break;
    }
  }
  return type;
}

//准备好 SE 解析器
int se_parse_init (void *conn) {
  SeConnection *c = conn;
  HttpConnection *h = &c->http;
  MediaType media;
  int type = 3;
  Parser *p = &c->parser;
  if (h->content_type && media_range (&media, h->content_type))
    type = se_range (media.type);
  switch (type) {
  case SE_EXI:
    exi_parse_init (p, &se_schema, NULL, 0);
    break;
  case SE_XML:
  case APPLICATION_XML:
    parse_init (p, &se_schema, NULL);
    break;
  default:
    return 0;
  }
  return 1;
}

uint64_t *se_sfdi (void *conn) {
  SeConnection *s = conn;
  return &s->sfdi;
}

void free_se_body (void *conn) {
  SeConnection *s = conn;
  if (s->parser.obj) {
    free_se_object (s->parser.obj, s->parser.type);
    s->parser.obj = NULL;
  }
}


//取出se回复的数据中的解析后的对象部分，也就是前面HTTP回复中的body部分。
void *se_body (void *conn, int *type) {
  SeConnection *s = conn;
  void *body;
  if (body = s->parser.obj) {
    *type = s->parser.type;
    s->parser.obj = NULL; //取出之后就将原来的指针腾空出来。
  }
  return body;
}

#define SE_START 0
#define SE_DATA 1

// return HTTP method, SE_ERROR, or SE_INCOMPLETE 接收服务器回复的数据并且尝试解析成SE对象。
int se_receive (void *conn) {
  SeConnection *s = conn;
  HttpConnection *h = conn;
  Parser *p = &s->parser;
  char *data;
  int length, type, code, method;
  http_flush (h);
  switch (method = http_receive (h)) {
  case HTTP_NONE:
    PRINT_HTTP(LOG_D("se_receive:HTTP_NONE\n"));
    break;
  case HTTP_ERROR:
    PRINT_HTTP(LOG_E("se_receive:HTTP_ERROR\n"));
    return SE_ERROR;
  default:
    switch (s->state) {
    case SE_START:
      PRINT_HTTP(LOG_I("se_receive:SE_START\n"));
      p->obj = NULL;
      print_http_status (h);
      if (h->media_range)
        s->media = select_media (h->media_range);
      if (h->body) {  //如果在HEADER中指示存在body部分数据的
        if (se_parse_init (s)) s->state++;
        else {
          code = 415;
          goto error;
        }
      } else return method;
    case SE_DATA: //在收到数据之后，将XML文档解析成一个数据对象。
      printf("se_receive:SE_DATA\n");
      while (data = http_data (h, &length)) {
        
        #if 0
        if(length > 0){ //add by lewis
          printf("se_receive:length:%d\n",length);
          printf("se_receive:content:\n%s\n",data);
        }
        #endif
        //add by lewis start
        if(length <= 0) continue; //收到了数据之后，再执行后面的流程。为了方便调试。
        //add by lewis end
        parser_rebuffer (p, data, length);  //重新指定新的待解析的数据的开始地址，待后面解析。
        if( parse_doc (p, &type) ) {
          s->state = SE_START;
          return method;
        } else if (!http_complete (h)) {
          http_rebuffer (h, p->ptr);
        } else {  //如果遇到这个错误，通常是由于服务器返回的消息中，包含了客户端无法识别的SE类型值，此时要修改sep.xsd文档，将新的类型值加入进去。
          printf ("parse error in message body\n");
          print_parse_stack (p);  //解析失败后，将已经解析成功的内容打印出来，以便分析
          code = 400;
          goto error;
        }
      }
      set_timeout (s);
    }
  }
  printf("se_receive:return SE_INCOMPLETE\n");
  return SE_INCOMPLETE;
error:
  if (h->method == HTTP_RESPONSE) http_close (h);
  else http_error (h, code);
  printf("se_receive:SE_ERROR\n");
  return SE_ERROR;
}

SeConnection *connections = NULL;
int se_media = SE_XML;  //当前使用的媒介（se_media）的类型，该值为的类型为 SeMediaType 

void *new_conn (int client) {
  SeConnection *c = type_alloc (SeConnection);
  char *accept = se_media == SE_XML ? "application/sep+xml; level=-S1"
                 : "application/sep-exi; level=-S1, application/sep+xml; level=-S1";
  char *media = se_media == SE_XML ? "application/sep+xml"
                : "application/sep-exi";
  http_init (c, client, accept, media);
  c->media = se_media;
  c->next = connections;  //下一个连接
  connections = c;  // connections 存放首个连接对象，从 connections 出发，就可以遍历到每一个 SeConnection 。
  return c;
}

/*
根据地址信息 （ 地址包含了IP地址和端口信息，这里简称地址 ）找出连接 （ connection ） 对象并返回
*/
void *find_conn (Address *addr) {
  SeConnection *c;
  foreach (c, connections)
  if (http_client (c) && address_eq (&c->host, addr)) return c;
  return NULL;
}


/*根据地址，返回连接对象，如果之前没有存在这个连接对象，则新建一个 */
void *get_conn (Address *addr) {
  SeConnection *c = find_conn (addr);
  return c ? c : new_conn (1);
}

/*
连接到一个服务器中，构成一个 SE_connection
*/
void *se_connect (Address *addr, int secure) {
  SeConnection *c = get_conn (addr);
  address_copy (&c->host, addr);
  if (net_status (c) == Closed)
    conn_connect (c, addr, secure);
  if (conn_session (c)) http_flush (c); //如果当前就是已经连接着的，那么仅仅是flush这个通道。
  return c;
}

/*
根据URI信息来连接到服务器，形成一个 SE_connection
*/
void *se_connect_uri (Uri *uri) {
  int secure = streq (uri->scheme, "https");
  return se_connect (uri->host, secure);  //连接网络
}


/*
这个函数应用于服务端，在本Demo代码中并没有用到。仅仅是在这里定义了，应该仅仅是作为库提供。
*/
void *se_accept (Acceptor *a, int secure) {
  return conn_accept (new_conn (0), a, secure);
}

/*
新建连接并且发送数据。在发送的时刻，构建待发送的数据的内容。
这个函数是上述几个函数的总和。
*/
void *se_send (void *conn, void *data, int type,
               char *href, int method) {
  Uri128 buf;
  Uri *uri = &buf.uri;
  http_parse_uri (&buf, conn, href, 127);
  if (uri->host) conn = se_connect_uri (uri);
  if (conn) {
    Output o;
    char buffer[4096];
    SeConnection *c = conn;
    int length, header;
    header = http_send (conn, buffer, uri->path, method);
    se_output_init (&o, buffer + header, 4096 - header, c->media);
    length = output_doc (&o, data, type); //数据构建成xml格式文本。
    set_content_length (buffer, length);  //
    length += header;
    printf ("se_send:\n");
    http_write (conn, buffer, length);
    print_se_object (data, type);
    printf ("\n");
  }
  return conn;
}


//构建回复给服务器的数据
void se_response (void *resp, SE_Event_t *ev, char *lfdi, int status) {
  SE_Response_t *r = resp;
  r->_flags = SE_createdDateTime_exists | SE_status_exists;
  r->href = NULL;
  r->createdDateTime = time (NULL);
  memcpy (r->endDeviceLFDI, lfdi, 20);
  r->status = status;
  memcpy (r->subject, ev->mRID, 16);
}

#endif
