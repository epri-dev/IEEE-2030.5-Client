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

//判断一个字符串是否仅仅包含了了一个表示10进制数字的内容
#define unsigned_int(x, data) only (number64 (x, data))

/*
将一个表示有符号10进制数字的字符串，转换成对应的10进制数字。
参数：
y:用来返回转换后的数字。
data:文本地址，在执行完毕该操作后，data将移动到字符串末尾
*/
int signed_int (int64_t *y, char *data) {
  int sign = 1;
  if (*data == '-') {
    sign = -1;
    data++;
  }
  return unsigned_int ((uint64_t *)y, data) ? *y *= sign, 1 : 0;
}

#define pack_signed(dest, sx, data) \
  (signed_int (&sx, data)? *dest = sx, 1 : 0)
#define pack_unsigned(dest, ux, data) \
  (unsigned_int (&ux, data)? *dest = ux, 1 : 0)


//解析出来一个 token 。
int parse_token (Parser *p) {
  if (!p->need_token) return p->token;
  switch ((p->token = xml_token (p->xml))) {
  case XML_INVALID:
    p->state = PARSE_INVALID;
    printf("parse_token:PARSE_INVALID\n");
    return XML_INVALID;
  case XML_INCOMPLETE:
    p->ptr = (uint8_t *)p->xml->content;
    printf("parse_token:XML_INCOMPLETE\n");
    return XML_INCOMPLETE;
  default:
    printf("parse_token:default:succeed,token name:%s\n",p->xml->name);
    p->need_token = 0;
    return p->token;
  }
}

int parse_text (Parser *p) {
  switch (parse_token (p)) {
  case XML_TEXT:
    p->ptr = (uint8_t *)p->xml->content;
    p->need_token = 1;
    break;
  case END_TAG:
    p->ptr = "";
    break;
  default:
    printf("parse_text:default return 0\n");
    return 0;
  }
  return 1;
}

int parse_hex (uint8_t *value, int n, char *data) {
  int x, m = 0, c, d;
  //while (c != '\0' && !ws (c)) {
  while (c = hex_digit (d = *data)) {
    if (m == n) return 0;
    data++;
    x = (d - c) << 4;
    if (c = hex_digit (d = *data++))
      value[m++] = x | (d - c);
    else return 0;
  }
  if (!m || !only (data)) return 0;
  if (n -= m) {
    memmove (value + n, value, m);
    memset (value, 0, n);
  }
  return 1;
}
//解析一个tag中的属性值，将其填充到value指针给出的地址上去。这个地址是这个待解析的属性，在基类数据中的偏移地址。
int parse_value (Parser *p, void *value) {
  int64_t sx;
  uint64_t ux;
  int type = p->se->xs_type;
  char *data = (char *)p->ptr;
  int n = type >> 4;
  //printf("parse_value:%s\n",data);
  switch (type & 0xf) {
  case XS_STRING:
    if (n) {
      if (strlen (data) > n - 1){printf("parse_value:XS_STRING length error\n");return 0;}
      strcpy (value, data);
    } else *(char **)(value) = strdup (data);
    return 1;
  case XS_BOOLEAN:
    if (streq (data, "true") || streq (data, "1"))
      *(uint32_t *)value |= 1 << p->flag;
    else if (!(streq (data, "false") || streq (data, "0"))) {
      p->state = PARSE_INVALID;
      break;
    }
    return 1;
  case XS_HEX_BINARY:
    return parse_hex (value, n, data);
  case XS_ANY_URI:
    *(char **)(value) = strdup (data);
    return 1;
  case XS_LONG:
    return pack_signed ((int64_t *)value, sx, data);
  case XS_INT:
    return pack_signed ((int32_t *)value, sx, data);
  case XS_SHORT:
    return pack_signed ((int16_t *)value, sx, data);
  case XS_BYTE:
    return pack_signed ((int8_t *)value, sx, data);
  case XS_ULONG:
    return pack_unsigned ((uint64_t *)value, ux, data);
  case XS_UINT:
    return pack_unsigned ((uint32_t *)value, ux, data);
  case XS_USHORT:
    return pack_unsigned ((uint16_t *)value, ux, data);
  case XS_UBYTE:
    return pack_unsigned ((uint8_t *)value, ux, data);
  }
  printf("parse_value:return 0 at end\n");
  return 0;
}

int parse_text_value (Parser *p, void *value) {
  return parse_text (p) && parse_value (p, value);
}
//如果解析到一个tag的开始部分（tag的名字部分）
int start_tag (Parser *p, const SchemaElement *se) {
  const char *name = se_name (se, p->schema);//printf("\nstart_tag:name:%s\n",name);
  switch (parse_token (p)) {
  case START_TAG:
  case EMPTY_TAG:
    if (p->empty) p->state = PARSE_INVALID;
    else if (!streq (name, p->xml->name)) break;  //如果待比较的xml中的这个tag的名字跟现在se中的名字不符，那么就退出。
    else {//如果比较后相符，则相当于该xml中的这个tag存在于“se_schema.c”文件中的某一个类型中的某个子类（的名字），意味这个这个tag是有效的。
      p->empty = p->token;  //
      p->need_token = 1;    //看起来意味着“解析下一个token”的意思。
      if (p->empty && se->simple) p->state = PARSE_INVALID;
      else return 1;  //表示找到了一个在表中的token（SE子类名）
    }
    break;
  case END_TAG:
  case XML_INCOMPLETE:
    break;
  default:
    p->state = PARSE_INVALID;
  }
  return 0;
}

int end_tag (Parser *p, const SchemaElement *se) {
  const char *name = se_name (se, p->schema);
  //printf("end_tag:name:%s\n",name);
  int token=0;
  switch (token = parse_token (p)) {
  case END_TAG:
    if (streq (name, p->xml->name)) {
      p->need_token = 1;
      return 1;
    }
  }
  printf("end_tag:return 0,token:%d\n",token);
  return 0;
}

int compare_names (const void *a, const void *b) {
  const char *const *name_a = a;
  const char *const *name_b = b;
  return strcmp (*name_a, *name_b);
}

//在schema->names( 在 se_schema.c 这个文档中 ) 中找到要搜索的name，看是否存在。
void *find_se_name (const Schema *schema, char *name) {
  return bsearch (&name, schema->names, schema->length,
                  sizeof (char *), compare_names);
}

/*解析到一个“START_TAG”，取得其名字和属性，以及名字相对应的se位置*/
int xml_start (Parser *p) {
  //static int times = 0;
  p->need_token = 1;
  while (1) {
    const char *const *name;
    switch (parse_token (p)) {
    case XML_DECL:
      if (p->xml_decl) goto invalid;
      p->xml_decl = 1;
      p->need_token = 1;
      continue;
    case START_TAG: //如果解析到一个表示“开始”的tag
    case EMPTY_TAG:
      if (!(name = find_se_name (p->schema, p->xml->name))) goto invalid;
      p->type = name - p->schema->names;    //相对于数组首元素的偏移量，与 se_types.h 中定义的类型定义一致。
      p->se = &p->schema->elements[p->type];//在 elements 数组中的位置
      p->empty = p->token;
      p->need_token = !p->empty;
      printf("xml_start:succeed to parse a tag,name:%s,return 1\n",p->xml->name);
      return 1;
    case XML_INCOMPLETE:
      return 0;
    default:
      goto invalid;
    }
  }
invalid:
  p->state = PARSE_INVALID;
  printf("xml_start:PARSE_INVALID,tag name:%s,return 0\n",p->xml->name);
  return 0;
}
/*基本原理是：从某个se对象的第一个成员开始，逐行往下，对之前xml文本中解析出来的内容执行匹配，如果匹配到，那么*/
/*检查当前的这个se（从se_schema中取出的某一行）是否：1）属于了前面从xml中解析出来的属性表中的其中一个属性，如果是，则接下去的步骤是解析该属性。2）是否是一个tag，如果是，则接下去解析*/
int xml_next (Parser *p) {
  const SchemaElement *se = p->se;  //从前面设定好了的se开始往后扫描
  while (p->state == PARSE_NEXT) {
    if (!se->n) p->state = PARSE_END; //如果n=0，表示这一个基类中的子类都已经解析完了，解析到此结束。
    else if (se->attribute) { //如果这行表示的似乎属性
      const char *name = se_name (se, p->schema);printf("xml_next:attribute se name:%s\n",name);  //如果se的名字在之前所解析到的attr中，那么就解析该对象到se对象中。
      if ((p->ptr = attr_value (p->xml->attr, name))){
        printf("xml_next:find a attribute %s,value:%s\n",name,p->ptr);
        p->state = PARSE_ELEMENT; //如果符合其中的一个属性，则需要对该属性做SE层面的解析。
      }
    } else if (!p->empty) {
      if (start_tag (p, se)){
        printf("xml_next:find a new tag:%s\n",p->xml->name);
        p->state = PARSE_ELEMENT;  //如果解析到一个tag名字，则意味着需要继续解析该tag。
      }
      else if (p->token == XML_INCOMPLETE) {
        printf("xml_next:XML_INCOMPLETE,return 0\n");
        return 0;
      }
    } else if (se->min) p->state = PARSE_INVALID;
    se++; // 从基类开始，逐个子类解析。
  }
  p->se = se - 1;
  return 1;
}


int xml_end (Parser *p, const SchemaElement *se) {
  if (p->empty || end_tag (p, se)) {
    p->empty = 0;
    return 1;
  }
  printf("xml_end:return 0\n");
  return 0;
}

//应该是1表示成功。
int xml_sequence (Parser *p, StackItem *t) {
  const SchemaElement *se = t->se;
  if (start_tag (p, se)) return 1;
  if (p->token <= END_TAG) {
    if (t->count < se->min) p->state = PARSE_INVALID;
    else p->state++;
  }
  return 0;
}

//
void parse_done (Parser *p) {
  p->ptr = p->xml->data;
  p->xml->content = NULL;
}

/*

*/
void xml_rebuffer (Parser *p, char *data, int length) {
  XmlParser *xml = p->xml;
  if (xml->content) {
    int offset = xml->content - data; //这应该是一个负值。
    xml->content = data;  //content 重新指向新的数据
    xml->data -= offset;  /* data指针重新定位到以传入的 data 指针开始的地方 */
  } else xml->data = data;
}

const ParserDriver xml_parser = { xml_start, xml_next, xml_end, xml_sequence,parse_value, parse_text_value, parse_done, xml_rebuffer };

/*XML parser 的初始化*/
void parse_init (Parser *p, const Schema *schema, char *data) {
  XmlParser *xml = p->xml;
  memset (p, 0, sizeof (Parser));
  p->xml = xml ? xml : calloc (1, sizeof (XmlParser));  //如果xml是空的，那么新申请一个
  xml_init (p->xml, data);
  p->schema = schema;
  p->driver = &xml_parser;
  p->need_token = 1;
}

#endif
