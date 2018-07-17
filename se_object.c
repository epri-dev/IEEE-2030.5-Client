// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup se_object IEEE 2030.5 Schema

    Provides utility functions for IEEE 2030.5 schema defined objects.
    @ingroup schema
    @{
*/

/** @brief Generate MRID for IEEE 2030.5 identified object.

    MRID is a 16 byte number based upon a random number, a global count, and a
    manufactuer PEN.
    @param mrid is pointer to an array of 16 bytes
*/
void mrid_gen (uint8_t *mrid);

typedef struct {
  unsigned short offset;
  short type;
} Key;

typedef struct {
  unsigned short id;
  unsigned short base;
  unsigned short offset;
  unsigned short type;
  Key key[3];
} ListInfo;

/** @brief Get the list field of a IEEE 2030.5 list type object
    @param obj is a pointer to an IEEE 2030.5 list type object
    @param info is a pointer to the ListInfo for the list type
    @returns a pointer to the list field of the object
*/
#define se_list_field(obj, info) (List **)((obj)+(info)->offset)

/** @brief Is an IEEE 2030.5 object type derived from a base type?
    
    Queries the schema to determine if an object of certain type is derived
    from a base object. This is useful for dealing with certain classes of
    objects (e.g se_type_is_a (type, SE_Resource), or 
    se_type_is_a (type, SE_Event)).
    @param type is the type of the derived object
    @param base is the type of the base object
    @returns 1 if object is derived from base, 0 otherwise
*/
#define se_type_is_a(type, base) type_is_a (type, base, &se_schema)

/** @brief Is an IEEE 2030.5 object type a List derived type?
    @returns 1 if the type is a SubscribableList or a List type, 0 otherwise
*/
#define se_list(type) (se_type_is_a (type, SE_SubscribableList) \
		       || se_type_is_a (type, SE_List))

/** @brief Is an IEEE 2030.5 object type an Event derived type?
    @returns 1 if the type is a Event type, 0 otherwise
*/
#define se_event(type) se_type_is_a (type, SE_Event)

/** @brief Return the size of an IEEE 2030.5 object given its type.

    Queries the schema to determine the size of a object given its type.
    @param type is the type of the IEEE 2030.5 object
    @returns the size of the object in bytes
*/
#define se_object_size(type) object_size (type, &se_schema)

/** @brief Free an IEEE 2030.5 object.
    @param obj is an IEEE 2030.5 object
    @param type is the type of the o
 */
#define free_se_object(obj, type) free_object (obj, type, &se_schema)

/** @brief Replace an IEEE 2030.5 object with another of the same type.

    Frees the elements of the destination object and copies the source object
    to the same location, replacing one object with another. Also frees the
    source object container.
*/
#define replace_se_object(dest, src, type)				\
  replace_object (dest, src, type, &se_schema)

/** @brief Get the ListInfo structure for the given schema type.

    The type should be one the IEEE 2030.5 list types for a non-NULL result.
    @param type is an SE_ type name (e.g. SE_EndDeviceList)
    @returns a pointer to the ListInfo structure for the given type or
    NULL if the type is not a list type.
*/
ListInfo *find_list_info (unsigned short type);

/** @brief Compare the keys of two IEEE 2030.5 objects using the provided list
    ordering.

    @param a is an IEEE 2030.5 object
    @param b is an IEEE 2030.5 object
    @param info is a pointer to a ListInfo structure
    @returns a result < 0 if a comes before b, a result > 0 if a comes after b,
    and a result == 0 if a and b can occupy the same position 
*/
int compare_keys (void *a, void *b, ListInfo *info);

/** @brief Insert an IEEE 2030.5 object into a sorted list.

    Comparison is based upon the ListInfo given.
    @param list is a list of objects of a uniform type given by info->type
    @param n is a pointer to the List container of the object to be inserted.
    @param info is a pointer to the ListInfo upon which the comparison is made.
    @returns the sorted list with n inserted
*/
void *insert_se_object (List *list, List *n, ListInfo *info);

/** @brief Initialize an Output object to output an XML or EXI document.
    @param o is a pointer to an Output object
    @param buffer is a container for the document
    @param size is the size of the container
    @param xml is 1 for an XML document, 0 for an EXI document
*/
void se_output_init (Output *o, char *buffer, int size, int xml);

/** @brief Print an IEEE 2030.5 object as an XML document
    @param obj is a pointer to the object
    @param type is the object type
*/
void print_se_object (void *obj, int type);

/** @} */

#ifdef HEADER_ONLY

extern int pen_id; //< set to manufacturer PEN
extern Schema se_schema;

#else

#include "se_schema.c"
#include "se_list.c"

// replace with manufacturer PEN
int pen_id = 54321;

void mrid_gen (uint8_t *mrid) {
  static int count = 0; int r;
  srand (time (NULL)); r = rand ();
  memset (mrid, 0, 4);
  PACK32 (mrid+4, r);
  PACK32 (mrid+8, count++);
  PACK32 (mrid+12, pen_id);
}

int compare_ids (const void *a, const void *b) {
  const int short *x = a, *y = b;
  return *x - *y;
}

ListInfo *find_list_info (unsigned short type) {
  return bsearch (&type, se_list_info, SE_LIST_LENGTH,
		  sizeof (ListInfo), compare_ids);  
}

int compare_binary (uint8_t *a, uint8_t *b, int n) {
  while (n--) {
    if (a[n] < b[n]) return -1;
    if (a[n] > b[n]) return 1;
  } return 0;
}

#define compare_uint(a, b, type) \
  (*(type)a < *(type)b)? -1 : (*(type)a - *(type)b)

int compare_key (void *a, void *b, Key *key) { int type, n;
  if (key->type < 0) { void *t = a; a = b; b = t; type = -key->type; }
  else type = key->type;
  n = type >> 4; type &= 0xf;
  a += key->offset; b += key->offset;
  switch (type) {
  case XS_STRING: case XS_ANY_URI: return strcmp (a, b);
  case XS_HEX_BINARY: return compare_binary (a, b, n);
  case XS_LONG: return *(int64_t *)a - *(int64_t *)b;
  case XS_INT: return *(int32_t *)a - *(int32_t *)b;
  case XS_SHORT: return *(int16_t *)a - *(int16_t *)b;
  case XS_BYTE: return *(int8_t *)a - *(int8_t *)b;
  case XS_ULONG: return compare_uint (a, b, uint64_t *);
  case XS_UINT: return compare_uint (a, b, uint32_t *);
  case XS_USHORT: return compare_uint (a, b, uint16_t *);
  case XS_UBYTE: return compare_uint (a, b, uint8_t *);
  } return -1;
}

int compare_keys (void *a, void *b, ListInfo *info) {
  Key *key = info->key; int i = 0, ret;
  while (i < 3 && key->type) {
    if (ret = compare_key (a, b, key)) return ret;
    i++; key++;
  } return ret;
}

void *insert_se_object (List *list, List *n, ListInfo *info) {
  List *prev = NULL, *l = list;
  while (l && (compare_keys (n->data, l->data, info) > 0))
    prev = l, l = l->next;
  if (prev) prev->next = n; else list = n;
  n->next = l; return list;
}

void se_output_init (Output *o, char *buffer, int size, int xml) {
  if (xml) output_init (o, &se_schema, buffer, size); 
  else exi_output_init (o, &se_schema, buffer, size);
}

void print_se_object (void *obj, int type) {
  Output o; char buffer[1024];
  output_init (&o, &se_schema, buffer, 1024);
  while (output_doc (&o, obj, type)) printf ("%s", buffer);
}

#endif
