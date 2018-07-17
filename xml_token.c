// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include "utf8.c"

#ifndef HEADER_ONLY

#include <stdlib.h>
#include <string.h>

#define MAX_ATTRIBUTE 16

typedef struct _XmlParser {
  char *name, *content, *data;
  int state, token, length;
  char *attr[MAX_ATTRIBUTE*2];
} XmlParser;

enum TokenType {START_TAG, EMPTY_TAG, END_TAG, XML_DECL, XML_PI, XML_TEXT,
		XML_NONE, XML_INCOMPLETE, XML_INVALID};

void *attr_value (char **attr, const char *name) { int i;
  for (i = 0; i < MAX_ATTRIBUTE*2; i += 2)
    if (attr[i]) { if (streq (attr[i], name)) return attr[i+1]; }
    else return NULL;
  return NULL;
}

int name_start (int c) {
  return alpha (c) || c == ':' || c == '_' 
    || in_range (c, 0xc0, 0xd6) || in_range (c, 0xd8, 0xf6)
    || in_range (c, 0xf8, 0x2ff) || in_range (c, 0x370, 0x37d)
    || in_range (c, 0x37f, 0x1fff) || in_range (c, 0x200c, 0x200d)
    || in_range (c, 0x2070, 0x218f) || in_range (c, 0x2c00, 0x2fef)
    || in_range (c, 0x3001, 0xd7ff) || in_range (c, 0xf900, 0xfdcf)
    || in_range (c, 0xfdf0, 0xfffd) || in_range (c, 0x10000, 0xeffff);
}

int name_char (int c) {
  return c == '-' || c == '.' || digit (c)
    || name_start (c) || c == 0xb7
    || in_range (c, 0x300, 0x36f) || in_range (c, 0x203f, 0x2040);
}

char *xml_name (char *data) {
  int first = 1, c; char *next;
  while (next = utf8_char (&c, data)) {
    if (first) { first = 0;
      ok (name_start (c)); 
    } else if (!name_char (c)) return data;
    data = next;
  } return NULL;
}

int xml_char (int c) {
  return ws (c) || in_range (c, 0x20, 0xd7ff)
    || in_range (c, 0xe000, 0xfffd)
    || in_range (c, 0x10000, 0x10ffff);  
}

int hex_digit (int c) {
  if (digit (c)) return '0';
  if (in_range (c, 'a', 'f')) return 'a' - 10;
  if (in_range (c, 'A', 'F')) return 'A' - 10;
  return 0;
}

char *hex_number (int *x, char *data) {
  int c, d, y = 0, n = 0;
  while (c = hex_digit (d = *data))
    y = (y << 4) | (d - c), n++, data++;
  return n > 0? *x = y, data : NULL;
}

char *xml_reference (char **ptr, char *data) {
  const char * const refs[] = {"amp", "lt", "gt", "quot", "apos"};
  const char entity[] = {'&', '<', '>', '\"', '\''};
  char *end = strchr (data, ';'); int ref = 0, i;
  if (!end) return NULL; *end++ = '\0';
  if (*data == '#') { data++;
    data = *data == 'x'? hex_number (&ref, data+1)
      : number (&ref, data);
    return data && *data == '\0'?
      *ptr = utf8_encode (*ptr, ref), end : NULL;
  }
  i = string_index (data, refs, 5);
  return i < 5? *(*ptr)++ = entity[i], end : NULL;
}

char *token_end (char *data, char *end, int n) { char *next;
  if ((next = strstr (data, end))) {
    *next = '\0'; return next+n;
  } return NULL;
}

char *att_value (char *value) {
  int c, q = *value++; char *data = value;
  ok (q == '"' || q == '\''); 
  while (c = *data++) {
    switch (c) {
    case '\0': case '<': return NULL;
    case '&': ok (data = xml_reference (&value, data)); break;
    default: if (c == q) return *value = '\0', data;
    } *value++ = c;
  } return NULL;
}

char *xml_eq (char *data) {
  char *eq = trim (data);
  return *eq == '='? trim (eq+1) : NULL;
}

char *xml_attributes (char **attr, char *data) {
  char *next, *end; int i = 0; data = trim (data);
  memset (attr, 0, sizeof(char *)*MAX_ATTRIBUTE*2);
  while (end = xml_name (data)) {
    char *value = xml_eq (end);
    if (value && (next = att_value (value++))) {
      *end = '\0'; attr[i] = data; attr[i+1] = value;
      i = (i + 2) % (2 * MAX_ATTRIBUTE);
    } else return NULL;
    data = trim (next);
  } return data;
}

int xml_pi (XmlParser *p, char *data) { char *end;
  p->name = data; ok_v (data = end = xml_name (data), 0);
  if (ws (*data)) { *data ='\0';
    if (streq (p->name, "xml")) {
      p->token = XML_DECL;
      return only (xml_attributes (p->attr, data+1));
    } p->content = trim (data+1);
  } else p->content = NULL;
  p->token = XML_PI;
  return 1;
}

char *xml_tag (XmlParser *p, char *data) { char *end;
  p->token = *data == '/'? data++, END_TAG : START_TAG;
  p->name = data; ok (data = end = xml_name (data));
  if (p->token == START_TAG) {
    if (ws (*data))
      ok (data = xml_attributes (p->attr, data+1));
    if (*data == '/') data++, p->token = EMPTY_TAG;
  } else data = trim (data);
  ok (*data == '>'); *end = '\0';
  return data+1;
}

// scan an xml token, return the token type
int xml_token (XmlParser *p) {
  int state = p->state, c;
  char *data = p->data, *next, *text;
  if (p->token == XML_TEXT) text = p->content+p->length;
  while (c = *data) { next = data+1;
    switch (state) {
    case 0: // initial state
      if (c == '<') state = 2;
      else if (ws (c)) next = trim (next);
      else {
	p->content = text = data;
	p->token = XML_TEXT; 
	state++; continue;
      } break;
    case 1: // text
      switch (c) {
      case '&':
	if (strchr (next, ';')) {
	  if (!(next = xml_reference (&text, next)))
	    return XML_INVALID;
	} else goto incomplete; break;
      case '<': state++; break;
      default:
	if (next = utf8_char (&c, data)) {
	  if (xml_char (c))
	    text = utf8_encode (text, c);
	  else return XML_INVALID;
	} else goto incomplete;
      } break;
    case 2: // "<" (tag)
      switch (c) {
      case '!': state = 4; break;
      default:
	if (p->token == XML_TEXT) {
	  p->state = 2; p->data = data;
	  *text = '\0';
	  p->length = text - p->content;
	  p->token = XML_NONE; return XML_TEXT;
	}
	if (c == '?') { state++; break; }
	if (strchr (next, '>'))
	  return (p->data = xml_tag (p, data))?
	    p->state = 0, p->token : XML_INVALID;
	goto incomplete;
      } break;
    case 3: // "<?" (processing instruction)
      if (next = token_end (data, "?>", 2))
	return xml_pi (p, data)? p->data = next, p->token : XML_INVALID;
      goto incomplete;
    case 4:  // "<!" (comment, CDATA)
      switch (c) {
      case '-': state++; break;
      case '[':
	if ((next = token_end (next, "]]>", 3))) {
	  if (streq (data+1, "CDATA[")) {
	    int n = (next-3) - (data+7);
	    memmove (text, data+7, n);
	    text += n; state = 1;
	  } else return XML_INVALID;
	} else goto incomplete; break;
      default: return XML_INVALID;
      } break;
    case 5: // "<!-" (comment)
      if (c == '-') state++;
      else return XML_INVALID;
      break; 
    case 6: // "<!--" (comment)
      if ((next = token_end (data, "-->", 3))) {
	if (*(next-4) == '-') return XML_INVALID;
	state = p->token == XML_TEXT? 1 : 0; break;
      } goto incomplete;
    } data = next;
  }
 incomplete:
  if (p->token == XML_TEXT) p->length = text - p->content;
  else p->content = data;
  p->state = state; p->data = data;
  return XML_INCOMPLETE;
}

void xml_init (XmlParser *p, char *buffer) {
  p->content = p->data = buffer; p->token = XML_NONE; p->state = 0;
}

#endif
