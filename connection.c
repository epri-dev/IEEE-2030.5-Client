// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup connection Connection

    //Connection对象是对TCPport对象的扩展，为TCP或者TLS连接提供了一个接口。
    
    A Connection extends a TcpPort and provides an interface to either a TCP
    or TLS connection. Connections are established by calling either
    @ref conn_connect(这个函数面向客户端) or @ref conn_accept(这个函数面向服务器), the "secure" parameter determines
    whether to use TLS or not. After a connection has been initiated the client
    (or server) calls @ref conn_session to check the status of the connection.
    In the case of TLS, repeat calls to @ref conn_session might be required to
    fully negotiate a TLS session. After a session has been established, use
    @ref conn_read and @ref conn_write to read and write data to the connection
    stream.
    
    对于一个TLS连接来说，可能需要调用多次conn_session来实现。
    当连接构建完毕后，调用conn_read和conn_write来执行数据的读写。

    @{
*/

enum SessionStatus {SESSION_NONE, SESSION_CONNECTED, SESSION_NEW};


//Connection 对象
typedef struct _Connection Connection;


/** @brief Return the session status of a Connection.

    For a TLS connection, perform session negotiation including client/server
    Hello messages and the exchange of certificates.
    @param conn is a pointer to a Connection
    @returns one of SESSION_NONE, SESSION_CONNECTED, SESSION_NEW
*/
int conn_session (void *conn);

/** @brief Read data from a Connection.
    @param conn is a pointer to a Connection
    @param buffer is a container to the data to be read
    @param size is the size of the container in bytes
    @returns the length of the data read or -1 on failure
*/
int conn_read (void *conn, char *buffer, int size);

/** @brief Write data to a Connection.
    @param conn is a pointer to a Connection
    @param data is a pointer to the data
    @param length is the length of the data
    @returns the length of the data written or -1 on failure
 */
int conn_write (void *conn, const char *data, int length);

/** @brief Close a Connection.

    Send a TLS alert for a TLS connection, and send RST for a TCP connection.
    @param conn is a pointer to a Connection
*/
void conn_close (void *conn);

/** @brief Is the Connection a TLS Connection?
    @param conn is a pointer to a Connection
    @returns 1 for a TLS connection, 0 for a TCP connection
*/
int conn_secure (void *conn);

/** @brief Accept a connection.

    Accepts a TCP connection request from the Acceptor. If a secure connection
    is requested, assign the Connection the role of a TLS server and setup the
    connection for reading and writing TLS data.
    @param conn is a pointer to a Connection
    @param a is a pointer to a TCP connection Acceptor
    @param secure is 1 for TLS connection, 0 for a TCP connection
    @returns the value of conn
*/
void *conn_accept (void *conn, Acceptor *a, int secure);

/** @brief Connect to a server.

    Send a TCP connection request to the server at the Address. If a secure
    connection is requested assign the Connection the role of a TLS client
    and setup the connection for reading and writing TLS data.
    @param conn is a pointer to a Connection
    @param server is the Address of a TCP/TLS server
    @param secure is 1 for a TLS connection, 0 for a TCP connection
    @returns the value conn
*/
void *conn_connect (void *conn, Address *server, int secure);

/** @brief Get the TLS session ID.

    The session ID is a 32 byte value that identifies a client/server session.
    @param conn is a pointer to a Connection
    @returns the session ID for a TLS connection, NULL otherwise
 */
const uint8_t *tls_session_id (void *conn);

/** @} */

#ifndef HEADER_ONLY


//TLS_NONE：无TLS连接？ TLS_NEGOTIATE：协商？ TLS_SHUTDOWN：关闭连接。TLS_SESSION连接已经建立。
enum TlsState {TLS_NONE, TLS_NEGOTIATE, TLS_SHUTDOWN, TLS_SESSION};

typedef struct _Connection {
  TcpPort tcp;
  void *tls;
  unsigned tls_state : 2; //TlsState
  int (*session) (void *);  //当前状态
  int (*read) (void *, char *, int);        //从连接中读取数据
  int (*write) (void *, const char *, int); //往连接中写入数据
  void (*close) (void *); //关闭连接
} Connection;


/* TCP层面的基本的读、写、关闭连接等基本的操作函数 */
#define conn_field(conn, name) struct_field (Connection, conn, name)

int conn_session (void *conn) { //判断当前是否连接了。
  return conn_field (conn, session (conn));
}
int conn_read (void *conn, char *buffer, int size) {  //读数据函数
  return conn_field (conn, read (conn, buffer, size));
}
int conn_write (void *conn, const char *data, int length) { //写数据函数
  return conn_field (conn, write (conn, data, length));
}
int conn_secure (void *conn) {  //加密函数
  return conn_field (conn, tls) != NULL;
}
void conn_close (void *conn) {  //关闭连接函数
  conn_field (conn, close (conn));
}

int tcp_session (void *conn) {
  return net_status (conn) == Connected;
}


//实际的函数指针
void tcp_setup (Connection *c) {
  c->tls = NULL;
  c->session = tcp_session;
  c->read = net_read;
  c->write = net_write;
  c->close = net_close;
}

const uint8_t *tls_session_id (void *conn) {
  Connection *c = conn;
  if (c->tls) return ssl_session_id (c->tls);
  return NULL;
}

/*
In the case of TLS, repeat calls to @ref conn_session might be required to
fully negotiate a TLS session.
*/

int tls_session (void *conn) {
  Connection *c = conn;
  if (net_status (c) == Connected) {
    switch (c->tls_state) {
    case TLS_NEGOTIATE:
      if (ssl_handshake (c->tls)) {
        c->tls_state = TLS_SESSION;
        return SESSION_NEW;
      } else if (!ssl_pending ()) {
        c->tls_state = TLS_SHUTDOWN;
        print_ssl_error ("tls_session");
      } else break;
    case TLS_SHUTDOWN:
      if (ssl_close (c->tls)) {
        c->tls_state = TLS_NONE;
        ssl_free (c->tls);
        c->tls = NULL;
        net_close (c);
      }
      break;
    case TLS_SESSION:
      return SESSION_CONNECTED;
    }
  }
  return SESSION_NONE;
}


//TLS 关闭连接
void tls_close (void *conn) {
  Connection *c = conn;
  c->tls_state = TLS_SHUTDOWN;
  tls_session (c);
}

// TLS 读数据
int tls_read (void *conn, char *data, int length) {
  Connection *c = conn;
  int ret = -1;
  if (c->tls_state == TLS_SESSION) {
    ret = ssl_read (c->tls, data, length);
    if (ret <= 0 && !ssl_pending ()) tls_close (conn);  //如果读取失败，并且没有未读取的数据的情况下，关闭连接
  }
  return ret;
}

//TLS 写数据
int tls_write (void *conn, const char *data, int length) {
  Connection *c = conn;
  int ret = -1;
  if (c->tls_state == TLS_SESSION) {
    ret = ssl_write (c->tls, data, length);
    if (ret <= 0 && !ssl_pending ()) tls_close (conn);  //如果写入失败，并且没有未读取的数据的情况下，关闭连接
  }
  return ret;
}

//建立TLS对象的时候直接就构建了连接
void tls_setup (Connection *c) {
  c->tls = ssl_new (c);
  c->session = tls_session;
  c->read = tls_read;
  c->write = tls_write;
  c->close = tls_close;
  c->tls_state = TLS_NEGOTIATE;
}

//接受一个连接 （应该是面向服务器端的程序）
void *conn_accept (void *conn, Acceptor *a, int secure) {
  Connection *c = conn;
  net_accept (conn, a);
  if (secure) {
    tls_setup (conn);
    ssl_accept (c->tls);
  } else tcp_setup (conn);
  return conn;
}


/*建立TCP连接*/
void *conn_connect (void *conn, Address *server, int secure) {
  Connection *c = conn;
  net_connect (conn, server);
  if (secure) {
    tls_setup (conn);
    ssl_connect (c->tls);
  } else tcp_setup (c);
  return conn;
}

#endif
