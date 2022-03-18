// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @addtogroup parse
    @{
*/

/** @brief Initialize the Parser to parse an XML document.

    @param p is a pointer to the Parser
    @param schema is a pointer to the Schema
    @param data is a pointer to the XML stream
*/
void parse_init (Parser *p, const Schema *schema, char *data);

/** @} */

#include "xml_token.c"

#ifndef HEADER_ONLY

#define unsigned_int(x, data) only (number64 (x, data))

int signed_int (int64_t *y, char *data) { int sign = 1;
  if (*data == '-') { sign = -1; data++; }
  return unsigned_int ((uint64_t *)y, data)? *y *= sign, 1 : 0;
}

#define pack_signed(dest, sx, data) \
  (signed_int (&sx, data)? *dest = sx, 1 : 0)
#define pack_unsigned(dest, ux, data) \
  (unsigned_int (&ux, data)? *dest = ux, 1 : 0)

int parse_token (Parser *p) {
  if (!p->need_token) return p->token;
  switch ((p->token = xml_token (p->xml))) {
  case XML_INVALID: p->state = PARSE_INVALID; return XML_INVALID;
  case XML_INCOMPLETE: p->ptr = (uint8_t *)p->xml->content;
    return XML_INCOMPLETE;
  default: p->need_token = 0; return p->token;
  }
}

int parse_text (Parser *p) {
  switch (parse_token (p)) {
  case XML_TEXT:
    p->ptr = (uint8_t *)p->xml->content; 
    p->need_token = 1; break;
  case END_TAG: p->ptr = ""; break;
  default: return 0;
  } return 1;
}

int parse_hex (uint8_t *value, int n, char *data) {
  int x, m = 0, c, d;
  //while (c != '\0' && !ws (c)) {
  while (c = hex_digit (d = *data)) {
    if (m == n) return 0; data++;
    x = (d - c) << 4;
    if (c = hex_digit (d = *data++))
      value[m++] = x | (d - c);
    else return 0;
  }
  if (!m || !only (data)) return 0;
  if (n -= m) {
    memmove (value+n, value, m);
    memset (value, 0, n);
  }
  return 1;
}

int parse_value (Parser *p, void *value) {
  int64_t sx; uint64_t ux;
  int type = p->se->xs_type; 
  char *data = (char *)p->ptr;
  int n = type >> 4;
  switch (type & 0xf) {
  case XS_STRING: if (n) {
      if (strlen (data) > n-1) return 0;
      strcpy (value, data);
    } else *(char **)(value) = strdup (data); return 1;
  case XS_BOOLEAN: if (streq (data, "true") || streq (data, "1"))
      *(uint32_t *)value |= 1 << p->flag;
    else if (!(streq (data, "false") || streq (data, "0"))) {
      p->state = PARSE_INVALID; break; } return 1;
  case XS_HEX_BINARY: return parse_hex (value, n, data);
  case XS_ANY_URI: *(char **)(value) = strdup (data); return 1;
  case XS_LONG: return pack_signed ((int64_t *)value, sx, data);
  case XS_INT: return pack_signed ((int32_t *)value, sx, data);
  case XS_SHORT: return pack_signed ((int16_t *)value, sx, data);
  case XS_BYTE: return pack_signed ((int8_t *)value, sx, data);
  case XS_ULONG: return pack_unsigned ((uint64_t *)value, ux, data);
  case XS_UINT: return pack_unsigned ((uint32_t *)value, ux, data);
  case XS_USHORT: return pack_unsigned ((uint16_t *)value, ux, data);
  case XS_UBYTE: return pack_unsigned ((uint8_t *)value, ux, data);
  } return 0;
}

int parse_text_value (Parser *p, void *value) {
  return parse_text (p) && parse_value (p, value);
}

int start_tag (Parser *p, const SchemaElement *se) {
  const char *name = se_name (se, p->schema);
  switch (parse_token (p)) {
  case START_TAG: case EMPTY_TAG:
    if (p->empty) p->state = PARSE_INVALID;
    else if (!streq (name, p->xml->name)) break;
    else { p->empty = p->token; p->need_token = 1;
      if (p->empty && se->simple) p->state = PARSE_INVALID;
      else return 1;
    } break;
  case END_TAG: case XML_INCOMPLETE: break;
  default: p->state = PARSE_INVALID;
  } return 0;
}

int end_tag (Parser *p, const SchemaElement *se) {
  const char *name = se_name (se, p->schema);
  switch (parse_token (p)) {
  case END_TAG: if (streq (name, p->xml->name)) {
      p->need_token = 1; return 1; } 
  } return 0;
}

int compare_names (const void *a, const void *b) {
  const char * const *name_a = a;
  const char * const *name_b = b;
  return strcmp (*name_a, *name_b);
}

void *find_se_name (const Schema *schema, char *name) {
  return bsearch (&name, schema->names, schema->length,
		  sizeof (char *), compare_names);
}

int xml_start (Parser *p) {
  p->need_token = 1;
  while (1) { const char * const *name;
    switch (parse_token (p)) {
    case XML_DECL: if (p->xml_decl) goto invalid;
      p->xml_decl = 1; p->need_token = 1; continue;
    case START_TAG: case EMPTY_TAG:
      if (!(name = find_se_name (p->schema, p->xml->name))) goto invalid;
      p->type = name - p->schema->names;
      p->se = &p->schema->elements[p->type];
      p->empty = p->token; p->need_token = !p->empty; 
      return 1;
    case XML_INCOMPLETE: return 0;
    default: goto invalid;
    } 
  } invalid:
  p->state = PARSE_INVALID; return 0;
}

int xml_next (Parser *p) {
  const SchemaElement *se = p->se;
  while (p->state == PARSE_NEXT) {
    if (!se->n) p->state = PARSE_END;
    else if (se->attribute) {
      const char *name = se_name (se, p->schema);
      if ((p->ptr = attr_value (p->xml->attr, name)))
	p->state = PARSE_ELEMENT;
    } else if (!p->empty) {
      if (start_tag (p, se)) p->state = PARSE_ELEMENT;
      else if (p->token == XML_INCOMPLETE) return 0;
    } else if (se->min) p->state = PARSE_INVALID;
    se++;
  } p->se = se-1; return 1;
}

int xml_end (Parser *p, const SchemaElement *se) {
  if (p->empty || end_tag (p, se)) {
    p->empty = 0; return 1;
  } return 0;
}

int xml_sequence (Parser *p, StackItem *t) {
  const SchemaElement *se = t->se;
  if (start_tag (p, se)) return 1;
  if (p->token <= END_TAG) {
    if (t->count < se->min) p->state = PARSE_INVALID;
    else p->state++;
  } return 0;
}

void parse_done (Parser *p) {
  p->ptr = p->xml->data; p->xml->content = NULL;
}

void xml_rebuffer (Parser *p, char *data, int length) {
  XmlParser *xml = p->xml;
  if (xml->content) {
    int offset = xml->content - data;
    xml->content = data; xml->data -= offset;
  } else xml->data = data;
}

const ParserDriver xml_parser = {
  xml_start, xml_next, xml_end, xml_sequence,
  parse_value, parse_text_value, parse_done, xml_rebuffer
};

void parse_init (Parser *p, const Schema *schema, char *data) {
  XmlParser *xml = p->xml;
  memset (p, 0, sizeof (Parser));
  p->xml = xml? xml : calloc (1, sizeof (XmlParser));
  xml_init (p->xml, data);
  p->schema = schema;
  p->driver = &xml_parser;
  p->need_token = 1;
}

#endif
