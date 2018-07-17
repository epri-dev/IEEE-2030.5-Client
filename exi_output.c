// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @addtogroup output
    @{
*/

/** @brief Initialize Output object to output an EXI document

    @param o is a pointer to an Output object
    @param schema is a pointer to a Schema object
    @param buffer is the container for the EXI output
    @param size is the size of the buffer
*/
void exi_output_init (Output *o, const Schema *schema, char *buffer, int size);

/** @} */

#ifndef HEADER_ONLY

void output_byte (Output *o, uint8_t b) {
  if (o->bit) {
    *o->ptr++ |= b >> o->bit;
    *o->ptr = b << (8-o->bit);
  } else *o->ptr++ = b;
}

void output_uint (Output *o, uint64_t x) {
  do { uint8_t b = x & 0x7f; x >>= 7;
    if (x) b |= 0x80;
    output_byte (o, b);
  } while (x);
}

int output_binary (Output *o, uint8_t *b, int n) {
  while (n > 1 && *b == 0) n--, b++;
  output_uint (o, n);
  if (o->ptr+n+1 > o->end) return 0;
  while (n--) output_byte (o, *b++); return 1;
}

void output_bit (Output *o, char bit) {
  if (o->bit == 7) {
    *o->ptr++ |= bit; o->bit = 0;
  } else *o->ptr |= bit << (7 - o->bit++);
}

void output_bits (Output *o, uint32_t bits, int n) {
  char *ptr = o->ptr;
  int m = (o->bit + n) & 7;
  int i = (o->bit + n) >> 3;
  o->bit = m; o->ptr += i;
  if (m) { ptr[i] |= bits << (8-m); bits >>= m; }
  while (i) { ptr[--i] |= bits; bits >>= 8; }
}

void output_integer (Output *o, int64_t x) {
  int sign = x < 0; if (sign) x = -x;
  output_bit (o, sign); output_uint (o, x);
}

int exi_output_literal (Output *o, char *s) {
  char *next; int c;
  output_uint (o, utf8_length (s)+2);
  while (*s && (next = utf8_char (&c, s))) {
    output_uint (o, c); s = next;
  } return 1;
}

int exi_output_string (Output *o, const SchemaElement *se, char *s) {
  const char *name = se_name (se, o->schema);
  StringTable *t = find_table (o->local, name); int i, part, bits;
  if (t) {
    i = find_string (t, s);
    if (i >= 0) { part = 0; goto compact_id; }
  } else t = o->local = new_string_table (o->local, name, 8);
  o->local = add_string (o->local, t, s);
  i = find_string (o->global, s);
  if (i >= 0) { part = 1; t = o->global; goto compact_id; }
  o->global = add_string (o->global, o->global, s);
  exi_output_literal (o, s);
  return 1;
 compact_id:
  output_uint (o, part);
  bits = bit_count (t->index-1); 
  if (bits) output_bits (o, i, bits);
  return 1;
}

int exi_output_value (Output *o, void *value) {
  int type = o->se->xs_type;
  int n = type >> 4;
  if (o->end - o->ptr < 10) return 0;
  switch (type & 0xf) {
  case XS_STRING: if (n) return exi_output_string (o, o->se, value);
  case XS_ANY_URI: return exi_output_string (o, o->se, *(char **)value);
  case XS_BOOLEAN: output_bit (o, ((*(uint32_t *)value) >> o->flag) & 1); break;
  case XS_HEX_BINARY: return output_binary (o, value, n);
  case XS_LONG: output_integer (o, *(int64_t *)value); break;
  case XS_INT: output_integer (o, *(int32_t *)value); break;
  case XS_SHORT: output_integer (o, *(int16_t *)value); break;
  case XS_BYTE: output_bits (o, (*(int8_t *)value)+128, 8); break;
  case XS_ULONG: output_uint (o, *(uint64_t *)value); break;
  case XS_UINT: output_uint (o, *(uint32_t *)value); break;
  case XS_USHORT: output_uint (o, *(uint16_t *)value); break;
  case XS_UBYTE: output_bits (o, *(uint8_t *)value, 8); break;
  } return 1;
}

int exi_output_simple (Output *o, void *value) {
  int type = o->se->xs_type;
  int n = type >> 4; char *s;
  switch (type & 0xf) {
  case XS_STRING: if (n) { s = value; break; }
  case XS_ANY_URI: s = *(char **)value; break;
  default: output_ch:
    output_bit (o, 0); // CH
    return exi_output_value (o, value);
  }
  if (*s == '\0') // don't encode empty strings
    output_bits (o, 0x4, 3); // EE
  else goto output_ch;
  return 1;
}

int exi_output_event (Output *o, const SchemaElement *se, int type) {
  if (o->end - o->ptr >= 3) {
    int bits = bit_count (o->n);
    if (type == EE_EVENT && !o->n) bits = 1;
    output_bits (o, o->code, bits);
    o->n = o->code = 0;
    return 1;
  } return 0;
}

void exi_output_header (Output *o) {
  memset (o->buffer, 0, o->end - o->buffer);
  output_byte (o, 0xa0); // distinguising bits, options present, version
  output_bits (o, 0xc, 6); // header/common/schemaId
  exi_output_literal (o, (char *)o->schema->schemaId);
  output_bits (o, 1, 1); // EE
}

void exi_output_done (Output *o) {
  free_tables (o->global); free_tables (o->local);
}

const OutputDriver exi_output = {
  exi_output_event,
  exi_output_value,
  exi_output_simple,
  exi_output_done
};

void exi_output_init (Output *o, const Schema *schema, char *buffer, int size) {
  output_init (o, schema, buffer, size);
  o->driver = &exi_output;
  exi_output_header (o);
  o->global = new_string_table (NULL, NULL, 32);
}

#endif
