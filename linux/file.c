// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int file_type (const char *name) {
  struct stat sb;
  if (stat (name, &sb) == 0) {
    switch (sb.st_mode & S_IFMT) {
    case S_IFREG: return FILE_REGULAR;
    case S_IFDIR: return FILE_DIR;
    }
  } return FILE_NONE;
}

void process_dir (const char *name, void *ctx,
		  void (*func) (const char *, void *ctx)) {
  DIR *dp = opendir (name); char path[128];
  struct dirent *ep;
  if (!dp) { perror ("process_dir"); exit (0); }
  while (ep = readdir (dp))
    if (ep->d_type == DT_REG) {
      sprintf (path, "%s/%s", name, ep->d_name);
      func (path, ctx);
    }
  closedir (dp);
}
