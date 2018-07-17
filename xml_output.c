/** @addtogroup output
    @{
*/

/** @brief Initialize an Output object to output an XML document.

    @param o is a pointer to an Output object
    @param schema is a pointer to a Schema object
    @param buffer is a container for the XML document
    @param size is the size of the buffer
*/
void output_init (Output *o, const Schema *schema, char *buffer, int size);

/** @} */

#ifndef HEADER_ONLY

int output_char (Output *o, int c) {
  if (o->ptr+1 < o->end) { *o->ptr++ = c; *o->ptr = '\0'; return 1; }
  return 0;
}

int output_escaped (Output *o, char *s) {
  char *ptr = o->ptr; int c;
  if (!s) {
    printf ("output_quoted: NULL value\n");
    print_stack (&o->stack, o->schema);
    return 0;
  }
  while (*s) {
    char *next = utf8_char (&c, s);
    int size = o->end - ptr, n;
    switch (c) {
    case '<': n = 4; s = "&lt;"; break;
    case '>': n = 4; s = "&gt;"; break;
    case '&': n = 5; s = "&amp;"; break;
    case '\"': n = 6; s = "&quot;"; break;
    default: n = next-s;
    }
    if (size > n) { memcpy (ptr, s, n); ptr += n; }
    else { *o->ptr = '\0'; return 0; }
    s = next;
  } o->ptr = ptr; *ptr = '\0'; return 1;
}

char hex_char (int x) {
  if (x < 10) return '0' + x;
  else return 'a' + (x-10);
}

int output_hex (Output *o, uint8_t *value, int n) {
  int i = 0, size = o->end - o->ptr;
  while (i < n-1 && value[i] == 0) i++;
  if (((n-i)*2) >= size) return 0;
  do { int x = value[i++];
    *o->ptr++ = hex_char (x>>4);
    *o->ptr++ = hex_char (x&0xf);
  } while (i < n);
  return 1;
}

int output_value (Output *o, void *value) {
  int type = o->se->xs_type;
  int n = type >> 4;
  switch (type & 0xf) {
  case XS_STRING: if (n) return output_escaped (o, value);
  case XS_ANY_URI: return output_escaped (o, *(char **)value);
  case XS_BOOLEAN: return ((*(uint32_t *)value) & (1 << o->flag))?
      output_string (o, "true") : output_string (o, "false");
  case XS_HEX_BINARY: return output_hex (o, value, n);
  case XS_LONG: return output_string (o, "%lld", *(int64_t *)value);
  case XS_INT: return output_string (o, "%d", *(int32_t *)value);
  case XS_SHORT: return output_string (o, "%d", *(int16_t *)value);
  case XS_BYTE: return output_string (o, "%d",  *(int8_t *)value);
  case XS_ULONG: return output_string (o, "%llu", *(uint64_t *)value);
  case XS_UINT: return output_string (o, "%u", *(uint32_t *)value);
  case XS_USHORT: return output_string (o, "%u", *(uint16_t *)value);
  case XS_UBYTE: return output_string (o, "%u", *(uint8_t *)value);
  } return 0;
}

int output_quoted (Output *o, void *value) {
  char *last = o->ptr;
  if (!value) {
    printf ("output_quoted: NULL value\n");
    print_stack (&o->stack, o->schema);
  } else if (output_char (o, '\"') && output_value (o, value)
      && output_char (o, '\"')) return 1;
  *last = '\0'; o->ptr = last; return 0;
}

int output_break (Output *o, char *format, ...) {
  va_list args; int n, size; char *last = o->ptr;
  if (!o->first) {
    char *end = o->ptr + o->indent + 1;
    if (end >= o->end) return 0;
    *o->ptr++ = '\n';
    while (o->ptr < end) *o->ptr++ = ' ';
  } size = o->end - o->ptr;
  va_start (args, format);
  n = vsnprintf (o->ptr, size, format, args);
  va_end (args);
  if (n >= size || n < 0) { *last = '\0'; o->ptr = last; return 0; }
  o->ptr += n; return 1; 
}

void output_flush (Output *o) {
  o->ptr = o->buffer;
}

int output_event (Output *o, const SchemaElement *se, int event) {
  const char *name = se_name (se, o->schema);
  if (event & 2 && o->open) {
    if (!output_char (o, '>')) return 0;
    o->open = 0;
  }
  switch (event) {
  case EE_EVENT:
    if (o->open) {
      if (!output_string (o, "/>")) return 0;
      o->open = 0; o->indent -= 2;
    } else if (se->simple) {
      return output_string (o, "</%s>", name);
    } else { o->indent -= 2;
      if (!output_break (o, "</%s>", name)) {
	o->indent += 2; return 0; }
    } break;
  case AT_EVENT: return output_string (o, " %s=", name);
  case SE_SIMPLE: return output_break (o, "<%s>", name);
  case SE_COMPLEX: if (output_break (o, "<%s", name)) {
      if (o->first) {
	const char *ns = o->schema->namespace;
	if (ns && !output_string (o, " xmlns=\"%s\"", ns)) return 0;
	o->first = 0;
      }
      o->open = 1; o->indent += 2; break;
    } else return 0;
  } return 1;
}

void output_done (Output *o) { output_char (o, '\n'); }

const OutputDriver xml_output = {
  output_event,
  output_quoted,
  output_value,
  output_done
};

void output_init (Output *o, const Schema *schema, char *buffer, int size) {
  memset (o, 0, sizeof (Output));
  o->schema = schema; o->ptr = o->buffer = buffer;
  o->end = buffer+size; o->driver = &xml_output;
}

#endif
