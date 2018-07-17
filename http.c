// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup http_connection HttpConnection
    @ingroup connection

    An HttpConnection extends a Connection and provides a client or server
    interface to an HTTP message stream
    (<a href="https://tools.ietf.org/html/rfc7230">RFC 7230</a>,
    <a href="https://tools.ietf.org/html/rfc7231">RFC 7231</a>).

    In HTTP client/server roles are fixed through the lifetime of the
    connection and are determined by whether a host requested a connection
    (a client) or accepted a connection request (a server). Clients submit
    requests (GET, PUT, POST, DELETE) and process responses. Servers process
    client requests and respond with a status code and possibly a message body.

    @{
*/

typedef struct _HttpConnection HttpConnection;

enum HttpMethod {HTTP_GET, HTTP_PUT, HTTP_POST, HTTP_DELETE, HTTP_HEAD,
		 HTTP_UNKNOWN, HTTP_RESPONSE, HTTP_NONE, HTTP_ERROR};

/** @brief An HTTP request */
typedef struct _HttpRequest {
  struct _HttpRequest *next;
  void *context;
  uint8_t method; char uri[];
} HttpRequest;

/** @brief Write a POST request to a buffer and queue the request.

    See @ref http_send.
*/
#define http_post(conn, buffer, uri)		\
  http_send (conn, buffer, uri, HTTP_POST)
/** @brief Write a PUT request to a buffer and queue the request.

    See @ref http_send.
*/
#define http_put(conn, buffer, uri)			\
  http_send (conn, buffer, uri, HTTP_PUT)

/** @brief Initialize an HTTP connection.
    @param conn is a pointer to an HttpConnection
    @param client is non-zero for a client HttpConnection
    @param accept is a string listing acceptable media ranges (Accept header)
    @param media is the Content-Type for PUT and POST
*/
void http_init (void *conn, int client, const char *accept, const char *media);

/** @brief Flush queued data to an HTTP connection.
    @param conn is a pointer to an HttpConnection
*/
void http_flush (void *conn);

/** @brief Write data to an HTTP connection immediately if possible or queue
    for later.

    Use http_flush to flush queued data.
    @param conn is a pointer to an HttpConnection
    @param data is an array of bytes
    @param length is the length of data
*/
void http_write (void *conn, void *data, int length);

/** @brief Perform a GET request immediately if possible or queue for later.
    @param conn is a pointer to an HttpConnection
    @param uri is the request URI
*/
void http_get (void *conn, const char *uri);

/** @brief Perform a DELETE request immediately if possible or queue for later.
    @param conn is a pointer to an HttpConnection
    @param uri is the request URI
*/
void http_delete (void *conn, const char *uri);

/** @brief Write a PUT or POST request message to a buffer and queue the
    request.
    
    Use the length returned as a location for the request content. Use the
    function @ref set_content_length to update the Content-Length. Finally, use
    the function @ref http_write to complete the request.
    @param conn is a pointer to an HttpConnection
    @param buffer is a buffer large enough for the request
    @param uri is the request URI
    @param method is either HTTP_PUT or HTTP_POST
    @returns the length of the message
*/
int http_send (void *conn, char *buffer, const char *uri, int method);

/** @brief Write an HTTP status line to buffer.
    @param buffer is the storage for the status line
    @param status is the status code
    @param reason is a string explaining the status
    @returns the length of the status line
*/
int http_status_line (char *buffer, int status, const char *reason);

/** @brief Write HTTP content headers to a buffer.
    @param buffer is the storage for the headers
    @param media is the Content-Type
    @param length is the Content-Length
    @returns the length of the headers
*/
int http_content (char *buffer, const char *media, int length);

/** @brief Update the Content-Length header.

    The Content-Length header should have a enough blank spaces for the length.
    This function overwrites the blank spaces with the length provided.
    @param buffer contains a Content-Length header
    @param length is the value to be set for the Content-Length
*/
void set_content_length (char *buffer, int length);

/** @brief Close the HTTP connection and return queued requests.
    @param conn is a pointer to an HttpConnection
    @returns an HttpRequest linked list
*/
HttpRequest *http_queued (void *conn);

/** @brief Close the HTTP connection and free queued requests/data.
    @param conn is a pointer to an HttpConnection
*/
void http_close (void *conn);

/** @brief Send HTTP response, status 405 (Method Not Allowed).
    @param conn is a pointer to an HttpConnection
    @param allow is a list of allowed methods (Allow header)
*/
void http_allow (void *conn, const char *allow);

/** @brief Send an HTTP response with status.
    @param conn is a pointer to an HttpConnection
    @param status is the status code to return
*/
void http_respond (void *conn, int status);

/** @brief Send an HTTP response with status 201 (Created).
    @param conn is a pointer to an HttpConnection
    @param uri is the value to be returned in the Location header
*/
void http_created (void *conn, const char *uri);

/** @brief Send HTTP response with status 204 (No Content).
    @param conn is a pointer to an HttpConnection
    @param uri is the value to be returned in the Location header
*/
void http_no_content (void *conn, const char *uri);

/** @brief Send HTTP response with status and close connection.
    @param conn is a pointer to an HttpConnection
    @param status is the status code to return 
*/
void http_error (void *conn, int status);

/** @brief Receive an HTTP request/response.
    @param conn is a pointer to an HttpConnection
    @returns one of HTTP_GET, HTTP_PUT, HTTP_POST, HTTP_DELETE, HTTP_HEAD if
    an HTTP request is succesfully received (server).
    
    HTTP_UNKNOWN for a request with an method other than GET, PUT, POST, DELETE,
    or HEAD (server).

    HTTP_RESPONSE for an HTTP response (client)

    HTTP_NONE for an incomplete message (client, server), @ref http_receive
    can called again when more data is available.

    HTTP_ERROR for an incorrect message (client, server)
*/
int http_receive (void *conn);

/** @brief Does the HTTP message have a body?
    @returns 1 if the message has a body, 0 otherwise
*/
int http_body (void *conn);

/** @brief Return data from the message body.

    Call again until all the data has been returned. @ref http_complete

    @param conn is a pointer to an HttpConnection
    @param length is a pointer to the length of the data returned
    @returns a pointer to the data from the message body
*/
char *http_data (void *conn, int *length);

/** @brief Clear data up to the pointer and read more data from the connection.

    An HttpConnection uses a fixed length buffer to retrieve data, this
    function clears data and moves the remainder of the data to the beginning
    of the buffer so more data can be read.
    @param conn is a pointer to an HttpConnection
    @param data is a pointer to 
*/
void http_rebuffer (void *conn, char *data);

/** @brief Is data from the message body complete?
    @param conn is a pointer to an HttpConnection
    @returns 1 is message is complete, 0 otherwise
*/
int http_complete (void *conn);

/** @brief Get status for HTTP response.

    Valid when @ref http_receive returns HTTP_RESPONSE.
    @param conn is a pointer to an HttpConnection
    @returns the status
*/
int http_status (void *conn);

/** @brief Return the method associated with the HTTP response.
    @param conn is a pointer to an HttpConnection
    @returns the HTTP method
*/
int http_method (void *conn);

/** @brief Get the path for an HTTP request.
    @param conn is a pointer to an HttpConnection
    @returns the path
*/
char *http_path (void *conn);

/** @brief Get the query for an HTTP request.
    @param conn is a pointer to an HttpConnection
    @returns the query
*/
char *http_query (void *conn);

/** @brief Get the Accept header value (media range).
    @param conn is a pointer to an HttpConnection
    @returns the media range
*/
char *http_range (void *conn);

/** @brief Is the HTTP connection a client?
    @param conn is a pointer to an HttpConnection
    @returns 1 for a client side connection, 0 for a server side connection
 */
int http_client (void *conn);

/** @brief Get the Location header value
    @param conn is a pointer to an HttpConnection
    @returns the value of the Location header or NULL if none exists
*/
char *http_location (void *conn);

/** @brief Parse an HTTP URI using connection parameters.
    @param buf is a pointer to a buffered Uri
    @param conn is a pointer to an HttpConnection
    @param href is a pointer to the URI string
    @param length is maximmum length of the URI string
    @returns 1 if href is a valid URI, 0 otherwise
*/
int http_parse_uri (void *buf, void *conn, const char *href, int length);

/** @brief Print the HTTP request/response on stdout
    @param conn is a pointer to an HttpConnection
    @param enable is 1 to enable debug output, 0 to disable
*/
void http_debug (void *conn, int enable);

/** @brief Set the context for an HTTP request.

    Sets the context for the last queued HTTP request. This value can later be
    retrieved by @ref http_context when a response is received.
    @param conn is a pointer to an HttpConnection
    @param context is value to associate with the last request
*/
void set_request_context (void *conn, void *context);

/** @brief Get the context for an HTTP response.

    Use @ref set_request_context to set the value of the context for a request,
    this value can then be retrieved using this function once a response has
    been successfully received. 
    @param conn is a pointer to an HttpConnection
    @returns the value associated with the request
*/
void *http_context (void *conn);

/** @} */

#ifndef HEADER_ONLY

#include <time.h>

enum HttpState {HTTP_START, HTTP_HEADER, HTTP_DATA, HTTP_COMPLETE, HTTP_CLOSED};

const char * const http_methods[] =
  {"GET", "PUT", "POST", "DELETE", "HEAD", ""};

typedef struct _SendQueueItem {
  struct _SendQueueItem *next;
  int length;
  char buffer[];
} SendQueueItem;

// An HTTP connection uses a fixed length buffer
// make sure buffer is large enough for the request/status + headers
#define BUFFER_SIZE 2048
typedef struct _HttpConnection {
  Connection tcp;
  char *query, *content_type, *media_range, *location;
  Address host; // host from the Host: header field
  char *headers, *version;
  const char *media; // media type for POST/PUT
  const char *accept; // media types accepted
  char *data; // pointer to the next header line or http content
  int end;    // buffer + end = the end of the http message
  int length; // the amount of data in buffer
  int content_length; // from the Content-Length header
  uint8_t state, method, request_method;
  unsigned body : 1; // response or request has a body
  unsigned close : 1; // close signaled in last request/response
  unsigned client : 1; // true for client connection
  unsigned debug : 1;
  int status, error, header;
  void *context; // request context
  Queue send, request;
  char target[256]; Uri uri; // request target
  char buffer[BUFFER_SIZE];
} HttpConnection;

#define buffer_full(h) (((h)->length+1) == BUFFER_SIZE)

#include "http_parse.c"

#define http_field(conn, name) struct_field (HttpConnection, conn, name)

int http_body (void *conn) { return http_field (conn, body); }
int http_content_length (void *conn) {
  return http_field (conn, content_length); }
int http_status (void *conn) { return http_field (conn, status); }
int http_method (void *conn) { return http_field (conn, request_method); }
char *http_path (void *conn) { return http_field (conn, uri.path); }
char *http_query (void *conn) { return http_field (conn, uri.query); }
char *http_range (void *conn) { return http_field (conn, media_range); }
int http_client (void *conn) { return http_field (conn, client); }
char *http_location (void *conn) { return http_field (conn, location); }
void http_debug (void *conn, int enable) { http_field (conn, debug) = enable; }
void *http_context (void *conn) { return http_field (conn, context); }

void print_http_status (void *conn) { HttpConnection *c = conn;
  const char *method = http_methods[c->request_method];
  printf ("%s %s: %d\n", method, c->uri.path, c->status);  
}

void http_init (void *conn, int client,
		const char *accept,
		const char *media) {
  HttpConnection *c = conn;
  c->client = client;
  c->data = c->buffer;
  c->accept = accept;
  c->media = media;
  c->version = "HTTP/1.1";
  c->headers = "";
}

void print_headers (void *conn, char *buffer) { char *end;
  if (end = strstr (buffer, "\r\n\r\n")) { *end = '\0';
    printf ("--- conn = %p -->\n"
	    "%s\r\n\r\n", conn, buffer); *end = '\r';
  }
}

void http_flush (void *conn) {
  HttpConnection *h = conn; SendQueueItem *i;
  while ((i = queue_peek (&h->send))
	 && conn_write (conn, i->buffer, i->length) > 0) {
    if (h->debug) print_headers (conn, i->buffer);
    free (queue_remove (&h->send));
  }
  if (queue_empty (&h->send) && h->close) conn_close (h);
}

void http_write (void *conn, void *data, int length) {
  HttpConnection *h = conn;
  if (h->send.first || conn_write (conn, data, length) < 0) {
    SendQueueItem *i = malloc (sizeof (SendQueueItem) + length);
    i->length = length; i->next = NULL;
    memcpy (i->buffer, data, length);
    queue_add (&h->send, i); return;
  } if (h->debug) print_headers (conn, data);
}

void queue_request (HttpConnection *c, int method, const char *uri) {
  HttpRequest *r = malloc (sizeof (HttpRequest) + strlen (uri) + 1);
  r->next = r->context = NULL; r->method = method; strcpy (r->uri, uri);
  queue_add (&c->request, r);
}

void set_request_context (void *conn, void *context) {
  HttpConnection *c = conn; HttpRequest *r;
  if (r = queue_tail (&c->request)) r->context = context;
}

HttpRequest *dequeue_request (HttpConnection *c) {
  return queue_remove (&c->request);
}

int http_date (char *buffer) {
  time_t now = time (NULL); struct tm tm = *gmtime (&now);
  return strftime (buffer, 1024, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", &tm);
}

int http_request (void *conn, char *buffer, const char *uri, int method) {
  HttpConnection *c = conn; Address addr;
  const char *name = http_methods[method];
  int n = sprintf (buffer, "%s %s %s\r\n", name, uri, c->version);
  n += http_date (buffer+n); n += sprintf (buffer+n, "Host: ");
  n += write_address_port (buffer+n, net_remote (&addr, c));
  n += sprintf (buffer+n, "\r\nAccept: %s\r\n", c->accept);
  queue_request (conn, method, uri); return n;
}

void http_get (void *conn, const char *uri) { char buffer[256];
  int n = http_request (conn, buffer, uri, HTTP_GET);
  n += sprintf (buffer+n, "\r\n");
  http_write (conn, buffer, n);
}

void http_delete (void *conn, const char *uri) { char buffer[256];
  int n = http_request (conn, buffer, uri, HTTP_DELETE);
  n += sprintf (buffer+n, "\r\n");
  http_write (conn, buffer, n);
}

int http_content (char *buffer, const char *media, int length) {
  int n = sprintf (buffer, "Content-Type: %s\r\n", media);
  n += sprintf (buffer+n, "Content-Length: %-6d\r\n\r\n", length);
  return n;
}

int http_send (void *conn, char *buffer, const char *uri, int method) {
  HttpConnection *c = conn;
  int n = http_request (conn, buffer, uri, method);
  return n + http_content (buffer+n, c->media, 0);
}

void set_content_length (char *buffer, int length) {
  char *field = strstr (buffer, "Content-Length:") + 16;
  field += sprintf (field, "%d", length); *field = ' ';
}

HttpRequest *http_queued (void *conn) {
  HttpConnection *h = conn;
  HttpRequest *r = queue_peek (&h->request);
  queue_free (&h->send); queue_clear (&h->request);
  conn_close (h); h->state = HTTP_CLOSED;
  return r;
}

void http_close (void *conn) {
  printf ("http_close\n");
  free_list (http_queued (conn));
}

int http_status_line (char *buffer, int status, const char *reason) {
  int n = sprintf (buffer, "HTTP/1.1 %d %s\r\n", status, reason);
  return n + http_date (buffer+n);
}

void http_created (void *conn, const char *uri) { char buffer[512];
  int n = http_status_line (buffer, 201, "Created");
  n += sprintf (buffer+n, "Content-Length: 0\r\nLocation: %s\r\n\r\n", uri);
  http_write (conn, buffer, n);
}

void http_no_content (void *conn, const char *uri) { char buffer[512];
  int n = http_status_line (buffer, 204, "No Content");
  n += sprintf (buffer+n, "Location: %s\r\n\r\n", uri);
  http_write (conn, buffer, n);
}

void http_allow (void *conn, const char *allow) { char buffer[512];
  int n = http_status_line (buffer, 405, "Method Not Allowed");
  n += sprintf (buffer+n, "Allow: %s\r\n\r\n", allow);
  http_write (conn, buffer, n);
}

const char *http_reason (int status) {
  switch (status) {
  case 204: return "No Content";
  case 400: return "Bad Request";
  case 403: return "Forbidden";
  case 404: return "Not Found";
  case 405: return "Method Not Allowed";
  case 408: return "Request Timeout";
  case 406: return "Not Acceptable";
  case 415: return "Unsupported Media Type";
  case 500: return "Internal Server Error";
  default: return "";
  }
}

void http_respond (void *conn, int status) { char buffer[512];
  const char *reason = http_reason (status);
  int n = http_status_line (buffer, status, reason);
  n += sprintf (buffer+n, "Content-Length: 0\r\n\r\n");
  http_write (conn, buffer, n);
}

// fatal error, send status and close connection
void http_error (void *conn, int status) {
  printf ("http_error %p %d\n", conn, status); fflush (stdout);
  http_respond (conn, status); http_close (conn);
}

int http_read (void *conn) {
  HttpConnection *h = conn;
  int n = conn_read (conn, h->buffer+h->length,
		     BUFFER_SIZE-h->length-1);
  if (n <= 0) return n;
  h->length += n;
  h->buffer[h->length] = '\0';
  return n;
}

int http_complete (void *conn) { HttpConnection *c = conn;
  return c->state == HTTP_CLOSED || c->end <= c->length;
}

// return data associated with HTTP message
char *http_data (void *conn, int *length) {
  HttpConnection *c = conn;
  if (c->state != HTTP_DATA) return NULL;
 top:
  if (c->close) {
    if (net_status (c) == Closed) c->state++; // HTTP_COMPLETE
    http_read (c); *length = c->length;
  } else if (c->end <= c->length) {
    *length = c->end; c->state++; // HTTP_COMPLETE
  } else if (!buffer_full (c) && http_read (c) > 0)
    goto top;
  else *length = c->length;
  *length -= c->data - c->buffer;
  return c->data;
}					       

void http_rebuffer (void *conn, char *data) {
  HttpConnection *c = conn; int n;
  if (n = data - c->buffer) {
    if (c->end) c->end -= n;
    c->length -= n; c->data = c->buffer;
    memmove (c->buffer, data, c->length+1);
  }
}

// return next complete line in message or NULL
static char *next_line (HttpConnection *h) {
  int c; char *data = h->data;
 top:
  while ((c = *data)) {
    if (c == '\r' && *(data+1) == '\n') {
      *data = '\0';
      return data+2;
    }
    data++;
  } if (http_read (h) > 0) goto top;
  if (data != h->data || h->state != HTTP_START)
    set_timeout (h);
  return NULL;
}

// header flags
#define HTTP_HOST 1
#define HTTP_ACCEPT 2
#define HTTP_CONTENT_TYPE 4
#define HTTP_CONTENT_LENGTH 8
#define HTTP_CONNECTION 16
#define HTTP_LOCATION 32

// receive an HTTP message
int http_receive (void *conn) {
  HttpConnection *c = conn; HttpRequest *r; Uri uri; int i;
  const char * const headers[] = {"host", "accept", "content-type",
				  "content-length", "connection",
				  "location"}; 
  char *header, *method, *target, *text, *data, *next;
  while (1) {
    switch (c->state) {
    case HTTP_START: // request/status line
      ok_v (next = next_line (c), HTTP_NONE); data = c->data;
      if (*data == '\0') break; // allow empty lines to start
      if (c->debug) printf ("<-- conn = %p ---\n"
			    "%s\r\n", c, data);
      c->close = c->end = c->header = c->error = 0;
      c->content_type = c->media_range = NULL; c->body = 1;
      c->content_length = -1;
      if (c->client) {
	if ((data = token_sp (&text, data))
	    && streq (text, c->version) // status line
	    && (data = token_sp (&text, data))
	    && number (&c->status, text)
	    && c->status <= 999
	    && (r = dequeue_request (c))
	    && request_target (c, r->uri)) {
	  c->context = r->context;
	  c->method = HTTP_RESPONSE;
	  c->request_method = r->method; free (r);
	} else goto close;
      } else if ((data = token_sp (&method, data))
		 && (data = token_sp (&target, data))
		 && request_target (c, target)
		 && streq (data, c->version)) { // request line
	c->request_method = c->method = string_index (method, http_methods, 5);
      } else { http_error (c, 400); return HTTP_ERROR; }
      c->state++; break;
    case HTTP_HEADER:
      ok_v (next = next_line (c), HTTP_NONE); data = c->data;
      if (c->debug) printf ("%s\r\n", data);
      switch (*data) {
      case '\0': // end of headers
	if (c->error) {
	  if (c->method == HTTP_RESPONSE) goto close;
	  http_error (c, c->error); return HTTP_ERROR;
	}
	if ((c->method == HTTP_RESPONSE
	     && ((c->header & HTTP_CONTENT_LENGTH && !c->end)
		 || ((c->status >= 100 && c->status <= 199)
		     || c->status == 204 || c->status == 304)))
	     || (c->method != HTTP_RESPONSE && !c->end)) {
	  // response or request with no body
	  c->body = 0; c->state = HTTP_COMPLETE;
	} else { c->state++; // HTTP_DATA
	  if (!c->end) c->close = 1; // close-delimited message
	}
	c->end += next - c->buffer; // message end	
	c->data = next;
	return c->method;
      case ' ': case '\t': c->error = 400; break; // obsolete line folding
      default:
	if (data = token_colon (&header, data)) {
	  i = string_index (to_lower (header), headers, 6);
	  c->header |= 1 << i;
	  switch (i) {
	  case 0: // Host
	    data = ows (parse_host (&uri, &c->host, data));
	    if (*data != '\0') c->error = 400; break;
	  case 1: // Accept
	    c->media_range = data; break;
	  case 2: // Content-Type
	    c->content_type = data; break;
	  case 3: // Content-Length
	    data = ows (number (&c->end, data));
	    if (data && *data == '\0')
	      c->content_length = c->end;
	    else c->error = 400; break;
	  case 4: // Connection
	    if (data = token (&text, data))
	      c->close = streq (to_lower (text), "close");
	    break;
	  case 5: // Location
	    c->location = data;
	  }
	} else c->error = 400;
      } break;
    case HTTP_DATA: return c->method;
    case HTTP_COMPLETE:
      if (!c->close) {
	http_rebuffer (c, c->buffer + c->end);
	c->state = HTTP_START; continue;
      } c->state++;
    case HTTP_CLOSED:
      return HTTP_NONE;
    }
    c->data = next;
  }
 close:
  http_close (c); return HTTP_NONE;
}

#endif
