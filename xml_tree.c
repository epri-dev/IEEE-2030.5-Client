// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include "xml_token.c"

enum NodeType {ELEMENT_NODE, TEXT_NODE, PI_NODE};

struct _Element;

typedef struct _Node {
  struct _Node *next;
  struct _Element *parent;
  int type;
} Node;

typedef struct _Attribute {
  struct _Attribute *next;
  char *name, *prefix, *value;
} Attribute;

typedef struct _Element {
  struct _Element *next;
  struct _Element *parent;
  int type;
  struct _Element *first, *last;
  char *name, *prefix;
  Attribute *attr;
} Element;

typedef struct {
  Node *next;
  Element *parent;
  int type;
  char data[];
} Text;

void add_node (Element *parent, void *node) {
  Node *n = node;
  n->next = NULL;
  n->parent = parent;
  if (parent->last) parent->last->next = node;
  else parent->first = node;
  parent->last = node;
}

char *local_name (char **prefix, char *qname) { char *c;
  if (c = strchr (qname, ':')) {
    *c = '\0'; *prefix = qname; return c+1;
  } *prefix = ""; return qname;
}

Element *new_element (Element *parent, char *name, char **attr) {
  Element *e = malloc (sizeof (Element));
  int i; Attribute *tail = NULL, *a;
  e->type = ELEMENT_NODE;
  e->name = local_name (&e->prefix, strdup (name));
  e->attr = NULL;
  e->first = e->last = NULL;
  add_node (parent, e);
  for (i = 0; i < MAX_ATTRIBUTE * 2; i += 2)
    if (attr[i]) { 
      a = malloc (sizeof (Attribute));
      a->next = NULL;
      a->name = local_name (&a->prefix, strdup (attr[i]));
      a->value = strdup (attr[i+1]);
      if (tail) tail->next = a;
      else e->attr = a;
      tail = a;
    } else break;
  return e;
}

void new_text (Element *parent, int type, char *content, int length) {
  Text *t = malloc (sizeof (Text)+length+1);
  t->type = type;
  memcpy (t->data, content, length);
  t->data[length] = '\0';
  add_node (parent, t);
}

void new_content (Element *parent, int type, char *content) {
  new_text (parent, type, content, strlen (content));
}

void free_attr (Attribute *a) {
  Attribute *t;
  while (a) { t = a; a = a->next;
    free (t->name); free (t->value); free (t);
  }
}

void free_node (void *node) {
  Node *n = node, *t; Element *e;
  while (n) { t = n; n = n->next;
    switch (t->type) {
    case ELEMENT_NODE:
      e = (Element *)t;
      free_node (e->first);
      if (e->name) free (e->name);
      if (e->attr) free_attr (e->attr);
    case TEXT_NODE: case PI_NODE: 
      free (t); break;
    }
  }
}

Element *xml_parse (char *data) {
  XmlParser p; Element *e, root; char *prefix, *name;
  memset (&root, 0, sizeof(Element)); e = &root;
  xml_init (&p, data);
  while (1) {
    switch (xml_token (&p)) {
    case START_TAG: e = new_element (e, p.name, p.attr); break;
    case EMPTY_TAG: new_element (e, p.name, p.attr); break;
    case END_TAG:
      name = local_name (&prefix, p.name);
      if (!e->parent || strcmp (name, e->name) != 0
	  || strcmp (prefix, e->prefix) != 0) {
	free_node (root.first);
	return NULL;
      }
      e = e->parent;
      if (!e->parent) {
	return root.first;
      }
      break;
    case XML_DECL: break;
    case XML_PI: new_content (e, PI_NODE, p.content); break;
    case XML_TEXT: new_text (e, TEXT_NODE, p.content, p.length); break;
    default: return NULL;
    }
  }
}

Element *xml_read (char *name) {
  char *buffer = file_read (name, NULL),
    *start = utf8_start (buffer);
  Element *e = xml_parse (start);
  free (buffer); return e;
}

char *xml_attr (Element *e, char *name) {
  Attribute *a;
  foreach (a, e->attr)
    if (strcmp (a->name, name) == 0)
      return a->value;
  return NULL;
}

Element *find_element (Element *e, char *name) {
  foreach (e, e->first) {
    if (e->type == ELEMENT_NODE) { Element *f;
      if (strcmp (e->name, name) == 0) return e;
      if (f = find_element (e, name)) return f;
    }
  } return NULL;
}
