// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HEADER_ONLY

char *file_read (const char *name, int *length) {
  FILE *f = fopen (name, "rb");
  char *buffer; int end;
  if (!f) { perror (name); exit (0); }
  fseek (f, 0, SEEK_END);
  end = ftell (f);
  buffer = malloc (end+1);
  fseek (f, 0, SEEK_SET);
  fread (buffer, end, 1, f);
  fclose (f);
  buffer[end] = '\0';
  if (length) *length = end;
  return buffer;
}

char *break_line (char *buffer) {
  static char *line; char *start, *end;
  if (buffer) line = buffer;
  start = line;
  end = strchr (line, '\n');
  if (end) {
    line = end+1;
    if (*(end-1) == '\r') end--;
    *end = '\0'; return start;
  } line = start + strlen (start);
  return line == start ? NULL : start;
}

int empty_line (char *s) { int c;
  while ((c = *s++) != '\0')
    if (!(c == ' ' || c == '\t')) return 0;
  return 1;
}

void process_buffer (char *buffer, void (*func) (int n, char *)) {
  char *line = break_line (buffer); int n = 0;
  while (line) {
    if (!empty_line (line)) func (n, line);
    n++; line = break_line (NULL);
  }
}

void process_file (const char *name, void (*func) (int n, char *)) {
  char *buffer = file_read (name, NULL);
  process_buffer (buffer, func);
  free (buffer);
}

#endif
