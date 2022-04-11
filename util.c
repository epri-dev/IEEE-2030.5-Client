// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup util Utility

    Provides base layer idiomatic C macros and functions.
    @{
*/

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define min(x, y) (((x) < (y))? x : y)
#define max(x, y) (((x) < (y))? y : x)
#define ok(x) if (!(x)) return NULL
#define ok_v(x, value) if (!(x)) return value
#define streq(a,b) (strcmp (a, b) == 0)
#define in_range(x, a, b) ((x) >= a && (x) <= b)
#define alpha(c) (in_range (c, 'a', 'z') || in_range (c, 'A', 'Z'))
#define digit(c) in_range (c, '0', '9')
#define ws(c) (c == '\n' || c == ' ' || c == '\t' || c == '\r') // 判断是否是空白字符
#define only(x) (*trim (x) == '\0') /*判断一个字符串是否仅仅存在\r\n\t和空格，如果是则返回true*/
#define struct_field(type, any, name) ((type *)any)->name
#define type_alloc(type) calloc (1, sizeof (type))
#define number_q(x, data) (digit (*(data))? number (x, data) : data)

int bit_count (uint32_t x);
int string_index (const char *s, const char *const *ss, int n);
char *trim (char *data);
char *to_lower (char *s);
char *number (int *x, char *data);
char *number64 (uint64_t *x, char *data);

/** @} */

#ifndef HEADER_ONLY

int bit_count (uint32_t x) {
  int n = 0;
  while (x) x >>= 1, n++;
  return n;
}

//给出一个字符串在一个字符串列表中的index
int string_index (const char *s, const char *const *ss, int n) {
  int i;
  for (i = 0; i < n; i++) if (streq (s, ss[i])) break;
  return i;
}

char *trim (char *data) {
  while (ws (*data)) data++;  //跳过'\n','\t','空格','\r'这四种字符，找到可以显示的字符(ASCII)
  return data;
}

char *to_lower (char *s) {
  char *t = s;
  while (*t) *t = tolower (*t), t++;
  return s;
}

//将一个表示10进制数值的字符，转换成一个数值。
//如果做了哪怕一次转换，最后返回的是data指针，同时执行了*x=y这个操作。否则返回NULL。
char *number (int *x, char *data) {
  int y = 0, n = 0;
  while (digit (*data))     //连续执行，直到遇到一个不是表示10进制的字符。
    y = y * 10 + (*data++ - '0'), n++;
  return n > 0 ? *x = y, data : NULL;
}


/*将一个表示一个数字的文本转换成一个10进制数字
参数
x：返回的1进制数字
data：传入的文本的首地址
*/
char *number64 (uint64_t *x, char *data) {
  uint64_t y = 0;
  int n = 0;
  while (digit (*data))
    y = y * 10 + (*data++ - '0'), n++;
  return n > 0 ? *x = y, data : NULL; /*将数字通过x来返回，并且将执行了读取之后的文本位置返回*/
}

#endif


