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

/*向output对象的缓冲区输出一个字节*/
int output_char (Output *o, int c) {
  if (o->ptr + 1 < o->end) {
    *o->ptr++ = c;  //"*"的优先级高于"++"，所以是先设置值，再增长ptr的值。
    *o->ptr = '\0';
    return 1;
  }
  return 0;
}

/* 向output对象的缓冲器输出一个“转义字符”。比如在数据对象中，原本是'<'符号，要以"&lt;"符号输出来表示。 */
int output_escaped (Output *o, char *s) {
  char *ptr = o->ptr;
  int c;
  if (!s) {
    printf ("output_quoted: NULL value\n");
    print_stack (&o->stack, o->schema);
    return 0;
  }
  while (*s) {
    char *next = utf8_char (&c, s);
    int size = o->end - ptr, n;
    switch (c) {
    case '<':
      n = 4;
      s = "&lt;";
      break;
    case '>':
      n = 4;
      s = "&gt;";
      break;
    case '&':
      n = 5;
      s = "&amp;";
      break;
    case '\"':
      n = 6;
      s = "&quot;";
      break;
    default:
      n = next - s;
    }
    if (size > n) {
      memcpy (ptr, s, n);
      ptr += n;
    } else {
      *o->ptr = '\0';
      return 0;
    }
    s = next;
  }
  o->ptr = ptr;
  *ptr = '\0';
  return 1;
}

char hex_char (int x) {
  if (x < 10) return '0' + x;
  else return 'a' + (x - 10);
}

int output_hex (Output *o, uint8_t *value, int n) {
  int i = 0, size = o->end - o->ptr;
  while (i < n - 1 && value[i] == 0) i++;
  if (((n - i) * 2) >= size) return 0;
  do {
    int x = value[i++];
    *o->ptr++ = hex_char (x >> 4);
    *o->ptr++ = hex_char (x & 0xf);
  } while (i < n);
  return 1;
}


/*输出一个数字到output对象的缓冲区中*/
int output_value (Output *o, void *value) {
  int type = o->se->xs_type;
  int n = type >> 4;
  switch (type & 0xf) {
  case XS_STRING:
    if (n) return output_escaped (o, value);
  case XS_ANY_URI:
    return output_escaped (o, *(char **)value);
  case XS_BOOLEAN:
    return ((*(uint32_t *)value) & (1 << o->flag)) ?
           output_string (o, "true") : output_string (o, "false");
  case XS_HEX_BINARY:
    return output_hex (o, value, n);
  case XS_LONG:
    return output_string (o, "%lld", *(int64_t *)value);
  case XS_INT:
    return output_string (o, "%d", *(int32_t *)value);
  case XS_SHORT:
    return output_string (o, "%d", *(int16_t *)value);
  case XS_BYTE:
    return output_string (o, "%d",  *(int8_t *)value);
  case XS_ULONG:
    return output_string (o, "%llu", *(uint64_t *)value);
  case XS_UINT:
    return output_string (o, "%u", *(uint32_t *)value);
  case XS_USHORT:
    return output_string (o, "%u", *(uint16_t *)value);
  case XS_UBYTE:
    return output_string (o, "%u", *(uint8_t *)value);
  }
  return 0;
}

/*输出一个"引用的"内容，即带了双引号的字符串，比如典型的：
<DERControl href="/derp/2/derc/0">
中的href内容
*/
int output_quoted (Output *o, void *value) {
  char *last = o->ptr;
  if (!value) {
    printf ("output_quoted: NULL value\n");
    print_stack (&o->stack, o->schema);
  } else if (output_char (o, '\"') && output_value (o, value)
             && output_char (o, '\"')) return 1;
  *last = '\0';
  o->ptr = last;
  return 0;
}

/*输出一个换行，并且填充空格，即将输出的内容做换行格式化*/
int output_break (Output *o, char *format, ...) {
  va_list args;
  int n, size;
  char *last = o->ptr;
  if (!o->first) {  //如果当前打印的不是第一行，或者说第一行已经打印掉了，则需要在后面打印空格作为缩进。
    char *end = o->ptr + o->indent + 1;
    if (end >= o->end) return 0;
    *o->ptr++ = '\n';
    while (o->ptr < end) *o->ptr++ = ' ';
  }
  size = o->end - o->ptr;
  va_start (args, format);
  n = vsnprintf (o->ptr, size, format, args);
  va_end (args);
  if (n >= size || n < 0) { //如果实际可以打印的内容超过了缓冲区大小，则意味着打印失败。
    *last = '\0';
    o->ptr = last;
    return 0;
  }
  o->ptr += n;
  return 1;
}

/*将之前待输出的内容废弃掉*/
void output_flush (Output *o) {
  o->ptr = o->buffer;
}

/**/
int output_event (Output *o, const SchemaElement *se, int event) {
  const char *name = se_name (se, o->schema);
  if (event & 2 && o->open) {   /*如果输出尚未结束，则此时让其结束*/
    if (!output_char (o, '>')) return 0;
    o->open = 0;
  }
  switch (event) {
  case EE_EVENT:
    if (o->open) {
      if (!output_string (o, "/>")) return 0;
      o->open = 0;
      o->indent -= 2;
    } else if (se->simple) {
      return output_string (o, "</%s>", name);/* 如果是一个简单类型，比如数字，则开始tag和结束tag都是在同一行中。*/
    } else {
      o->indent -= 2;
      if (!output_break (o, "</%s>", name)) { /* 如果是复杂类型的，则需要多行来容纳，需要换行。*/
        o->indent += 2;
        return 0;
      }
    }
    break;
  case AT_EVENT:
    return output_string (o, " %s=", name);   /*应该是打印属性的意思，典型的如  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" href="/dcap/tm"> */
  case SE_SIMPLE:
    return output_break (o, "<%s>", name);  //这个是一个开始tag并且不含有属性。
  case SE_COMPLEX:
    if (output_break (o, "<%s", name)) {
      if (o->first) { //如果首行还未打印则现在打印首行。
        const char *ns = o->schema->namespace;
        if (ns && !output_string (o, " xmlns=\"%s\"", ns)) return 0;  //namespace存在就打印namespace
        o->first = 0;
      }
      o->open = 1;
      o->indent += 2;
      break;
    } else return 0;
  }
  return 1;
}

void output_done (Output *o) {
  output_char (o, '\n');
}

const OutputDriver xml_output = {
  output_event, /*output_event*/
  output_quoted,/*output_attr_value，输出一段引用的字符串*/
  output_value, /*output_value 成员，输出一个数值*/
  output_done   /*output_done 成员*/
};

void output_init (Output *o, const Schema *schema, char *buffer, int size) {
  memset (o, 0, sizeof (Output));
  o->schema = schema;
  o->ptr = o->buffer = buffer;
  o->end = buffer + size;
  o->driver = &xml_output;
}

#endif
