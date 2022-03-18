// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup schema Schema

    Defines the Schema and SchemaElement data types, provides utility functions
    for Schema defined objects.
    @{
*/

enum XsType {XS_NULL, XS_STRING, XS_BOOLEAN, XS_HEX_BINARY, XS_ANY_URI,
	     XS_LONG, XS_INT, XS_SHORT, XS_BYTE, XS_ULONG, 
	     XS_UINT, XS_USHORT, XS_UBYTE};

#define xs_type(b, l) (((l)<<4)|(b))

typedef struct {
  union {
    unsigned short offset;
    unsigned short size;
  };
  union {
    unsigned short xs_type;
    unsigned short index;
  };
  unsigned char min, max, n;
  unsigned int bit : 5;
  unsigned int attribute : 1;
  unsigned int simple : 1;
  unsigned int unbounded : 1;
} SchemaElement;

typedef struct _Schema {
  const char *namespace;
  const char *schemaId;
  const int length;
  const SchemaElement *elements;
  const char * const *names;
  const uint16_t *ids;
} Schema;

int se_is_a (const SchemaElement *se, int base, const Schema *schema);

/** @brief Is a type derived from another type within a schema?
    @param type is the derived type
    @param base is a base type
    @param schema is a pointer to a Schema
    @returns 1 if type is derived from base, 0 otherwise
*/
int type_is_a (int type, int base, const Schema *schema);

/** @brief What is the size of an object of a given type?
    @param type is a schema type
    @param schema is a pointer to a Schema
    @returns the size of the schema typed object in bytes 
*/
int object_size (int type, const Schema *schema);

/** @brief Return a string representation of a type.
    @param type is a schema type
    @param schema is a pointer to a Schema
    @returns the type name as a string
*/
const char *type_name (int type, const Schema *schema);

/** @brief Free an object's elements without freeing the object container.
    @param obj is a pointer to a schema typed object 
    @param type is the type of the object
    @param schema is a pointer to the Schema
*/
void free_object_elements (void *obj, int type, const Schema *schema);

/** @brief Free an object's elements and the container.
    @param obj is a pointer to a schema typed object 
    @param type is the type of the object
    @param schema is a pointer to the Schema
*/
void free_object (void *obj, int type, const Schema *schema);

/** @brief Replace one object for another.

    Free the elements of the destination object and copy the source object to
    the same location. Free the source object container.
    @param dest is the destination object
    @param src is the source object
    @param type is the schema type of the objects
    @param schema is a pointer to the Schema
 */
void replace_object (void *dest, void *src, int type, const Schema *schema);

/** @} */

#ifndef HEADER_ONLY

// pointer types
int is_pointer (int type) {
  switch (type) {
  case XS_STRING: case XS_ANY_URI: return 1;
  } return 0;
}

int se_is_a (const SchemaElement *se, int base, const Schema *schema) {
  base = schema->elements[base].index;
  if (se->simple || se->attribute) return 0;
  while (se->index) {
    if (se->index == base) return 1;
    se = &schema->elements[se->index];
  } return 0;
}

int type_is_a (int type, int base, const Schema *schema) {
  if (type >= schema->length) return 0;
  return se_is_a (&schema->elements[type], base, schema);
}

int object_size (int type, const Schema *schema) {
  const SchemaElement *se = &schema->elements[type];
  return schema->elements[se->index].size;
}

const char *type_name (int type, const Schema *schema) {
  return schema->names[type];
}

const char *se_name (const SchemaElement *se, const Schema *schema) {
  int index = se - schema->elements;
  int id = index < schema->length? index
    : schema->ids[index - schema->length];
  return schema->names[id];
}

int object_element_size (const SchemaElement *se, const Schema *schema) {
  if (se->simple) {
    int n = se->xs_type >> 4;
    switch (se->xs_type & 0xf) {
    case XS_STRING: return n? n : sizeof (char *);
    case XS_BOOLEAN: return 0;
    case XS_HEX_BINARY: return n;
    case XS_ANY_URI: return sizeof (char *);
    case XS_LONG: return 8;
    case XS_INT: return 4;
    case XS_SHORT: return 2;
    case XS_BYTE: return 1;
    case XS_ULONG: return 8;
    case XS_UINT: return 4;
    case XS_USHORT: return 2;
    case XS_UBYTE: return 1;
    } 
  } else {
    se = &schema->elements[se->index];
    return se->size;
  } return 0;
}

void free_elements (void *obj, const SchemaElement *se,
		    const Schema *schema) {
  while (1) { int i; void *element = obj + se->offset;
    if (se->attribute || se->simple) {
      if (is_pointer (se->xs_type)) {
	void **value = (void **)element; i = 0;
	while (i < se->max && *value) {
	  void *t = *value; free (t); value++; i++;
	}
      }
    } else if (se->n) {
      const SchemaElement *first = &schema->elements[se->index];
      if (se->unbounded) { List *t, *l = *(List **)(element);
	while (l) {
	  t = l; l = l->next;
	  free_elements (t->data, first+1, schema);
	  if (t->data) free (t->data); free (t); }
      } else {
	for (i = 0; i < se->max; i++) {
	  free_elements (element, first+1, schema);
	  element += first->size; 
	}
      }
    } else return; se++;
  }
}

void free_object_elements (void *obj, int type, const Schema *schema) {
  const SchemaElement *se = &schema->elements[type];
  se = &schema->elements[se->index+1];
  free_elements (obj, se, schema);
}

void free_object (void *obj, int type, const Schema *schema) {
  free_object_elements (obj, type, schema); free (obj);
}

void replace_object (void *dest, void *src, int type, const Schema *schema) {
  free_object_elements (dest, type, schema);
  memcpy (dest, src, object_size (type, schema)); free (src);
}

#endif
