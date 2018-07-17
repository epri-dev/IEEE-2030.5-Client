void print_desc_line (int n, char *desc) {
  print ("    %s\n\n", desc);
}

void print_description (void *desc) {
  if (desc) {
    char *buffer = strdup (desc);
    process_buffer (buffer, print_desc_line);
    free (buffer);
  }
}

void print_field_doc (char *name, Field *f) {
  print ("    @var SE_%s_t::%s\n", name, f->name);
  print_description (f->desc);
}

void print_particle_doc (char *name, Particle *part, SchemaType *head) {
  Particle *p; Field *f;
  foreach (p, part) {
    switch (p->kind) {
    case PartElement: f = &p->field;
      if (f->prim != XS_BOOLEAN)
	print_field_doc (name, &p->field);
      break;
    case PartGroup:
    case PartChoice:
    case PartSequence:
      print_particle_doc (name, p->child, head);
    case PartAny:
      break;
    }
  }
}

void print_fields_doc (char *name, SchemaType *t, SchemaType *head) {
  List *attr; SchemaType *u;
  if (t->base && (u = find_by_name (head, t->base)))
    print_fields_doc (name, u, head);
  if (!t->restriction) {
    foreach (attr, t->attr) {
      AttrDecl *a = attr->data;
      print_field_doc (name, &a->field);
    }
    print_particle_doc (name, t->part, head);
  }
}

void print_flags_doc (SchemaType *t, SchemaType *head) { List *f;
  if (t->base) print_flags_doc (find_by_name (head, t->base), head);
  foreach (f, t->flags) {
    print_flag (f->data, 0); print ("\n");
  }
}

void print_type_doc (SchemaType *t, SchemaType *head) {
  char *base = t->base? t->base : "se_types";
  print ("/** @defgroup %s %s\n", t->name, t->name);
  switch (t->kind) {
  case ComplexType:
    print ("    @ingroup %s\n\n", base);
    print_description (t->desc);
    print ("    @{ */\n\n");
    print ("/** @struct SE_%s_t se_types.h\n\n", t->name);
    print_description (t->desc);
    if (has_flags (t, head)) {
      print ("    @var SE_%s_t::_flags\n", t->name);
      print_flags_doc (t, head);
    }
    print_fields_doc (t->name, t, head);
    break;
  case AtomicType:
    if (string_index (base, xs_names, 12) < 12)
      print ("    @ingroup se_typedefs\n\n");
    else print ("    @ingroup %s\n\n", base);
    print_description (t->desc);
    print ("    @{ */\n\n");
    print ("/** @typedef SE_%s_t\n\n", t->name);
    print_description (t->desc);
    break;
  }
  print ("*/\n\n/** @} */\n\n");
}

void print_types_doc (List *list, SchemaType *head) { List *l;
  print ("/** @defgroup se_types IEEE 2030.5 Types */\n\n");
  print ("/** @defgroup se_typedefs Typedefs \n");
  print ("    @ingroup se_types */\n\n");
  foreach (l, list) print_type_doc (l->data, head);
}
