// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup se_connection SeConnection
    @ingroup http_connection

    An SeConnection extends an HttpConnection and provides support for the
    IEEE 2030.5 media types "application/sep+xml" and "application/sep-exi".
    @{
*/

#define SE_CONNECTION EVENT_NEW

#define SE_ERROR (HTTP_RESPONSE+1)
#define SE_INCOMPLETE (HTTP_RESPONSE+2)


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
#define se_put(conn, data, type, href)		\
  se_send (conn, data, type, href, HTTP_PUT)
#define link_send(conn, obj, data, type, method)			\
  if ((data) && se_exists (obj, type##Link))				\
    se_send (conn, data, SE_##type, (obj)->type##Link.href, method)
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
    responsible for freeing the object with @ref free_se_object.
    @param conn is a pointer to an SeConnection
    @param type is a pointer to the returned type of IEEE 2030.5 object  type是指向返回的对象的类型
    @returns an IEEE 2030.5 object present in the HTTP message body (if any),
    otherwise NULL
*/
void *se_body (void *conn, int *type);

/** @brief Free the IEEE 2030.5 object parsed from the HTTP message 释放掉从HTTP返回的消息中解析出来的对象IEEE 2030.5中
    body (if any).
    @param conn is a pointer to an SeConnection
*/
void free_se_body (void *conn);

/** @brief Receive an IEEE 2030.5 message. 接收一个 IEEE 2030.5 消息
    @param conn is a pointer to a SeConnection
    @returns the HTTP method on success (see @ref http_receive)

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
  HttpConnection http;
  Address host;
  Parser parser;
  int state, media;
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


//取出se回复的数据中的body部分。
void *se_body (void *conn, int *type) {
  SeConnection *s = conn;
  void *body;
  if (body = s->parser.obj) {
    *type = s->parser.type;
    s->parser.obj = NULL;
  }
  return body;
}

#define SE_START 0
#define SE_DATA 1

// return HTTP method, SE_ERROR, or SE_INCOMPLETE
int se_receive (void *conn) {
  SeConnection *s = conn;
  HttpConnection *h = conn;
  Parser *p = &s->parser;
  char *data;
  int length, type, code, method;
  http_flush (h);
  switch (method = http_receive (h)) {
  case HTTP_NONE:
    printf("se_receive:HTTP_NONE\n");
    break;
  case HTTP_ERROR:
    printf("se_receive:HTTP_ERROR\n");
    return SE_ERROR;
  default:
    switch (s->state) {
    case SE_START:
      printf("se_receive:SE_START\n");
      p->obj = NULL;
      print_http_status (h);
      if (h->media_range)
        s->media = select_media (h->media_range);
      if (h->body) {
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
          printf("se_receive:return method,s->state=SE_START\n");
          return method;
        } else if (!http_complete (h)) {
          http_rebuffer (h, p->ptr);
        } else {  //如果遇到这个错误，通常是由于服务器返回的消息中，包含了客户端无法识别的SE名称。
          printf ("parse error in message body\n");
          print_parse_stack (p);
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
  connections = c;  //connections存放首个连接对象
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
  if (conn_session (c)) http_flush (c);
  return c;
}

/*
根据URI信息来连接到服务器，形成一个 SE_connection
*/
void *se_connect_uri (Uri *uri) {
  int secure = streq (uri->scheme, "https");
  return se_connect (uri->host, secure);
}


/*
这个函数应用于服务端，在本Demo代码中并没有用到。仅仅是在这里定义了，应该仅仅是作为库提供。
*/
void *se_accept (Acceptor *a, int secure) {
  return conn_accept (new_conn (0), a, secure);
}

/*
新建连接并且发送数据。这个函数是上述几个函数的总和。
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
    length = output_doc (&o, data, type);
    set_content_length (buffer, length);
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
