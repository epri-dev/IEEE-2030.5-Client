// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdlib.h>

typedef struct _StringTable {
  struct _StringTable *next, *prev;
  int length, index;
  const void *name;
  char *strings[];
} StringTable;

StringTable *find_table (StringTable *t, const void *name) {
  while (t) {
    if (t->name == name) return t;
    t = t->next;
  } return NULL;
}

int find_string (StringTable *t, char *s) { int i;
  for (i = 0; i < t->index; i++)
    if (streq (s, t->strings[i])) return i;
  return -1;
}

StringTable *new_string_table (StringTable *t, const void *name, int length) {
  StringTable *n = malloc (sizeof (StringTable) + sizeof (char *) * length);
  n->name = name; n->length = length; n->index = 0;
  n->prev = NULL; n->next = t;
  if (t) t->prev = n;
  return n;
}

StringTable *add_string (StringTable *head, StringTable *t, char *s) {
  if (t && t->index == t->length) {
    t->length <<= 1;
    t = realloc (t, sizeof (StringTable) + sizeof (char *) * t->length);
    if (t->prev) t->prev->next = t; else head = t;
    if (t->next) t->next->prev = t;
  }
  t->strings[t->index++] = s;
  return head;
}

void free_tables (StringTable *t) {
  while (t) {
    StringTable *n = t->next;
    free (t); t = n;
  }
}
