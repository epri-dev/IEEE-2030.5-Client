// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @addtogroup parse
    @{
*/

/** @brief Initialize the Parser to parse an EXI document.

    @param p is a pointer to a Parser object
    @param schema is the schema to use
    @param data is the beginning of the EXI stream
    @param length is the length of the stream in bytes
*/
void exi_parse_init (Parser *p, const Schema *schema, char *data, int length);

/** @} */

#ifndef HEADER_ONLY

/* The EXI bit-stream is encoded within a byte-stream, if the number of bytes
   needed from the byte stream is not met, set the truncated bit and return 0.
*/
#define need(p, n) if ((p->end - p->ptr) < (n)) return p->truncated = 1, 0

uint8_t parse_byte (Parser *p) {
  need (p, p->bit? 2 : 1);
  if (p->bit) {
    uint8_t b = (*p->ptr << p->bit) | (*(p->ptr+1) >> (8-p->bit));
    p->ptr++; return b;
  } return *p->ptr++;
}

// parse unsigned integers up to 70 (7x10) bits
uint64_t parse_uint (Parser *p) {
  uint64_t x = 0; int n = 0; uint8_t b;
  do { b = parse_byte (p);
    x |= (b & 0x7f) << n; n += 7;
  } while (!p->truncated && b & 0x80 && n < 70);
  return x;
}

/* parse binary data encoded as an unsigned integer and the same number of
   bytes, the length must be less than or equal to the predefined length by 
   the schema */
int parse_binary (Parser *p, uint8_t *b, uint64_t n) {
  uint64_t m = parse_uint (p);
  if (p->truncated) return 0;
  if (m > n) { p->state = PARSE_INVALID; return 0; }
  need (p, p->bit? m+1 : m);
  b += n-m;
  while (m--) *b++ = parse_byte (p); return 1;
}

int parse_bit (Parser *p) { need (p, 1);
  int bit = (*p->ptr >> (7-p->bit++)) & 1; 
  if (p->bit & 8) { p->ptr++; p->bit = 0; }
  return bit;
}

// parse n bits from the bit stream
uint32_t parse_bits (Parser *p, int n) {
  uint32_t bits = *p->ptr;
  int bit = (p->bit + n) & 7; // final bit offset
  int bytes = (p->bit + n) >> 3; // number of bytes to fetch
  need (p, bit? bytes+1 : bytes); p->bit = bit;
  while (bytes--) bits = (bits << 8) | *(++p->ptr);
  return (bits >> (8-bit)) & ~(-1 << n);
}

// parse a signed integer (bounded range has greater than 4096 values)
int64_t parse_integer (Parser *p) {
  uint8_t *ptr = p->ptr; int bit = p->bit;
  int sign = parse_bit (p);
  int64_t x = parse_uint (p);
  if (p->truncated) { p->ptr = ptr; p->bit = bit; }
  return sign? -x : x;
}

void parse_literal (Parser *p, char *s, int n) {
  while (n--) s = utf8_encode (s, parse_uint (p)); *s = '\0';
}

// parse compact id and look up string in the string table
int parse_compact_id (Parser *p, StringTable *t, void *value, int n) {
  if (t && t->index) {
    int id = parse_bits (p, bit_count (t->index-1));
    if (p->truncated) return 0;
    if (id < t->index) {
      char *s = t->strings[id];
      if (n) { if (strlen (s)+1 <= n) { strcpy (value, s); return 1; }
      } else { *(char **)value = strdup (s); return 1; } 
    } 
  } p->state = PARSE_INVALID; return 0;
}

// length of a utf-8 string (encoded as an exi string)
int exi_utf8_length (Parser *p, int n) {
  uint8_t *ptr = p->ptr; int length = 0;
  while (n) { int m = 1, b;
    /* UTF-8 code points need at most 21 bits (3 bytes in EXI encoding) and
       are encoded using 1 to 4 bytes in UTF-8. */
    while ((b = parse_byte (p)) & 0x80)
      if (++m > 3) { p->state = PARSE_INVALID; return -1; }
    if (p->truncated) return -1;
    // adjust encoded length based upon bits needed
    switch (m) {
    case 2: if (b & 0x70) m++; break; // more than 11 bits needed?
    case 3: if (b & 0x7c) m++; break; // more than 16 bits needed?
    } length += m; n--;
  } p->ptr = ptr; return length;
}

// parse an EXI string, either a compact identifier or string literal
int exi_parse_string (Parser *p, void *value, int n) {
  const SchemaElement *se = p->se; StringTable *t;
  const char *name = se_name (se, p->schema);
  uint8_t *ptr = p->ptr; char *s;
  int m = parse_uint (p);
  if (p->truncated) return 0;
  switch (m) {
  case 0: // local value lookup
    return parse_compact_id (p, find_table (p->local, name), value, n);
  case 1: // global value lookup
    return parse_compact_id (p, p->global, value, n);
  default: // literal value encoding
    m = exi_utf8_length (p, m-2);
    if (m < 0) { p->ptr = ptr; return 0; }
    if (n) { // string is stored in a fixed container
      if (m >= n) { p->state = PARSE_INVALID; return 0; }
      s = value;
    } else { *(char **)value = s = malloc (m+1); }
    parse_literal (p, s, m);
    t = find_table (p->local, name);
    if (!t) t = p->local = new_string_table (p->local, name, 8);
    p->local = add_string (p->local, t, s);
    p->global = add_string (p->global, p->global, s);
  } return 1;
}

int exi_parse_value (Parser *p, void *value) {
  int type = p->se->xs_type, n = type >> 4;
  switch (type & 0xf) {
  case XS_STRING: return exi_parse_string (p, value, n);
  case XS_BOOLEAN: if (parse_bit (p)) *(uint32_t *)value |= 1 << p->flag; break;
  case XS_HEX_BINARY: return parse_binary (p, value, n);
  case XS_ANY_URI: return exi_parse_string (p, value, 0);
  case XS_LONG: *(int64_t *)value = parse_integer (p); break;
  case XS_INT: *(int32_t *)value = parse_integer (p); break;
  case XS_SHORT: *(int16_t *)value = parse_integer (p); break;
  case XS_BYTE: *(int8_t *)value = parse_bits (p, 8) - 128; break;
  case XS_ULONG: *(uint64_t *)value = parse_uint (p); break;
  case XS_UINT: *(uint32_t *)value = parse_uint (p); break;
  case XS_USHORT: *(uint16_t *)value = parse_uint (p); break;
  case XS_UBYTE: *(uint8_t *)value = parse_bits (p, 8); break;
  } return !p->truncated;
}

/* IEEE 2030.5-2017 EXI options document in XML format
  <header xmlns="http://www.w3.org/2009/exi">
    <common><schemaId>S1</schemaId></common>
  </header> */
/* IEEE 2030.5-2017 EXI options document in EXI format (bitstream)
   0 (header) | 01 (common) | 10 (schemaID) | 0 (CH)
   | 0x04 (literal string length=2) | 0x53 0x31 ("S1") | 1 (EE) */

/* IEEE 2030.5 uses a fixed set of options, so we only check for the header
   and the options document as defined above. */
int exi_parse_header (Parser *p) { int c;
  if (memcmp (p->ptr, "$EXI", 4) == 0) p->ptr += 4;
  need (p, 5); // need 5 bytes
  c = *p->ptr++; // EXI header is one byte in this case
  // 10 (distinguishing bits) | 1 (options present) | 00000 (version = 1)
  if (c == 0xa0) {
    char schemaId[64];
    uint32_t x = parse_bits (p, 6);
    int n = parse_uint (p);
    if (n && n < 64) { n -= 2;
      parse_literal (p, schemaId, n);
      // verify options document
      if (x == 0xc && streq (schemaId, p->schema->schemaId))
	return parse_bit (p); // EE
    }
  } p->state = PARSE_INVALID; return 0;
}

// parse the header and the first event code (the global element)
int exi_parse_start (Parser *p) {
  if (!exi_parse_header (p)) return 0;
  p->type = parse_bits (p, bit_count (p->schema->length));
  if (p->type < p->schema->length) {
    p->se = &p->schema->elements[p->type];
    p->need_token = 1; return 1;
  } p->state = PARSE_INVALID; return 0;
}

int exi_parse_simple (Parser *p, void *value) {
  if (parse_bit (p)) {
    if (parse_bits (p, 3)) 
      return p->state = PARSE_INVALID, 0;
    return p->need_token = 0, 1; // EE is a second level code in this context
  } return exi_parse_value (p, value);
}

int exi_event (Parser *p, int n) {
  p->token = parse_bits (p, bit_count (n));
  if (p->truncated) return 0;
  if (p->token >= n)
    return p->state = PARSE_INVALID, 0;
  return 1;
}

int exi_parse_next (Parser *p) {
  const SchemaElement *se = p->se;
  if (se->n) {
    if (p->need_token) {
      int n = se->min? 1 : se->n;
      if (!exi_event (p, n)) return 0;
    }
    se += p->token;
    p->need_token = se->n? 1 : 0;
    p->state = se->n? PARSE_ELEMENT : PARSE_END;
    p->se = se;
  } else p->state = PARSE_END;
  return 1;
}

int exi_parse_end (Parser *p, const SchemaElement *se) {
  if (p->need_token) {
    if (parse_bit (p))
      return p->state = PARSE_INVALID, 0;
  } else p->need_token = 1; return 1;
}

int exi_parse_sequence (Parser *p, StackItem *t) {
  const SchemaElement *se = t->se;
  int n = t->count >= se->min? se->n : 1;
  if (exi_event (p, n)) {
    if (p->token) p->need_token = 0, p->token--, p->state++;
    else return 1;
  } return 0;
}

void exi_parse_done (Parser *p) {
  free_tables (p->global); free_tables (p->local);
}

void exi_rebuffer (Parser *p, char *data, int length) {
  p->ptr = data; p->end = data + length; p->truncated = 0;
}
 
const ParserDriver exi_parser = {
  exi_parse_start, exi_parse_next, exi_parse_end,
  exi_parse_sequence, exi_parse_value, exi_parse_simple,
  exi_parse_done, exi_rebuffer
};

void exi_parse_init (Parser *p, const Schema *schema,
		     char *data, int length) {
  memset (p, 0, sizeof (Parser));
  exi_rebuffer (p, data, length);
  p->schema = schema; p->driver = &exi_parser;
  p->global = new_string_table (NULL, NULL, 32);
}

#endif
