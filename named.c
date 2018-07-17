
typedef struct _Named {
  struct _Named *next;
  char *name;
  void *data;
} Named;

void *find_by_name (void *l, char *name);
void *get_by_name (void *l, char *name, int size);
Named *named_insert (Named *l, char *name, void *data);
void *_named_insert_sorted (void *h, void *x);
void *named_insert_sorted (void *head, char *name, void *data);
int find_index_by_name (void *l, char *name);

#ifndef HEADER_ONLY

void *find_by_name (void *l, char *name) {
  Named *list = l;
  while (list) {
    if (list->name && strcmp (list->name, name) == 0) return list;
    list = list->next;
  } return NULL;
}

void *get_by_name (void *l, char *name, int size) {
  Named **list = l, *n = find_by_name (*list, name);
  if (!n) {
    n = calloc (1, size);
    n->name = strdup (name);
    n->next = *list; *list = n;
  } return n;
}

Named *named_insert (Named *l, char *name, void *data) {
  Named *n = malloc (sizeof (Named));
  n->next = l;
  n->name = name;
  n->data = data;
  return n;
}

void *_named_insert_sorted (void *h, void *x) {
  Named *prev = NULL, *head = h, *n = x, *l = head;
  while (l && (strcmp (n->name, l->name) > 0)) {
    prev = l; l = l->next;
  }
  if (prev) { prev->next = n; n->next = l; }
  else { n->next = head; head = n; }
  return head;
}

void *named_insert_sorted (void *head, char *name, void *data) {
  Named *n = named_insert (NULL, name, data);
  return _named_insert_sorted (head, n);
}

int find_index_by_name (void *l, char *name) {
  Named *list = (Named *)l; int i = 0;
  while (list) {
    if (list->name && strcmp (list->name, name) == 0) return i;
    list = list->next; i++;
  } return -1;
}

#endif
