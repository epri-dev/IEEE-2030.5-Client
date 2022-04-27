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
    case S_IFREG:
      return FILE_REGULAR;
    case S_IFDIR:
      return FILE_DIR;
    }
  }
  return FILE_NONE;
}
/*
struct dirent * readdir(DIR * dir);
函数说明：readdir()返回参数dir 目录流的下个目录进入点。结构dirent 定义如下：

struct dirent
{
    ino_t d_ino; //d_ino 此目录进入点的inode
    ff_t d_off; //d_off 目录文件开头至此目录进入点的位移
    signed short int d_reclen; //d_reclen _name 的长度, 不包含NULL 字符
    unsigned char d_type; //d_type 所指的文件类型 
    har d_name[256];  //d_name 文件名
};

返回值：成功则返回下个目录进入点. 有错误发生或读取到目录文件尾则返回NULL.


*/
void process_dir (const char *name, void *ctx,
                  void (*func) (const char *, void *ctx)) {
  DIR *dp = opendir (name);
  char path[128];
  struct dirent *ep;
  if (!dp) {
    perror ("process_dir");
    exit (0);
  }
  while (ep = readdir (dp))
    if (ep->d_type == DT_REG) { //  DT_REG : A regular file. 常规文件
      sprintf (path, "%s/%s", name, ep->d_name);
      func (path, ctx); //对每一个文件都执行一遍func
    }
  closedir (dp);
}
