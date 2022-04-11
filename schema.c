// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup schema Schema

    Defines the Schema and SchemaElement data types, provides utility functions
    for Schema defined objects.
    @{
*/

enum XsType { XS_NULL, XS_STRING, XS_BOOLEAN, XS_HEX_BINARY, XS_ANY_URI,
                XS_LONG, XS_INT, XS_SHORT, XS_BYTE, XS_ULONG,
             XS_UINT, XS_USHORT, XS_UBYTE
            };

#define xs_type(b, l) (((l)<<4)|(b))

typedef struct {
  union {
    unsigned short offset;
    unsigned short size;      //这个对象的占用空间大小
  };
  union {
    unsigned short xs_type;
    unsigned short index;
  };
  unsigned char min, max, n;
  unsigned int bit : 5;
  unsigned int attribute : 1; //表示是否含有属性成员
  unsigned int simple : 1;    //是否是简单类型，比如整形类型或者指针类型
  unsigned int unbounded : 1;
} SchemaElement;

typedef struct _Schema {
  const char *namespace;
  const char *schemaId;
  const int length;     /*下面的schema->elements的数组单元个数*/
  const SchemaElement *elements;        /*一个数组，是SchemaElement元素的集合*/
  const char *const *names;
  const uint16_t *ids;
} Schema;

int se_is_a (const SchemaElement *se, int base, const Schema *schema);

/** @brief Is a type derived from another type within a schema? 判断一个类型是否是由另外一个属于schema中的类派生而来
    @param type is the derived type  派生类型
    @param base is a base type  基础类型
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

// pointer types 判断一个数据类型是否是一个指针类型
int is_pointer (int type) {
  switch (type) {
  case XS_STRING:
  case XS_ANY_URI:
    return 1;
  }
  return 0;
}

int se_is_a (const SchemaElement *se, int base, const Schema *schema) {
  //注意这里的base值变化了。
  base = schema->elements[base].index;  //每一个派生的类型，都由index变量来指定自己的基类在什么位置 -- 数组中的index
  if (se->simple || se->attribute) return 0;  //如果是一个简单类型比如整形数字之类的
  while (se->index) { //有的index是0，也就是说碰到了index是0的数组的行，那么就不再继续往下搜索了，也就是得到了基类。
    if (se->index == base) return 1;
    se = &schema->elements[se->index];  //看起来是一个可以循环跳转的表
  }
  return 0;
}

//判定一个类型是否派生自某个基类
int type_is_a (int type, int base, const Schema *schema) {
  if (type >= schema->length) return 0;
  return se_is_a (&schema->elements[type], base, schema);
}

//这里的type定义，参见se_types.h中的定义。每一个基础type都有一个唯一的定义值。

/*

比如对于基础类型 SE_AbstractDevice，其在se_types.h中定义的值是0

const SchemaElement se_elements[] = {
  {.min=1, .max=1, .index=1828}, // AbstractDevice
通过index值1828来索引，找到对应的行：
// AbstractDevice (1828)
{.size=sizeof(SE_AbstractDevice_t), .index=496},

就可以得到其size。

*/

int object_size (int type, const Schema *schema) {
  const SchemaElement *se = &schema->elements[type];
  return schema->elements[se->index].size;
}

const char *type_name (int type, const Schema *schema) {
  return schema->names[type];
}

/*参数中，se是从属于schema对象中的一个元素。在现在这个程序中，获取的是全局变量se_names[]中的有一个字符串*/
const char *se_name (const SchemaElement *se, const Schema *schema) {
  int index = se - schema->elements;/*获取se在数组schema->elementsh中的相对位置*/
  int id = index < schema->length ? index : schema->ids[index - schema->length];
  return schema->names[id];
}

//不同的元素的占用空间大小
int object_element_size (const SchemaElement *se, const Schema *schema) {
  if (se->simple) {
    int n = se->xs_type >> 4; //高4位表示数量
    switch (se->xs_type & 0xf) {  //第四位表示类型
    case XS_STRING:
      return n ? n : sizeof (char *);
    case XS_BOOLEAN:
      return 0;
    case XS_HEX_BINARY:
      return n;
    case XS_ANY_URI:
      return sizeof (char *);
    case XS_LONG:
      return 8;
    case XS_INT:
      return 4;
    case XS_SHORT:
      return 2;
    case XS_BYTE:
      return 1;
    case XS_ULONG:
      return 8;
    case XS_UINT:
      return 4;
    case XS_USHORT:
      return 2;
    case XS_UBYTE:
      return 1;
    }
  } else {
    se = &schema->elements[se->index];  //如果不是simple类型，那么就是复杂类型比如一个结构体数据
    return se->size;
  }
  return 0;
}

void free_elements (void *obj, const SchemaElement *se, const Schema *schema) {
  while (1) {
    int i;
    void *element = obj + se->offset;
    if (se->attribute || se->simple) {
      if (is_pointer (se->xs_type)) {
        void **value = (void **)element;
        i = 0;
        while (i < se->max && *value) {
          void *t = *value;
          free (t);
          value++;
          i++;
        }
      }
    } else if (se->n) {
      const SchemaElement *first = &schema->elements[se->index];
      if (se->unbounded) {
        List *t, *l = *(List **)(element);
        while (l) {
          t = l;
          l = l->next;
          free_elements (t->data, first + 1, schema);
          if (t->data) free (t->data);
          free (t);
        }
      } else {
        for (i = 0; i < se->max; i++) {
          free_elements (element, first + 1, schema);
          element += first->size;
        }
      }
    } else return;
    se++;
  }
}

void free_object_elements (void *obj, int type, const Schema *schema) {
  const SchemaElement *se = &schema->elements[type];
  se = &schema->elements[se->index + 1];
  free_elements (obj, se, schema);
}

void free_object (void *obj, int type, const Schema *schema) {
  free_object_elements (obj, type, schema);
  free (obj);
}

void replace_object (void *dest, void *src, int type, const Schema *schema) {
  free_object_elements (dest, type, schema);
  memcpy (dest, src, object_size (type, schema));
  free (src);
}

#endif
