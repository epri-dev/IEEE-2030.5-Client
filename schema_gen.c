// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdio.h>
#include <stdarg.h>

#include "file.c"
#include "util.c"
#include "list.c"
#include "named.c"
#include "xml_tree.c"
#include "schema.c"
#include "list_util.c"

typedef struct _Flag {
  struct _Flag *next;
  char *name;
  List *types;
  char id, bit, bool, min, count;
} Flag;

struct _SchemaType;

typedef struct {
  char *name, *type, *desc;
  Flag *flag; int prim;  
} Field;

enum ParticleType {PartElement, PartAll, PartChoice, PartSequence,
		   PartGroup, PartAny};

typedef struct _Particle {
  struct _Particle *next;
  int kind, min, max; char *ref;
  union {
    struct {
      Field field;
      List *subgroup;
      struct _SchemaType *st;
    }; // element
    struct _Particle *child; // group choice sequence
  };
} Particle;

typedef struct _AttrDecl {
  Field field;
  unsigned optional : 1;
} AttrDecl;

typedef struct _TableEntry {
  struct _TableEntry *next;
  char *name;
  char *type;
  SchemaElement *se;
} TableEntry;

enum TypeKind {AtomicType, ListType, UnionType, ComplexType};

typedef struct _SchemaType {
  struct _SchemaType *next;
  char *name, *desc;
  int kind; char *base;
  int length, minLength, maxLength;
  List *attr;
  Particle *part;
  struct _SchemaType *child;
  int index;
  unsigned restriction : 1;
  unsigned has_flags : 1;
  List *flags, *pre; void *post;
  int base_bits, bits;
  TableEntry *entries;
  List *deps;
} SchemaType;

typedef struct _ElementDecl {
  struct _ElementDecl *next;
  char *name;
  char *type;
  SchemaType *st;
} ElementDecl;

typedef struct {
  SchemaType *types;
  AttrDecl *attrs;
  ElementDecl *elements;
  char *targetNamespace;
  unsigned attributeFormDefault : 1;
  unsigned elementFormDefault : 1;
} SchemaDoc;

const char * const xs_names[] =
  {"xs:string", "xs:boolean", "xs:hexBinary", "xs:anyURI",
   "xs:long", "xs:int", "xs:short", "xs:byte",
   "xs:unsignedLong", "xs:unsignedInt", "xs:unsignedShort",
   "xs:unsignedByte"};

FILE *file;

int print (char *format, ...) {
  va_list args; int n;
  va_start (args, format);
  n = vfprintf (file, format, args);
  va_end (args);
  return n; 
}

#include "c_gen.c"
#include "doc_gen.c"
//#include "java_gen.c"

typedef struct _Edge {
  struct _Edge *next;
  void *a, *b;
} Edge;

typedef struct _Graph {
  List *start;
  Edge *edges;
} Graph;

void add_edge (Graph *g, void *a, void *b) {
  Edge *e = malloc (sizeof (Edge));
  e->next = g->edges; e->a = a; e->b = b;
  g->edges = e;
}

void add_dep (Graph *g, SchemaType *t, char *name, SchemaType *head) {
  if (strncmp (name, "xs:", 3) != 0)
    add_edge (g, t, find_by_name (head, name));
}

void add_attribute_dep (Graph *graph, SchemaType *t,
			List *list, SchemaType *head) {
  List *l; 
  foreach (l, list) {
    AttrDecl *a = l->data;
    add_dep (graph, t, a->field.type, head);
  }
}

List *add_unique (List *head, char *name) {
  if (!find_by_name (head, name))
    return list_insert (head, name);
  return head;
}

void add_particle_dep (Graph *graph, SchemaType *t,
		       Particle *part, SchemaType *root) {
  Particle *p;
  foreach (p, part) {
    if (p->kind == PartElement && p->field.type) {
      add_dep (graph, t, p->field.type, root);
      t->deps = add_unique (t->deps, p->field.type); 
    } else add_particle_dep (graph, t, p->child, root);
  }			   
}

// build type dependency graph
void build_graph (Graph *graph, SchemaType *head) {
  SchemaType *t; Edge *ed;
  memset (graph, 0, sizeof (Graph));
  foreach (t, head) {
    // add type to the list of potential start nodes
    graph->start = list_insert (graph->start, t);
    if (t->base) add_dep (graph, t, t->base, head);
    add_particle_dep (graph, t, t->part, head);
    add_attribute_dep (graph, t, t->attr, head);
  }
  // remove base types
  foreach (ed, graph->edges)
    graph->start = list_delete (graph->start, ed->b);
}

List *top_sort (Graph *graph) {
  List *s = graph->start, *l = NULL, *t;
  while (s) {
    Edge *e = graph->edges, *prev = NULL;
    void *n = s->data;
    t = s; s = s->next;
    link_insert (l, t);
    while (e) {
      if (e->a == n) {
	Edge *f = e; void *m = e->b;
	int in_use = 0; e = e->next;
	if (prev) prev->next = e;
	else graph->edges = e;
	free (f);
	foreach (f, graph->edges)
	  if (f->b == m) { in_use = 1; break; }
	if (!in_use) s = list_insert (s, m); 
      } else { prev = e; e = e->next; }
    }
  }
  return l;
}

int attr_eq (Element *e, char *name, char *value) {
  char *attr = xml_attr (e, name);
  return attr && streq (attr, value);
}

void add_namespace (SchemaDoc *doc, char *ns, char *prefix) {
}

int get_int_value (Element *e, char *name, int ret) {
  Element *f = find_element (e, name);
  if (f) { char *value = xml_attr (f, "value");
    if (value) return atoi (value);
  } return ret;
}

char *get_documentation (Element *e) { Element *f;
  if (f = find_element (e, "documentation")) {
    Text *t = (Text *)f->first;
    return t->data;
  } return NULL;
}

SchemaType *simple_type (SchemaDoc *doc, Element *e) {
  SchemaType *t = type_alloc (SchemaType), *u;
  Element *f, *g;
  t->name = xml_attr (e, "name");
  t->desc = get_documentation (e);
  if (f = find_element (e, "restriction")) {
    t->kind = AtomicType;
    t->base = xml_attr (f, "base");
    t->length = get_int_value (f, "length", -1);
    t->minLength = get_int_value (f, "minLength", -1);
    t->maxLength = get_int_value (f, "maxLength", -1);
    if (g = find_element (f, "simpleType"))
      t->child = simple_type (doc, g);
  } else if (f = find_element (e, "list")) {
    t->kind = ListType;
    t->base = xml_attr (f, "itemType");
    if (g = find_element (f, "simpleType"))
      t->child = simple_type (doc, g);
  } else if (f = find_element (e, "union")) {
    t->kind = UnionType;
    t->base = xml_attr (f, "memberTypes");
    foreach (g, (Element *)e->first) {
      if (streq (g->name, "simpleType")) {
	u = simple_type (doc, g);
	u->next = t->child; t->child = u;
      }
    }
  }
  t->next = doc->types; doc->types = t;
  return t;
}

List *type_attr (SchemaType *t, Element *e) {
  Element *f; List *head = NULL;
  foreach (f, (Element *)e->first) {   
    if (streq (f->name, "attribute")) {
      AttrDecl *a = type_alloc (AttrDecl);
      char *value;
      a->field.name = xml_attr (f, "name");
      a->field.type = xml_attr (f, "type");
      a->field.desc = get_documentation (f);
      value = xml_attr (f, "use");
      a->optional = value? streq (value, "optional") : 1;
      head = list_insert (head, a);
    } else if (streq (f->name, "attributeGroup")) {
    }
  } return list_reverse (head);
}

void part_attr (Particle *p, Element *e) {
  char *value = xml_attr (e, "minOccurs");
  p->min = value? atoi (value) : 1;
  value = xml_attr (e, "maxOccurs");
  p->max = value? (streq (value, "unbounded")? -1 : atoi (value)) : 1;
}

SchemaType *complex_type (SchemaDoc *doc, Element *e) {
  SchemaType *t; Element *f, *g; Particle *p, *last = NULL;
  if (f = find_element (e, "simpleContent")) {
    if (find_element (f, "restriction")) {
      t = simple_type (doc, f);
    } else if (g = find_element (f, "extension")) {
      t = type_alloc (SchemaType);
      t->desc = get_documentation (e);
      t->kind = AtomicType;
      t->base = xml_attr (g, "base");
      t->attr = type_attr (t, g);
    }
    t->name = xml_attr (e, "name");
  } else {
    t = type_alloc (SchemaType);
    t->desc = get_documentation (e);
    t->name = xml_attr (e, "name");    
    t->kind = ComplexType;
    if ((f = find_element (e, "simpleContent"))
	|| (f = find_element (e, "complexContent"))) {
      if (g = find_element (f, "restriction"))
	t->restriction = 1;
      else if (g = find_element (f, "extension"));
      t->base = xml_attr (g, "base");
    } else g = e;
    t->attr = type_attr (t, g);
    if (f = find_element (g, "group")) {
    } else if (f = find_element (g, "all")) {
    } else if (f = find_element (g, "choice")) {
    } else if (f = find_element (g, "sequence")) {
      p = t->part = type_alloc (Particle);
      p->kind = PartSequence;
      part_attr (p, f);
      foreach (e, f->first) {
	if (streq (e->name, "element")) {
	  Particle *q = type_alloc (Particle);
	  q->kind = PartElement;
	  q->field.name = xml_attr (e, "name");
	  q->field.type = xml_attr (e, "type");
	  q->field.desc = get_documentation (e);
	  part_attr (q, e);
	  if (g = find_element (e, "simpleType")) {
	    q->st = simple_type (doc, g);
	  } else if (g = find_element (e, "complexType")) {
	    q->st = complex_type (doc, g);
	  }
	  if (last) last->next = q;
	  else p->child = q; last = q;
	}
      }
    }
  }
  t->next = doc->types; doc->types = t;
  return t;
}

void element_decl (SchemaDoc *doc, Element *e) {
  ElementDecl *ed = type_alloc (ElementDecl);
  ed->name = xml_attr (e, "name");
  ed->type = xml_attr (e, "type");
  doc->elements = _named_insert_sorted (doc->elements, ed);
}

void process_schema (SchemaDoc *doc, Element *root) {
  Element *e; char *prefix; Attribute *a;
  foreach (a, root->attr) {
    if (streq (a->name, "attributeFormDefault"))
      doc->attributeFormDefault = streq (a->value, "qualified");
    else if (streq (a->name, "elementFormDefault"))
      doc->elementFormDefault = streq (a->value, "qualified");
    else if (streq (a->name, "targetNamespace"))
      doc->targetNamespace = a->value;
    else if (strncmp (a->name, "xmlns", 5) == 0) {
      char *prefix = strchr (a->name, ':');
      if (prefix) { *prefix = '\0'; prefix++; }
      add_namespace (doc, a->value, prefix);
    }
  }
  foreach (e, (Element *)root->first) {
    char *name = local_name (&prefix, e->name);
    if (streq (name, "import")) {
    } else if (streq (name, "simpleType")) {
      simple_type (doc, e);
    } else if (streq (name, "complexType")) {
      complex_type (doc, e);
    } else if (streq (name, "group")) {
    } else if (streq (name, "attributeGroup")) {
    } else if (streq (name, "element")) {
      element_decl (doc, e);
    } else if (streq (name, "attribute")) {
    }
  }
}

int main () {
  Graph graph; List *sorted;
  SchemaDoc doc = {0};
  Named *se_list = load_list ("se_list.txt"); 
  Element *root = xml_read ("sep.xsd");
  process_schema (&doc, root);
  build_graph (&graph, doc.types);
  sorted = top_sort (&graph);
  compute_flags (sorted, doc.types);
  file = fopen ("se_types.h", "wb+");
  print_types (sorted, doc.types);
  print_symbols (doc.elements);
  fclose (file);
  file = fopen ("se_types_doc.h", "wb+");
  print_types_doc (sorted, doc.types);
  fclose (file);
  file = fopen ("se_schema.c", "wb+");
  print_schema (sorted, &doc); fclose (file);
  file = fopen ("se_list.c", "wb+");  
  print_list_info (se_list, doc.types); fclose (file);
  // java_gen (&doc);
  return 0;
}
