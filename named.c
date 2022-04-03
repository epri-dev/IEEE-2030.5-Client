
typedef struct _Named {
  struct _Named *next;
  char *name;
  void *data;
} Named;        //就是name - data的缩写。

void *find_by_name (void *l, char *name);
void *get_by_name (void *l, char *name, int size);
Named *named_insert (Named *l, char *name, void *data);
void *_named_insert_sorted (void *h, void *x);
void *named_insert_sorted (void *head, char *name, void *data);
int find_index_by_name (void *l, char *name);

#ifndef HEADER_ONLY

//在Named list中根据名字找到对应的named实体
void *find_by_name (void *l, char *name) {
  Named *list = l;
  while (list) {
    if (list->name && strcmp (list->name, name) == 0) return list;
    list = list->next;
  }
  return NULL;
}

//如果在Named list中没有找到对应的实体，那么就新建一个，且放在原来的队列的首位。
//Named *list - 表示list包含的值为一个Named变量的地址
//Named **list 表示list变量包含了“一个包含了 指向某一个Named变量的地址 的变量”的地址。
//void *l这用的用法，要求输入的参数是一个“取一个指针变量的”地址。

void *get_by_name (void *l, char *name, int size) {
  Named **list = l, *n = find_by_name (*list, name);
  if (!n) {
    n = calloc (1, size);
    n->name = strdup (name);
    n->next = *list;
    *list = n;
  }
  return n;
}


//在原先的队列的头部插入一个新 named 元素。
Named *named_insert (Named *l, char *name, void *data) {
  Named *n = malloc (sizeof (Named));
  n->next = l;
  n->name = name;
  n->data = data;
  return n;
}

//按照name字符串的值，以升序方式插入一个新的named元素。
void *_named_insert_sorted (void *h, void *x) {
  Named *prev = NULL, *head = h, *n = x, *l = head;
  while (l && (strcmp (n->name, l->name) > 0)) {//大写字母的ascii的值比小写字母大
    prev = l;
    l = l->next;      //新插入的如果比后面的那个更大，则继续往后挪动。
  }
  if (prev) {
    prev->next = n;
    n->next = l;
  } else {
    n->next = head;
    head = n;
  }
  return head;
}

void *named_insert_sorted (void *head, char *name, void *data) {
  Named *n = named_insert (NULL, name, data);
  return _named_insert_sorted (head, n);
}

int find_index_by_name (void *l, char *name) {
  Named *list = (Named *)l;
  int i = 0;
  while (list) {
    if (list->name && strcmp (list->name, name) == 0) return i;
    list = list->next;
    i++;
  }
  return -1;
}

#endif
