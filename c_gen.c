const char *c_type (char *type) {
  switch (string_index (type, xs_names, 12) + 1) {
  case XS_STRING:
  case XS_ANY_URI:
    return "char *";
  case XS_UBYTE:
    return "uint8_t";
  case XS_USHORT:
    return "uint16_t";
  case XS_UINT:
    return "uint32_t";
  case XS_ULONG:
    return "uint64_t";
  case XS_BYTE:
    return "int8_t";
  case XS_SHORT:
    return "int16_t";
  case XS_INT:
    return "int32_t";
  case XS_LONG:
    return "int64_t";
  case XS_BOOLEAN:
    return "uint8_t";
  default:
    return NULL;
  }
}

void print_atomic_typedef (SchemaType *t) {
  if (streq (t->base, "xs:hexBinary")) {
    int n = t->maxLength;
    if (n == 1) print ("typedef uint8_t SE_%s_t;\n", t->name);
    else print ("typedef uint8_t SE_%s_t[%d];\n", t->name, n);
  } else if (streq (t->base, "xs:string")) {
    int n = t->maxLength;
    if (n) print ("typedef char SE_%s_t[%d];\n", t->name, n);
    else print ("typedef char *SE_%s_t;\n", t->name);
  } else {
    const char *type = c_type (t->base);
    if (type) print ("typedef %s SE_%s_t;\n", type, t->name);
    else print ("typedef SE_%s_t SE_%s_t;\n", t->base, t->name);
  }
}

void print_field (char *base, char *name, int n) {
  const char *type = c_type (base);
  if (type) print ("  %s %s", type, name);
  else print ("  SE_%s_t %s", base, name);
  if (n == 1) print (";\n", n);
  else print ("[%d];\n", n);
}

void print_particle (Particle *part, SchemaType *head) {
  Particle *p;
  Field *f;
  foreach (p, part) {
    switch (p->kind) {
    case PartElement:
      f = &p->field;
      if (p->max < 0)
        print ("  List *%s;\n", f->name);
      else if (f->prim != XS_BOOLEAN)
        print_field (f->type, f->name, p->max);
      break;
    case PartGroup:
    case PartChoice:
    case PartSequence:
      print_particle (p->child, head);
    case PartAny:
      break;
    }
  }
}

void print_flag (Flag *f, int base) {
  int bit = f->bit;
  if (base && f->bool) print ("// ");
  if (f->bool == 1)
    print ("#define SE_%s_true (1 << %d)\n", f->name, bit);
  else if (f->bool)
    print ("#define SE_%s_true(i) (1 << (%d+i))\n", f->name, bit);
  bit += f->bool;
  if (base && f->count) print ("// ");
  if (f->count == 1 & f->min == 0)
    print ("#define SE_%s_exists (1 << %d)\n", f->name, bit);
  else if (f->count) {
    int mask = ~(-1 << f->count);
    print ("#define SE_%s_count(x) (((se_flags (x) >> %d) & 0x%x) + %d)\n",
           f->name, bit, mask, f->min);
    print ("#define SE_set_%s_count(x, n) "
           "(se_flags(x) |= (((n) - %d) & 0x%x) << %d)\n",
           f->name, f->min, mask, bit);
  }
}

typedef struct _FlagGroup {
  int base;
  List *types;
  List *flags;
} FlagGroup;

List *global_flags = NULL;

void print_flags (SchemaType *t, SchemaType *u, SchemaType *head) {
  List *l;
  Flag *f;
  int base = t != u;
  if (!base)
    foreach (l, global_flags) {
    f = l->data;
    if (find_by_data (f->types, t))
      print_flag (f, 1);
  }
  if (u->base) print_flags (t, find_by_name (head, u->base), head);
  foreach (l, u->pre) {
    f = l->data;
    if ((base && find_by_data (f->types, t)) || !base)
      print_flag (f, base);
  }
  foreach (l, u->flags) print_flag (l->data, base);
  if (u->post) {
    FlagGroup *g = u->post;
    foreach (l, g->flags) print_flag (l->data, g->base);
    g->base = 1;
  }
}

void print_fields (SchemaType *t, SchemaType *head) {
  List *attr;
  SchemaType *u;
  if (t->base && (u = find_by_name (head, t->base)))
    print_fields (u, head);
  if (!t->restriction) {
    foreach (attr, t->attr) {
      AttrDecl *a = attr->data;
      Field *f = &a->field;
      print_field (f->type, f->name, 1);
    }
    print_particle (t->part, head);
  }
}

int has_flags (SchemaType *t, SchemaType *head) {
  if (t->has_flags) return 1;
  while (t->base) {
    t = find_by_name (head, t->base);
    if (t->has_flags) return 1;
  }
  return 0;
}

void print_type (SchemaType *t, SchemaType *head) {
  static int prev = 0;
  switch (t->kind) {
  case ComplexType:
    if (prev) print ("\n");
    prev = 0;
    print_flags (t, t, head);
    print ("typedef struct {\n");
    if (has_flags (t, head))
      print ("  uint32_t _flags;\n");
    print_fields (t, head);
    print ("} SE_%s_t;\n\n", t->name);
    break;
  case AtomicType:
    print_atomic_typedef (t);
    prev = 1;
  }
}

void print_types (List *list, SchemaType *head) {
  List *l;
  print ("// auto-generated by schema_gen\n\n");
  print ("#include <stddef.h>\n");
  print ("#include <stdint.h>\n\n");
  print ("#define se_flags(x) (*(uint32_t *)x)\n");
  print ("#define se_exists(x, f) (se_flags (x) & SE_##f##_exists)\n");
  print ("#define se_clear(x, f) (se_flags (x) &= ~SE_##f##_exists)\n");
  print ("#define se_set(x, f) (se_flags (x) |= SE_##f##_exists)\n");
  print ("#define se_true(x, f) (se_flags (x) & SE_##f##_true)\n");
  print ("#define se_set_true(x, f) (se_flags (x) |= SE_##f##_true)\n");
  print ("#define se_set_false(x, f) (se_flags (x) &= ~SE_##f##_true)\n\n");
  print ("// global flags\n");
  foreach (l, global_flags) print_flag (l->data, 0);
  print ("\n");
  foreach (l, list) print_type (l->data, head);
}

// find the primitive type given a type name
int xs_primitive (char *name, SchemaType *head) {
  int index, n = 0;
  SchemaType *t;
  while ((index = string_index (name, xs_names, 12)) == 12) {
    if (!(t = find_by_name (head, name))) {
      print ("unknown type %s\n", name);
      exit (0);
    }
    if (t->attr || t->part || !t->base) return XS_NULL;
    name = t->base;
    if (!n && t->maxLength > 0) n = t->maxLength;
  }
  return xs_type (index + 1, n);
}

Flag *flags = NULL;

Flag *add_flag (SchemaType *t, char *name, int min, int count, int bool) {
  Flag *f, *g;
  if (g = find_by_name (flags, name)) {
    char other[256];
    int i;
    for (i = 0, f = g; f && i <= g->id; i++) {
      if (f->min == min && f->count == count && f->bool == bool) goto out;
      sprintf (other, "%s%d", name, i);
      f = find_by_name (flags, other);
    }
    name = strdup (other);
    g->id++;
  }
  f = type_alloc (Flag);
  f->next = flags;
  flags = f;
  f->name = name;
  f->min = min;
  f->count = count;
  f->bool = bool;
out:
  f->types = list_insert (f->types, t);
  return f;
}

void attr_flags (SchemaType *t, SchemaType *head) {
  List *attr;
  foreach (attr, t->attr) {
    AttrDecl *a = attr->data;
    Field *f = &a->field;
    f->prim = xs_primitive (f->type, head);
    if (!is_pointer (f->prim)) {
      int bool = f->prim == XS_BOOLEAN;
      if (a->optional || bool)
        f->flag = add_flag (t, f->name, !a->optional, 1, bool);
    }
  }
}

void part_flags (SchemaType *t, Particle *part, SchemaType *head) {
  Particle *p;
  Field *f;
  int count, bool;
  foreach (p, part) {
    switch (p->kind) {
    case PartElement:
      f = &p->field;
      f->prim = xs_primitive (f->type, head);
      if (!is_pointer (f->prim)) {
        bool = f->prim == XS_BOOLEAN && p->max > 0 ? p->max : 0;
        count = (p->max > 0 && p->min < p->max) ?
                bit_count (p->max - p->min) : 0;
        if (count || bool)
          f->flag = add_flag (t, f->name, p->min, count, bool);
      }
      break;
    case PartSequence:
      part_flags (t, p->child, head);
    }
  }
}

// make sure parent types have space for flags of SchemaType
void set_has_flags (SchemaType *t, SchemaType *head) {
  t->has_flags = 1;
  while (t->base) {
    t = find_by_name (head, t->base);
    t->has_flags = 1;
  }
}

// list of ancestor types, most immediate first
List *type_ancestors (SchemaType *t, SchemaType *head) {
  List *p = NULL;
  while (t->base) {
    t = find_by_name (head, t->base);
    p = list_insert (p, t);
  }
  return list_reverse (p);
}

// lowest common ancestor type
SchemaType *type_lca (List *types, SchemaType *head) {
  List *path = type_ancestors (types->data, head), *l;
  foreach (l, types->next) {
    List *p = type_ancestors (l->data, head);
    path = list_intersect (path, p);
  }
  return path ? path->data : NULL;
}

int flag_bits (int bit, List *list) {
  int bits = 0;
  List *l;
  foreach (l, list) {
    Flag *f = l->data;
    f->bit = bit + bits;
    bits += f->bool + f->count;
  }
  return bits;
}

int list_eq (List *a, List *b) {
  while (a) {
    if (!b || a->data != b->data) return 0;
    a = a->next;
    b = b->next;
  }
  return 1;
}

List *add_flag_group (List *groups, Flag *f) {
  FlagGroup *g;
  List *l;
  foreach (l, groups) {
    g = l->data;
    if (list_eq (f->types, g->types)) goto out;
  }
  g = type_alloc (FlagGroup);
  g->types = f->types;
  groups = list_insert (groups, g);
out:
  g->flags = list_insert (g->flags, f);
  return groups;
}

int is_derived (SchemaType *t, SchemaType *u, SchemaType *head) {
  while (t->base) {
    t = find_by_name (head, t->base);
    if (t == u) return 1;
  }
  return 0;
}

List *pre_flag_groups (List *groups, SchemaType *head) {
  List *l, *m, *n, *post = NULL;
  FlagGroup *g;
  foreach (l, groups) {
    g = l->data;
    SchemaType *x;
    List *derived = NULL;
    foreach (m, g->types) {
      SchemaType *t = m->data, *u;
      foreach (u, head) {
        if (u->kind != ComplexType) continue;
        if (is_derived (u, t, head))
          derived = list_insert (derived, u);
      }
    }
    if (derived) {
      if (x = type_lca (g->types, head)) {
        x->pre = list_cat (x->pre, g->flags);
        foreach (m, g->flags) {
          Flag *f = m->data;
          f->types = list_union (f->types, derived);
        }
      } else {
        global_flags = list_cat (global_flags, g->flags);
        foreach (m, g->flags) {
          List *a = NULL;
          Flag *f = m->data;
          foreach (n, f->types)
          a = list_union (a, type_ancestors (n->data, head));
          a = list_union (a, derived);
          f->types = list_union (f->types, a);
        }
      }
    } else post = list_insert (post, g);
  }
  return post;
}

void post_flag_groups (List *groups, SchemaType *head) {
  List *l, *m;
  FlagGroup *g;
  foreach (l, groups) {
    int base_bits = 0;
    g = l->data;
    foreach (m, g->types) {
      SchemaType *t = m->data;
      int bits = t->base_bits + t->bits;
      t->post = g;
      base_bits = max (base_bits, bits);
    }
    flag_bits (base_bits, g->flags);
  }
}

void compute_flags (List *sorted, SchemaType *head) {
  SchemaType *t, *u;
  List *l, *m;
  Flag *f;
  int global_bits;
  List *groups = NULL;
  // collect flags
  foreach (l, sorted) {
    t = l->data;
    if (t->kind == ComplexType
        && !t->restriction) {
      attr_flags (t, head);
      part_flags (t, t->part, head);
    }
  }
  // distribute flags to types
  foreach (f, flags) {
    int n = list_length (f->types);
    if (n == 1) {
      t = f->types->data;
      t->flags = list_insert (t->flags, f);
    } else
      groups = add_flag_group (groups, f);
  }
  groups = pre_flag_groups (groups, head);
  global_bits = flag_bits (0, global_flags);
  foreach (l, sorted) {
    t = l->data;
    if (t->kind != ComplexType) continue;
    if (t->base && (u = find_by_name (head, t->base)))
      t->base_bits = u->base_bits + u->bits;
    else
      foreach (m, global_flags) {
      Flag *f = m->data;
      if (find_by_data (f->types, t)) {
        t->base_bits = global_bits;
        break;
      }
    }
    t->bits = flag_bits (t->base_bits, t->pre);
    t->flags = list_reverse (t->flags);
    t->bits += flag_bits (t->base_bits + t->bits, t->flags);
    if (t->flags || t->base_bits)
      set_has_flags (t, head);
  }
  post_flag_groups (groups, head);
}

void print_xs_type (int type) {
  char *base[] = {"", "XS_STRING", "XS_BOOLEAN", "XS_HEX_BINARY", "XS_ANY_URI",
                  "XS_LONG", "XS_INT", "XS_SHORT", "XS_BYTE", "XS_ULONG",
                  "XS_UINT", "XS_USHORT", "XS_UBYTE"
                 };
  int n = type >> 4;
  type &= 0xf;
  if (n) print ("xs_type(%s,%d)", base[type], n);
  else print (base[type]);
}

int get_index (SchemaType *schema, char *name) {
  SchemaType *s = find_by_name (schema, name);
  return s->index;
}

void print_schema_element (SchemaElement *se, char *parent,
                           char *name, char *type, SchemaType *head) {
  int simple = se->simple || se->attribute;
  print ("  {");
  if (parent && ((simple && se->xs_type != XS_BOOLEAN) || !simple))
    print (".offset=offsetof(SE_%s_t,%s), ", parent, name);
  print (".min=%d, ", se->min);
  if (se->attribute) {
    print (".max=1, .attribute=1, ");
    if ((!se->min && !is_pointer (se->xs_type)) || se->xs_type == XS_BOOLEAN)
      print (".bit=%d, ", se->bit);
  } else {
    if (se->unbounded) print (".unbounded=1, ");
    else if (se->max) print (".max=%d, ", se->max);
    if ((se->min < se->max && !is_pointer (se->xs_type))
        || (se->simple && se->xs_type == XS_BOOLEAN))
      print (".bit=%d, ", se->bit);
  }
  if (simple) {
    if (se->simple) print (".simple=1, ");
    print (".xs_type=");
    print_xs_type (se->xs_type);
  } else print (".index=%d", get_index (head, type));
  if (se->n) print (", .n=%d},\n", se->n);
  else print ("},\n");
}

int compare_attr (void *ea, void *eb) {
  List *la = ea, *lb = eb;
  AttrDecl *a = la->data, *b = lb->data;
  return strcmp (a->field.name, b->field.name);
}

List *collect_attr (SchemaType *t, SchemaType *head) {
  List *attr = list_dup (t->attr);
  char *base = t->base;
  while (base) {
    SchemaType *u = find_by_name (head, base);
    attr = list_cat (attr, list_dup (u->attr));
    base = u->base;
  }
  return quick_sort (attr, compare_attr);
}

List *qnames = NULL;

void insert_qname (char *name) {
  if (!find_by_name (qnames, name))
    qnames = list_insert (qnames, name);
}

TableEntry *te_insert (TableEntry *head, char *name, char *type,
                       SchemaElement *se) {
  TableEntry *te = type_alloc (TableEntry);
  insert_qname (name);
  te->name = name, te->type = type, te->se = se;
  te->next = head;
  return te;
}

/* The number of EXI productions in a given parsing context is determined by
   the number of possible attributes or elements that can occur after the
   minimum attribute or element count is met for the current table entry.
   If the minimal count is equal to the maximal count for the current table
   entry or the minimal count is not met, then the number of productions is 1.
*/
int productions (TableEntry *entries) {
  int n = 0, first = 1;
  TableEntry *e;
  foreach (e, entries) {
    SchemaElement *se = e->se;
    n++;
    if (first && (se->min < se->max || se->unbounded)) first = 0;
    else if (se->min) break;
  }
  if (!e) n++; // reached the end of a sequence
  return n;
}

TableEntry *gen_schema_part (Particle *part, SchemaType *head) {
  TableEntry *entries = NULL;
  SchemaElement *se;
  Particle *p;
  Field *f;
  foreach (p, part) {
    switch (p->kind) {
    case PartElement:
      f = &p->field;
      se = type_alloc (SchemaElement);
      se->min = p->min;
      if (f->flag) se->bit = f->flag->bit;
      if (p->max < 0) se->unbounded = 1;
      else se->max = p->max;
      if (f->prim > 0) {
        se->xs_type = f->prim;
        se->simple = 1;
      } else se->index = get_index (head, f->type);
      entries = te_insert (entries, f->name, f->type, se);
      break;
    case PartGroup:
    case PartChoice:
    case PartSequence:
      entries = list_cat (gen_schema_part (p->child, head), entries);
    case PartAny:
      break;
    }
  }
  return entries;
}

TableEntry *gen_schema_parts (SchemaType *t, SchemaType *head) {
  TableEntry *entries = NULL;
  if (t->base) {
    SchemaType *u = find_by_name (head, t->base);
    entries = list_cat (gen_schema_parts (u, head), entries);
  }
  if (!t->restriction)
    entries = list_cat (gen_schema_part (t->part, head), entries);
  return entries;
}

TableEntry *gen_schema_entries (SchemaType *t, SchemaType *head) {
  TableEntry *entries = NULL;
  List *list = collect_attr (t, head), *l;
  foreach (l, list) {
    AttrDecl *a = l->data;
    Field *f = &a->field;
    SchemaElement *se = type_alloc (SchemaElement);
    se->xs_type = f->prim;
    se->attribute = 1;
    se->max = 1;
    se->min = !a->optional;
    if (f->flag) se->bit = f->flag->bit;
    entries = te_insert (entries, f->name, NULL, se);
  }
  return list_reverse (list_cat (gen_schema_parts (t, head), entries));
}

void gen_schema_elements (List *sorted, int index, SchemaType *schema) {
  List *l;
  foreach (l, sorted) {
    SchemaType *t = l->data;
    if (t->kind == ComplexType) {
      t->index = index;
      t->entries = gen_schema_entries (t, schema);
      if (t->name) index++;
      index += list_length (t->entries);
    }
  }
}

void print_schema (List *sorted, SchemaDoc *doc) {
  int length = list_length (doc->elements);
  TableEntry *te;
  List *s, *q, *names = NULL;
  ElementDecl *e;
  gen_schema_elements (sorted, length, doc->types);
  print ("// auto-generated by schema_gen\n\n");
  print ("const SchemaElement se_elements[] = {\n");
  foreach (e, doc->elements) {
    int prim = xs_primitive (e->type, doc->types);
    print ("  {.min=1, .max=1, ");
    if (prim > 0) {
      print (".simple=1, .xs_type=");
      print_xs_type (prim);
    } else {
      print (".index=%d", get_index (doc->types, e->type));
    }
    print ("}, // %s\n", e->name);
  }
  foreach (s, sorted) {
    SchemaType *t = s->data;
    if (t->kind != ComplexType) continue;
    if (t->name) {
      int base = t->base ? get_index (doc->types, t->base) : 0;
      print ("  // %s (%d)\n", t->name, t->index);
      print ("  {.size=sizeof(SE_%s_t), .index=%d},\n",
             t->name, base);
    }
    foreach (te, t->entries) {
      SchemaElement *se = te->se;
      se->n = productions (te);
      print_schema_element (se, t->name, te->name, te->type, doc->types);
    }
  }
  print ("  {0}\n};\n\n");
  foreach (e, doc->elements) {
    if (q = find_by_name (qnames, e->name)) {
      qnames = list_remove (qnames, q);
      q->next = names;
      names = q;
    } else names = list_insert (names, e->name);
  }

  int print_new_line = 0;
  qnames = list_reverse (list_cat (qnames, names));
  print ("const char * const se_names[] = {");
  foreach (q, qnames) {
    print ("\"%s\", ", q->data);
    if(++print_new_line == 10){
      print_new_line =0;
      print ("\n");
    }
  }
  print ("};\n\n");
  
  print_new_line =0;
  print ("const uint16_t se_ids[] = {");
  foreach (s, sorted) {
    SchemaType *t = s->data;
    if (t->kind != ComplexType) continue;
    if (t->name) print ("0, ");
    foreach (te, t->entries){
      print ("%d, ", find_index_by_name (qnames, te->name));
      if(++print_new_line == 30){
        print_new_line = 0;
        print ("\n");
      }

    }
  }
  print ("};\n\n");
  print ("Schema se_schema = "
         "{\"%s\", \"S1\", %d, se_elements, se_names, se_ids};\n",
         doc->targetNamespace, length);
}

void print_symbols (ElementDecl *elements) {
  ElementDecl *e;
  int i = 0;
  foreach (e, elements)
  print ("#define SE_%s %d\n", e->name, i++);
  print ("\n");
}



//看起来像是导入了一个表格，按照字典顺序
Named *load_list (char *file) {
  FILE *f = fopen (file, "r");
  Named *n = NULL;
  List *l;
  char line[128], list_name[128], *name, *item;
  while (fgets (line, 128, f)) {
    if (!empty_line (line)) {
      name = strtok (line, ":");  //该函数返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。
      sprintf (list_name, "%sList", name);
      l = list_insert (NULL, strdup (name));  //strdup()会先用maolloc()配置与参数s 字符串相同的空间大小，然后将参数s 字符串的内容复制到该内存地址，然后把该地址返回。该地址最后可以利用free()来释放。
      while ((item = strtok (NULL, " \t\r\n")))
      	l = list_insert (l, strdup (item));
      n = named_insert_sorted (n, strdup (list_name), list_reverse (l));
    }
  }
  fclose (f);
  return n;
}

TableEntry *find_table_entry (char *name, char *field, SchemaType *schema) {
  SchemaType *s = find_by_name (schema, name);
  TableEntry *e;
  if (!s) return NULL;
  foreach (e, s->entries) {
    if (!e->name) break;
    if (streq (e->name, field)) return e;
  }
  return NULL;
}

void print_list_info (Named *list, SchemaType *schema) {
  Named *n;
  List *l;
  SchemaElement *se;
  TableEntry *e, *f;
  char *name;
  print ("#define SE_LIST_LENGTH %d\n", list_length (list));
  print ("ListInfo se_list_info[] = {\n");
  for (n = list; n != NULL; n = n->next) {
    char *base = find_table_entry (n->name, "subscribable", schema) ?
                 "SubscribableList" : "List";
    l = n->data;
    name = l->data;
    print ("  {SE_%s, SE_%s, offsetof(SE_%s_t, %s), SE_%s, {",
           n->name, base, n->name, name, name);
    foreach (l, l->next) {
      char *field = strdup (l->data), *sub;
      field = strtok (field, ".");
      sub = strtok (NULL, "");
      e = find_table_entry (name, field, schema);
      if (!e) {
        print ("print_list_info: could not find element %s in %s\n",
               field, name);
        exit (0);
      }
      if (sub) {
        f = find_table_entry (e->type, sub, schema);
        se = f->se;
      } else se = e->se;
      print ("{offsetof(SE_%s_t, %s), ", name, l->data);
      l = l->next;
      if (strcmp (l->data, "descending") == 0) print ("-");
      print_xs_type (se->xs_type);
      if (l->next) print ("}, ");
      else print ("}}");
    }
    if (n->next) print ("},\n");
    else print ("}\n");
  }
  print ("};\n");
}
