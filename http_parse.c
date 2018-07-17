// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

static int tchar (int c) {
  return in_range (c, 'A', 'Z') || in_range (c, '^', 'z') 
    || in_range (c, '0', '9') || c == '!' || in_range (c, '#', '\'')
    || in_range (c, '*', '+') || in_range (c, '-', '.')
    || c == '|' || c == '~'; 
}

static char *token (char **t, char *data) {
  *t = data; while (tchar (*data)) data++;
  return *t == data? NULL : data;
}

#define hws(c) (c == ' ' || c == '\t')
#define vchar(c) in_range (c, 0x21, 0x7e)
#define obs_text(c) in_range (c, 0x80, 0xff)
#define qdtext(c) (hws (c) || c == '!' || in_range (c, '#', '[') \
		   || in_range (c, ']', '~') || obs_text (c))

static char *quoted_string (char **value, char *data) {
  char *q = data+1; int c, t; *value = q;
  while ((c = *data++) != '\0') {
    switch (c) {
    case '\\': t = *data++;
      if (hws (t) || vchar (t) || obs_text (t)) *(q++) = t;
      else return NULL; break;
    case '\"': *q = '\0'; return data+1;
    default: if (qdtext (c)) *(q++) = c;
      else return NULL;
    }
  }
  return NULL;
}

static char *pvalue (char **value, char *data) {
  if (*data == '\"') return quoted_string (value, data);
  return token (value, data);
}

// qvalue = ( "0" [ "." 0*3DIGIT ] ) / ( "1" [ "." 0*3("0") ] )
static char *qvalue (int *q, char *data) {
  int n = 3, f = 0;
  switch (*data++) {
  case '0': *q = 0; break;
  case '1': *q = 1000; break;
  default: return NULL;
  }
  if (*data == '.') { data++;
    while (digit (*data) && n-- > 0)
      f = f * 10 + (*data++ - '0');
    while (n-- > 0) f *= 10;
    if (*q == 1000 && f != 0) return NULL;
    else *q += f;
  }
  return data;
}

static char *ows (char *data) { int c;
  if (!data) return NULL;
  for (c = *data; hws (c); c = *(++data));
  return data;
}

typedef struct {
  char *type, *sub;
  char *param[8];
  int quality;
} MediaType;

// parse media range/type
// "Accept" (RFC 7230 Appendix B. Collected ABNF)
// accepts more than one q parameter
char *media_range (MediaType *m, char *data) {
  int state = 0, c, i = 0; char *param, *value;
  ok (data);
  memset (m->param, 0, 8 * sizeof (char *)); c = *data;
  while (1) {
    switch (state) {
    case 0: switch (c) { // type
      case '\0': return NULL;
      case ' ': case '\t': case ',': data++; break;
      default: ok (data = token (&m->type, data)); 
	m->quality = 1000; state++; break;
      } break;
    case 1: ok (c == '/'); data++; state++; break;
    case 2: // subtype
      ok (data = token (&m->sub, data)); state++; break;
    case 3: *data = '\0'; state++;
    case 4: switch (c) { // parameter
      case ',': data++; 
      case '\0': return data;
      case ' ': case '\t': data++; break;
      case ';': state++; data++; break;
      default: return NULL;
      } break;
    case 5: // parameter name 
      ok (data = token (&param, ows (data))); state++; break;
    case 6: ok (c == '='); // parameter value
      *data = '\0'; to_lower (param);
      if (streq (param, "q")) {
	ok (data = qvalue (&m->quality, data+1)); state = 4;
      } else {
	ok (data = pvalue (&value, data+1)); state++;
      } break;
    case 7: *data = '\0'; // completed parameter
      m->param[i] = param; m->param[i+1] = value; i = (i+2)&7;
      state = 4; continue;
    }
    c = *data;
  }
}

int http_parse_uri (void *buf, void *conn, const char *data, int length) {
  if (conn) {
    int secure = conn_secure (conn); Uri *uri = buf;
    uri->scheme = secure? "https" : "http";
    uri->port = secure? 443 : 80;
  } return uri_parse (buf, data, length);
}

// parse HTTP request target (RFC 7230 Appendix B. Collected ABNF)
int request_target (HttpConnection *h, char *data) {
  int c = *data, length = strlen (data);
  if (length > 255) { h->error = 414; return 0; }
  strcpy (h->target, data);
  // authority form is only used for the CONNECT method (not supported)
  if (c == '/')
    data = parse_uri (&h->uri, &h->host, 5, h->target); // origin-form
  else data = parse_uri (&h->uri, &h->host, 0, h->target); // absolute form
  return data != NULL;
}

static char *token_sp (char **token, char *data) {
  int c; *token = data;
  while ((c = *data)) {
    if (hws (c)) { *data = '\0'; return ows (data+1); } data++;
  } return NULL;
}

static char *token_colon (char **token, char *data) {
  int c; *token = data;
  while ((c = *data)) {
    if (c == ':') { *data = '\0'; return ows (data+1); } data++;
  } return NULL;
}

