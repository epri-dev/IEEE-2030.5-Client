// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

char *utf8_char (int *code, char *data);
char *utf8_encode (char *data, unsigned int code);
char *utf8_start (char *data);

#ifndef HEADER_ONLY

#include <stdint.h>

#define UTF_REJECT 99

const char utf_table[] = {
  // map utf byte (c0..ff) to class
  1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3,  8, 6, 6, 6, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  // state transition
  99, 99, 9, (9 + 4), (9 + 8), (9 + 12), (9 + 16), (9 + 20), (9 + 24), // state 0
  0, 0, 0, 0,     // state 1 (c2..df)
  9, 9, 9, 9,     // state 2 (e1..ec, ee..ef)
  9, 9, 99, 99,   // state 3 (ed)
  17, 99, 99, 99, // state 4 (f4)
  17, 17, 17, 17, // state 5 (f1..f3)
  99, 99, 9, 9,   // state 6 (e0)
  99, 17, 17, 17  // state 7 (f0)
};

/*将存放在UTF8格式的数据串data中的字符解析出来，放到code开始的地址上去。
对于ASCII字符码，相当于不用解析，data是什么code就是什么。
返回的是经过了解析之后的data地址。
*/
char *utf8_char (int *code, char *data) {
  int state = 0, type;
  do {
    uint8_t c = (uint8_t) * data++;
    if (c < 128) {
      if (state) return NULL;
      *code = c;
      break;
    }
    if (state) {
      if (c & 0x40) return NULL;
      type = (c >> 4) & 3;
      *code = (c & 0x3f) | (*code << 6);
      state = utf_table[64 + state + type];
    } else if (c & 0x40) {
      type = utf_table[c ^ 0xc0];
      *code = (0xff >> type) & c;
      state = utf_table[64 + type];
    } else return NULL;
    if (state == UTF_REJECT) return NULL;
  } while (state);
  return data;
}

/*
UTF-8是一种变长字节编码方式。对于某一个字符的UTF-8编码，如果只有一个字节则其最高二进制位为0；
如果是多字节，其第一个字节从最高位开始，连续的二进制位值为1的个数决定了其编码的位数，其余各字节均以10开头。
UTF-8最多可用到6个字节。

1字节 0xxxxxxx
2字节 110xxxxx 10xxxxxx
3字节 1110xxxx 10xxxxxx 10xxxxxx
4字节 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
5字节 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx


函数功能：将一个code数据，转换成为UTF8编码，结果放到data指示的地址往后的位置上去。

*/
char *utf8_encode (char *data, unsigned int code) {
  if (code <= 0x7f) { //如果是单字节
    *data++ = code;
    return data;
  }
  if (code <= 0x7ff) {  //2字节
    *data++ = 0xc0 | (code >> 6);
byte_1:
    *data++ = 0x80 | (code & 0x3f);
    return data;
  }
  if (code <= 0xffff) { //3字节
    *data++ = 0xe0 | (code >> 12);
byte_2:
    *data++ = 0x80 | ((code >> 6) & 0x3f);
    goto byte_1;
  }
  if (code <= 0x1ffff) {  //4字节
    *data++ = 0xe0 | (code >> 18);
    *data++ = 0x80 | ((code >> 12) & 0x3f);
    goto byte_2;
  }
  return NULL;
}

int utf8_length (const char *data) {
  int c, length = 0;
  while (c = *data++) {
    if (c & 0x80) {
      if (c & 0x20) {
        if (c & 0x10) data++;
        data++;
      }
      data++;
    }
    length++;
  }
  return length;
}

char *utf8_start (char *data) {
  int c;
  utf8_char (&c, data);
  return c == 0xfeff ? data + 3 : data;
}

#endif
