// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include "utf8.c"

#ifndef HEADER_ONLY

#include <stdlib.h>
#include <string.h>

#define MAX_ATTRIBUTE 16

typedef struct _XmlParser {
  char *name, *content, *data;  
  /*
  name指的是当前token的名字。    
  content 和 data 指向所要解析的数据（文本），data是一个在解析过程中向后移动的指针，content是首地址。
  */
  int state, token, length;     // state : 当前解析到哪一步了。 token:指的是类型，由TokenType这个类型来指定。
  char *attr[MAX_ATTRIBUTE * 2];//记录这一个tag的所有属性的数组，存放的是 key - value 的指针值。
} XmlParser;

enum TokenType {START_TAG, EMPTY_TAG, END_TAG, XML_DECL, XML_PI, XML_TEXT,
                XML_NONE, XML_INCOMPLETE, XML_INVALID
               };
/*含义解释
START_TAG:一个表示一段内容的开始的 tag ，可能包含了属性。

XML_DECL:
The XML declaration is a processing instruction that identifies the document as being XML.
All XML documents should begin with an XML declaration.

Example:<?xml version="1.0" encoding="UTF-8" standalone="no" ?>


XML_PI:


*/
//从attr列表中获取到于name相匹配的属性值。如果找不到该name，那么返回NULL。
void *attr_value (char **attr, const char *name) {
  int i;
  for (i = 0; i < MAX_ATTRIBUTE * 2; i += 2)
    if (attr[i]) {
      if (streq (attr[i], name)) return attr[i + 1];
    } else return NULL;
  return NULL;
}

/*判断一个字符是否适合作为一个name的开始字符？？*/
int name_start (int c) {
  return alpha (c) || c == ':' || c == '_'
         || in_range (c, 0xc0, 0xd6) || in_range (c, 0xd8, 0xf6)
         || in_range (c, 0xf8, 0x2ff) || in_range (c, 0x370, 0x37d)
         || in_range (c, 0x37f, 0x1fff) || in_range (c, 0x200c, 0x200d)
         || in_range (c, 0x2070, 0x218f) || in_range (c, 0x2c00, 0x2fef)
         || in_range (c, 0x3001, 0xd7ff) || in_range (c, 0xf900, 0xfdcf)
         || in_range (c, 0xfdf0, 0xfffd) || in_range (c, 0x10000, 0xeffff);
}


/* 判断一个名字是否适合作为一个name中的字符 */
int name_char (int c) { /*空格、等号没有包含在内*/
  return c == '-' || c == '.' || digit (c)
         || name_start (c) || c == 0xb7
         || in_range (c, 0x300, 0x36f) || in_range (c, 0x203f, 0x2040);
}


//获取到一个tag中的名字部分的最后一个字符的位置，比如   <TimeLink href="/dcap/tm"/>中，TimeLink就是name。
char *xml_name (char *data) {
  int first = 1, c;
  char *next;
  while (next = utf8_char (&c, data)) {
    if (first) {
      first = 0;
      ok (name_start (c));  //如果首个字母不符合要求，则直接返回NULL。
    } else if (!name_char (c)) return data; //如果遇到空格或者等号或者无法识别的内容，则返回。
    data = next;
  }
  return NULL;
}

//判断是不是能够正常用于xml的字符。
int xml_char (int c) {  /*空格也是正常的字符 0x20*/
  return ws (c) || in_range (c, 0x20, 0xd7ff)
         || in_range (c, 0xe000, 0xfffd)
         || in_range (c, 0x10000, 0x10ffff);
}

//看起来像是为了“得到一个字符对0字符”的“基准”。
int hex_digit (int c) {
  if (digit (c)) return '0';
  if (in_range (c, 'a', 'f')) return 'a' - 10;
  if (in_range (c, 'A', 'F')) return 'A' - 10;
  return 0;
}

//看起来像是将一个16进制字符串转换成一个16进制数字。
char *hex_number (int *x, char *data) {
  int c, d, y = 0, n = 0;
  while (c = hex_digit (d = *data))     //直到遇到不是16进制数字字符的字符。
    y = (y << 4) | (d - c), n++, data++;
  return n > 0 ? *x = y, data : NULL;   //最后将data指向的最后字符地址传回。
}
/*

字符    表示
  <	  &lt;
  >	  &gt;
  &	  &amp;
  "	  &quot;
  '	  &apos;
  
将一个经过了转义的字符串，转换成原来的内涵。

*/

char *xml_reference (char **ptr, char *data) {
  const char *const refs[] = {"amp", "lt", "gt", "quot", "apos"};
  const char entity[] = {'&', '<', '>', '\"', '\''};
  char *end = strchr (data, ';');
  int ref = 0, i;
  if (!end) return NULL;
  *end++ = '\0';  //++优先级高于*。 所以是先将";"替换成'\0'然后再将end往后移动一个字节。
  if (*data == '#') {// "&#nnnn;" 表示一个10进制数字，"&#xhhhh;" 表示16进制数字。详见 https://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references 。
    data++;
    data = *data == 'x' ? hex_number (&ref, data + 1)
           : number (&ref, data); //如果#符号后面跟随的是x，则表示一个16进制数字，否则是10进制数字。然后将字符串转换成实际的数字。=号的优先级最低。
    return data && *data == '\0' ?
           *ptr = utf8_encode (*ptr, ref), end : NULL;
  }
  i = string_index (data, refs, 5);
  return i < 5 ? *(*ptr)++ = entity[i], end : NULL; //将用refs中表示的字符转换成为实际含义的字符。
}


/*如果end字符在data中，则在end字符处截断并且返回？？*/
char *token_end (char *data, char *end, int n) {
  char *next;
  if ((next = strstr (data, end))) {
    *next = '\0';
    return next + n;
  }
  return NULL;
}


/*实例：<EndDeviceListLink all="1" href="/edev"/>  
属性的值，都是用双引号或者单引号包含起来。
函数作用：将一个以双引号或者单引号开头的字符串放到value数组中，然后在这个函数中对这个字符串执行解析，
得到“属性”的有效部分，最后指针指向处理过了的字符串之后的位置。
*/
char *att_value (char *value) {
  int c, q = *value++;
  char *data = value; // data 的值已经是指向了双引号或者单引号之后的首个字符位置 （ 无论是可见字符还是不可见字符 ）
  ok (q == '"' || q == '\'');
  while (c = *data++) {
    switch (c) {
    case '\0':
    case '<':
      return NULL;
    case '&':
      ok (data = xml_reference (&value, data));
      break;
    default:
      if (c == q) return *value = '\0', data;
    } *value++ = c;
  }
  return NULL;
}


/*
得到在"="号之后的字符串开始地址（有效的可显示字符），相当于跳过等号。
*/
char *xml_eq (char *data) {
  char *eq = trim (data);
  return *eq == '=' ? trim (eq + 1) : NULL;
}

/*attr是一个存储属性的 key 和 value 的数组，这里将全部的属性都解析出来。*/
char *xml_attributes (char **attr, char *data) {
  char *next, *end;
  int i = 0;
  data = trim (data);
  memset (attr, 0, sizeof(char *)*MAX_ATTRIBUTE * 2); //每隔单元中放的都是指针。所以如果遇到0的，就表示到此结束。所以这里没有一个专门用于表示数组长度的单元。
  while (end = xml_name (data)) {
    char *value = xml_eq (end); //跳过等号。
    if (value && (next = att_value (value++))) {
      *end = '\0';  //将这个属性的名字部分截断
      attr[i] = data; //存储的是地址。
      attr[i + 1] = value;  //存储的都是地址
      i = (i + 2) % (2 * MAX_ATTRIBUTE);  //保证不会溢出。但是如果过长的话导致前面几个会被覆盖掉。
    } else return NULL;
    data = trim (next); //跳过空白字符，然后指向下一个属性的 key 即名字开始的地方
  }
  return data;
}

/*这里PI的意思就是： processing instruction ， 处理指令。
例如：
<?xml-stylesheet type="text/css" href="1.css"?>

*/
int xml_pi (XmlParser *p, char *data) {
  char *end;
  p->name = data;
  ok_v (data = end = xml_name (data), 0); //如果data为空，则返回0。
  if (ws (*data)) { /*如果遇到了非显示字符比如空格，则判定为DECL*/
    *data = '\0';
    if (streq (p->name, "xml")) {
      p->token = XML_DECL;  /*XML Declaration*/
      return only (xml_attributes (p->attr, data + 1));
    }
    p->content = trim (data + 1);
  } else p->content = NULL;
  p->token = XML_PI;  /*否则为PI */
  return 1;
}

/*将这个tag的名字取出，并且解析所有的属性，放到attr数组中。*/
char *xml_tag (XmlParser *p, char *data) {
  char *end;  //如果是类似于 "</EndDeviceList>" 这样的，那么就是一个表示结束的 tag。否则就是表示开始的tag
  p->token = *data == '/' ? data++, END_TAG : START_TAG;  //先判断 *data == '/'，最后才对 p->token 赋值。如果遇到 / 则表示一个tag结束了。比如 <TimeLink href="/dcap/tm"/>
  p->name = data;   //将name的开始部分保存下来。
  ok (data = end = xml_name (data));  //得到这个 tag 的 name 的结尾位置
  if (p->token == START_TAG) {
    if (ws (*data))
      ok (data = xml_attributes (p->attr, data + 1));
    if (*data == '/') data++, p->token = EMPTY_TAG; // 空的tag??  没有内容的 tag ?
  } else data = trim (data);
  ok (*data == '>');
  *end = '\0';
  return data + 1;
}

// scan an xml token, return the token type 

/*
扫描一个token，返回这个token的类型以及解析出这个token中的所有属性。
*/

int xml_token (XmlParser *p) {
  int state = p->state, c;
  char *data = p->data, *next, *text;
  if (p->token == XML_TEXT) text = p->content + p->length;  //看起来像是指向前面一个已经解析了的token的后面开始字符。
  while (c = *data) {
    next = data + 1;
    switch (state) {
    case 0: // initial state
      if (c == '<') state = 2;
      else if (ws (c)) next = trim (next);  //得到去除掉空白字符的字符串
      else {
        p->content = text = data;
        p->token = XML_TEXT;  //直到遇到有效字符，然后切换到下一个解析过程
        state++;  //切换到下面的1步骤
        continue;
      }
      break;
    case 1: // text
      switch (c) {
      case '&': //转义字符，比如 "&???;"，例如，Java<tm>必须写成：<name>Java&lt;tm&gt;</name>。转义字符都是以“&”开头，以“;”结束。
        if (strchr (next, ';')) {
          if (!(next = xml_reference (&text, next)))
            return XML_INVALID;
        } else goto incomplete;
        break;
      case '<': //一个新的tag的开始
        state++;  //转到 case 2
        break;
      default:
        if (next = utf8_char (&c, data)) {
          if (xml_char (c))
            text = utf8_encode (text, c);
          else return XML_INVALID;
        } else goto incomplete;
      }
      break;
    case 2: // "<" (tag)  如果是一个"<"符号，则解析到末尾，将整个"<>"包围的内容全部解析出来，包含这个tag的name，各项属性值。
      switch (c) {
      case '!': //如果是!开头的，则是注释
        state = 4;
        break;
      default:  //否则，是正常的文本。
        if (p->token == XML_TEXT) {
          p->state = 2;
          p->data = data;
          *text = '\0';
          p->length = text - p->content;
          p->token = XML_NONE;
          return XML_TEXT;
        }
        if (c == '?') { //"<?" (processing instruction)
          state++;
          break;
        }
        if (strchr (next, '>'))
          return (p->data = xml_tag (p, data)) ?
                 p->state = 0, p->token : XML_INVALID;  //返回这次解析到的token类型。
        goto incomplete;
      }
      break;
    case 3: // "<?" (processing instruction) 这是一个XML处理指令。处理指令以 <? 开始，以 ?> 结束。<? 后的第一个单词是指令名，如xml, 代表XML声明。
      if (next = token_end (data, "?>", 2))
        return xml_pi (p, data) ? p->data = next, p->token : XML_INVALID;
      goto incomplete;
    case 4:  // "<!" (comment, CDATA)
      switch (c) {
      case '-':
        state++;
        break;
      case '[':
        if ((next = token_end (next, "]]>", 3))) {
          if (streq (data + 1, "CDATA[")) {
            int n = (next - 3) - (data + 7);
            memmove (text, data + 7, n);
            text += n;
            state = 1;
          } else return XML_INVALID;
        } else goto incomplete;
        break;
      default:
        return XML_INVALID;
      }
      break;
    case 5: // "<!-" (comment)
      if (c == '-') state++;
      else return XML_INVALID;
      break;
    case 6: // "<!--" (comment)
      if ((next = token_end (data, "-->", 3))) {
        if (*(next - 4) == '-') return XML_INVALID;
        state = p->token == XML_TEXT ? 1 : 0;
        break;
      }
      goto incomplete;
    }
    data = next;
  }
incomplete:
  if (p->token == XML_TEXT) p->length = text - p->content;
  else p->content = data;
  p->state = state;
  p->data = data;
  return XML_INCOMPLETE;
}

void xml_init (XmlParser *p, char *buffer) {
  p->content = p->data = buffer;  //指向待解析的字符串文本
  p->token = XML_NONE;  //??
  p->state = 0;
}

#endif
