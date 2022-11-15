// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup uri Uri

    Provides a parser for Uniform Resource Identifiers (URIs
    <a href="https://www.ietf.org/rfc/rfc3986.txt">RFC 3986</a>)
    @{
*/

/** Parsed URI representation */
typedef struct {
  char *scheme; ///< is the URI scheme:HTTP or HTTPS  HTTP方案，是HTTP或者是HTTPS。
  char *name; ///< is the host name (if any)  主机名
  char *end; ///< is a pointer to the end of the host name 主机名的结尾
  char *path; ///< is the URI path  文件路径
  char *query; ///< is the URI query 查询字符
  Address *host; ///< is pointer to the URI host 主机的端口和地址的数字形式
  int port; ///< is the URI port    服务器端口
} Uri;

/** A buffered URI (abstract type 抽象类型) 这里未定义后面的buffer数组长度*/
typedef struct {
  Uri uri;
  Address host;
  char buffer[];  //这里的buffer不占用空间。这个是个柔性数组。可参考 https://www.zhihu.com/question/51784236 。
} UriBuffered;

/** A buffered URI instance type 定义了实际的数据长度 ， 跟上面的 UriBuffered 实体 */
typedef struct {
  Uri uri;
  Address host;
  char buffer[128];
} Uri128;

/** @brief Parse a URI.
    @param buf is a pointer to a buffered Uri
    @param href is a pointer to URI string
    @param length is the maximum length of the URI string
 */
int uri_parse (void *buf, const char *href, int length);

int write_uri (char *buffer, Uri *uri);

/** @} */

#ifndef HEADER_ONLY

char *pchar (char **p, char *data) {
  int c = *data++, x, y;
  if (alpha (c) || in_range (c, '&', ';') || c == '='
      || c == '!' || c == '$' || c == '@' || c == '_' || c == '~') {
    *(*p)++ = c;
    return data;
  } else if (c == '%') {
    ok (hex4 (&x, data++) && hex4 (&y, data++));
    *(*p)++ = (x << 4) | y;
    return data;
  }
  return NULL;
}

// parse the host component of a URI
char *parse_host (Uri *uri, Address *host, char *data) {
  char *name, *last;
  if (last = parse_address (host, uri->port, data)) {
    uri->host = host;
  } else {
    char *end = name = data;
    while (data) {
      last = data;
      switch (*data) {
      case '@':
      case ':':
        goto out;
      default:
        data = pchar (&end, data);
      }
    }
out:
    if (name != end) {
      uri->name = name;
      uri->end = end;
    }
  }
  data = last;
  if (*data == ':') {
    *data = '\0';
    data = number_q (&uri->port, data + 1);
    if (uri->host) set_port (host, uri->port);
  }
  return data;
}

// terminate URI host name without modifying the URI path
void terminate_host (Uri *uri) {
  if (uri->name && *uri->end != '\0') { // host name not terminated
    int count = uri->end - uri->name;
    if (uri->name + count == uri->path) {
      memmove (uri->name - 1, uri->name, count);
      uri->name--;
    }
    uri->name[count] = '\0';
  }
}

// parse URI-reference (RFC 3986)
/*将URL中的各个要素都解析出来.

state 参数用于指定解析过程从哪个步骤开始执行

*/
char *parse_uri (Uri *uri, Address *host, int state, char *data) {
  char *p;
  int c;
  uri->name = uri->path = uri->query = NULL;
  uri->host = NULL;
  while (1) {
    c = *data;
    switch (state) {
    case 0:
      if (c == '/') {
        ok (uri->scheme);
        state = 3;
      } else if (alpha (c)) {
        p = data;
        state++;
      } else return NULL;
      break;
    case 1: // scheme
      if (c == ':') {
        *data = '\0';
        uri->scheme = p;
        state++;
        if (streq (uri->scheme, "http")) uri->port = 80;
        else if (streq (uri->scheme, "https")) uri->port = 443;
      } else {
        ok (alpha (c) || digit (c) || c == '+' || c == '-' || c == '.');
      }
      break;
    case 2: // hier-part
      if (c == '/') {
        state++;
        break;
      }
      state = 5;
      continue;
    case 3: // hier-part
      if (c == '/') {
        state++;
        break;
      }
      data--;
      state = 5;
      continue;
    case 4: // authority
      data = parse_host (uri, host, data);
      state++;
      continue;
    case 5:
      p = uri->path = data;
      state++; // path
    case 6: // path
      switch (c) {
      case '/':
        *p++ = '/';
        break;
      case '?':
        *p = '\0';
        p = uri->query = data + 1;
        state++;
        break;
      default:
        goto pchar_data;
      }
      break;
    case 7: // query
      switch (c) {
      case '/':
      case '?':
        *p++ = c;
        break;
      default:
        goto pchar_data;
      }
    }
    data++;
    continue;
pchar_data:
    if (c == '\0' || ws (c)) {
      *p = '\0'; // terminate path or query
      terminate_host (uri);
      return data;
    }
    ok (data = pchar (&p, data));
  }
}


// parse absolute URI (buffered) 将一个href字符串中的各个元素都拆分开，放到一个UriBuffered结构的数据中。

int uri_parse (void *buf, const char *href, int length) {
  UriBuffered *uri = buf;
  if (strlen (href) > length) return 0;
  strcpy (uri->buffer, href);
  return parse_uri (buf, &uri->host, 0, uri->buffer) != NULL;
}

int write_uri (char *buffer, Uri *uri) {
  int n = 0;
  if (uri->name || uri->host) {
    n = sprintf (buffer, "%s://", uri->scheme);
    if (uri->name)
      n += sprintf (buffer + n, "%s:%d", uri->name, uri->port);
    else n += write_address_port (buffer + n, uri->host);
  }
  if (uri->path) n += sprintf (buffer + n, "%s", uri->path);
  if (uri->query) n += sprintf (buffer + n, "?%s", uri->query);
  return n;
}

#endif
