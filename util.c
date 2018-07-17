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
#define ws(c) (c == '\n' || c == ' ' || c == '\t' || c == '\r')
#define only(x) (*trim (x) == '\0')
#define struct_field(type, any, name) ((type *)any)->name
#define type_alloc(type) calloc (1, sizeof (type))
#define number_q(x, data) (digit (*(data))? number (x, data) : data)

int bit_count (uint32_t x);
int string_index (const char *s, const char * const *ss, int n);
char *trim (char *data);
char *to_lower (char *s);
char *number (int *x, char *data);
char *number64 (uint64_t *x, char *data);

/** @} */

#ifndef HEADER_ONLY

int bit_count (uint32_t x) { int n = 0;
  while (x) x >>= 1, n++; return n;
}

int string_index (const char *s, const char * const *ss, int n) { int i;
  for (i = 0; i < n; i++) if (streq (s, ss[i])) break; return i;
}

char *trim (char *data) { while (ws (*data)) data++; return data; }

char *to_lower (char *s) { char *t = s;
  while (*t) *t = tolower (*t), t++; return s;
}

char *number (int *x, char *data) {
  int y = 0, n = 0;
  while (digit (*data)) 
    y = y * 10 + (*data++ - '0'), n++;
  return n > 0? *x = y, data : NULL;
}

char *number64 (uint64_t *x, char *data) {
  uint64_t y = 0; int n = 0;
  while (digit (*data))
    y = y * 10 + (*data++ - '0'), n++;
  return n > 0? *x = y, data : NULL;
}

#endif


