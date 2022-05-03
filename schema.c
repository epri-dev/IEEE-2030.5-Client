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
    unsigned short offset;    //可能是这个SchemaElement在上一级对象中内存偏移地址。
    unsigned short size;      //这个对象的占用空间大小
  };
  union {
    unsigned short xs_type;   //这个应该是基础类型
    unsigned short index;     //这个应该是复杂类型，比如se_types.h中定义的值。
  };
  unsigned char min, max, n;  //在整个文档中，max最大值只有1。有可能是0。min只有0或者1。
  unsigned int bit : 5;       //其值表示自己在在其所在的结构体中的_flags这个变量中占据的是第几个bit。这个flag表示某个元素是否在服务器返回的数据中是否存在。
  //在se_types.h这个文件中，任何一个结构体变量都会在前面注释这个flag相关的信息。
  
  unsigned int attribute : 1; //表示自己是否是一个属性成员
  unsigned int simple : 1;    //是否自己是否是简单类型，比如整形类型或者指针类型
  unsigned int unbounded : 1; //看起来跟是否是 List 类型有关。指示该子类的“长度”是不定的，比如List成员。
} SchemaElement;

typedef struct _Schema {
  const char *namespace;  /* "http://ieee.org/2030.5" */
  const char *schemaId;   /* "S1" */
  const int length;     /* 下面的 schema->elements 数组中，在 se_types.h 中定义的基本类型type的个数（最后一个是SE_mRIDType 320），而不是下面的elements长度。*/
  const SchemaElement *elements;        /* 一个数组，是SchemaElement元素的集合。其中前面的length部分，跟后面的部分内容不一样。 */
  const char *const *names; /* 名字列表 */
  const uint16_t *ids;  /* id列表 */
} Schema;

int se_is_a (const SchemaElement *se, int base, const Schema *schema);

/** @brief Is a type derived from another type within a schema ?  判断一个类型是否是由另外一个属于schema中的类派生而来
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

/*举一个实例：
Step1:  
{.min = 1, .max = 1, .index = 1828}, // AbstractDevice
接下去找到这数组中1828行，可以直接搜索 1828


Step2:

se_schema.c这个文件是自动生成的，这里自动写好了注释，括号中的值就是所在的数组行号（索引）。
// AbstractDevice (1828)
{.size = sizeof(SE_AbstractDevice_t), .index = 496},  //offset =0 
{.offset = offsetof(SE_AbstractDevice_t, href), .min = 0, .max = 1, .attribute = 1, .xs_type = XS_ANY_URI, .n = 14},  //index = 0
{.offset = offsetof(SE_AbstractDevice_t, subscribable), .min = 0, .max = 1, .attribute = 1, .bit = 1, .xs_type = XS_UBYTE, .n = 13},
{.offset = offsetof(SE_AbstractDevice_t, ConfigurationLink), .min = 0, .max = 1, .bit = 14, .index = 1826, .n = 12},
{.offset = offsetof(SE_AbstractDevice_t, DERListLink), .min = 0, .max = 1, .bit = 13, .index = 1823, .n = 11},
{.offset = offsetof(SE_AbstractDevice_t, DeviceInformationLink), .min = 0, .max = 1, .bit = 12, .index = 1821, .n = 10},
{.offset = offsetof(SE_AbstractDevice_t, DeviceStatusLink), .min = 0, .max = 1, .bit = 11, .index = 1819, .n = 9},
{.offset = offsetof(SE_AbstractDevice_t, FileStatusLink), .min = 0, .max = 1, .bit = 10, .index = 1817, .n = 8},
{.offset = offsetof(SE_AbstractDevice_t, IPInterfaceListLink), .min = 0, .max = 1, .bit = 9, .index = 1814, .n = 7},
{.offset = offsetof(SE_AbstractDevice_t, lFDI), .min = 0, .max = 1, .bit = 3, .simple = 1, .xs_type = xs_type(XS_HEX_BINARY, 20), .n = 6},
{.offset = offsetof(SE_AbstractDevice_t, LoadShedAvailabilityListLink), .min = 0, .max = 1, .bit = 8, .index = 1811, .n = 5},
{.offset = offsetof(SE_AbstractDevice_t, loadShedDeviceCategory), .min = 0, .max = 1, .bit = 7, .simple = 1, .xs_type = xs_type(XS_HEX_BINARY, 4), .n = 4},
{.offset = offsetof(SE_AbstractDevice_t, LogEventListLink), .min = 0, .max = 1, .bit = 6, .index = 1808, .n = 3},
{.offset = offsetof(SE_AbstractDevice_t, PowerStatusLink), .min = 0, .max = 1, .bit = 5, .index = 1806, .n = 2},
{.offset = offsetof(SE_AbstractDevice_t, sFDI), .min = 1, .max = 1, .simple = 1, .xs_type = XS_ULONG, .n = 1},

继续找496

Step3:

// SubscribableResource (496)
{.size = sizeof(SE_SubscribableResource_t), .index = 325},  //offset = 0 
{.offset = offsetof(SE_SubscribableResource_t, href), .min = 0, .max = 1, .attribute = 1, .xs_type = XS_ANY_URI, .n = 3},
{.offset = offsetof(SE_SubscribableResource_t, subscribable), .min = 0, .max = 1, .attribute = 1, .bit = 1, .xs_type = XS_UBYTE, .n = 2},
继续搜索325

Step4:
// Resource (325)
{.size = sizeof(SE_Resource_t), .index = 0},
{.offset = offsetof(SE_Resource_t, href), .min = 0, .max = 1, .attribute = 1, .xs_type = XS_ANY_URI, .n = 2},
这里，index的值为0，搜索停止。


*/
/* base：就是在 se_types.h 中定义的元素的类型名，比如 SE_AbstractDevice 的值是0 */
int se_is_a (const SchemaElement *se, int base, const Schema *schema) {
  //注意这里的base值变化了。
  //每一个派生的类型，都由index变量来指定自己的基类在什么位置 -- 数组中的index
  base = schema->elements[base].index;  //获得base的值
  if (se->simple || se->attribute) return 0;  //如果是一个简单类型比如整形数字之类的
  /*举一个例子：
  // SubscribableResource (496)
  {.size = sizeof(SE_SubscribableResource_t), .index = 325},  //注意这里的第一行offset = 0而index不等于0，后面的相反。 
  {.offset = offsetof(SE_SubscribableResource_t, href), .min = 0, .max = 1, .attribute = 1, .xs_type = XS_ANY_URI, .n = 3},
  {.offset = offsetof(SE_SubscribableResource_t, subscribable), .min = 0, .max = 1, .attribute = 1, .bit = 1, .xs_type = XS_UBYTE, .n = 2},
  */
  while (se->index) { //有的index是0，也就是说碰到了index是0的数组的行，那么就不再继续往下搜索了，也就是得到了基类。
    if (se->index == base) return 1;
    se = &schema->elements[se->index];  //看起来是一个可以循环跳转的表
  }
  return 0;
}

//判定一个类型是否派生自某个基类
int type_is_a (int type, int base, const Schema *schema) {
  if (type >= schema->length) return 0; //之前跟length规定的长度的之前的元素是有效的。只搜索length前面的部分。
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

/*参数中，se是从属于schema对象中的一个元素。在现在这个程序中，获取的是全局变量se_names[]中的有一个字符串。schema->ids表中的长度跟se_elements的长度一致，每一行对应，值表示了 se_types.h 中定义的值。schema->names跟 se_types.h定义id宏定义值对应，通过偏移量即可获得名字字符串。*/
const char *se_name (const SchemaElement *se, const Schema *schema) {
  int index = se - schema->elements;/*获取se在数组 schema->elements 中的相对位置*/
  int id = index < schema->length ? index : schema->ids[index - schema->length];  /*排在 schema->length 之前的是基类；如果超过基类范围，则取ids中的值作为偏移量*/
  return schema->names[id]; //最后获得名字。
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


/*释放一个完整的对象的占用的内存空间
参数描述：
void *obj：是一个内存地址，包含了一个具体的SE对象
const SchemaElement *se：上述的obj数据的相应的描述信息
const Schema *schema：全局用到的 se_schema 数组。
*/
void free_elements (void *obj, const SchemaElement *se, const Schema *schema) {
  while (1) {
    int i;
    void *element = obj + se->offset; //获取到自己在上级object（对象）中的数据存储空间的内存偏移量地址 ??? 
    if (se->attribute || se->simple) {
      if (is_pointer (se->xs_type)) { //是否是一个指针类型的，比如表示URL的变量 *href。
        void **value = (void **)element;
        i = 0;
        while (i < se->max && *value) { //这个指针值不是空
          void *t = *value;
          free (t);
          value++;  //向后移动一个单元
          i++;
        }
      }
    /*举例：
    {.offset = offsetof(SE_MirrorReadingSet_t, Reading), .min = 0, .unbounded = 1, .index = 356, .n = 2},
    其指向的index=356，内容为：
      // Reading (356)
      {.size = sizeof(SE_Reading_t), .index = 349},
      {.offset = offsetof(SE_Reading_t, href), .min = 0, .max = 1, .attribute = 1, .xs_type = XS_ANY_URI, .n = 9},
    即下面的first指向的是{.size = sizeof(SE_Reading_t), .index = 349},
    所以，在后面的迭代调用 free_elements 的调用中，第二项参数是 first + 1，即从下面一行开始执行释放。
    
      */
    } else if (se->n) { //只有一个结构体中的“成员”，其n值总是大于0。
      const SchemaElement *first = &schema->elements[se->index];  //指向了该List成员的具体结构描述位置。当前位置仅仅是一个指针。
      if (se->unbounded) {  //通常只有List类型的成员，其unbounded值总是大于0。其他成员，总是等于0。即表明这个是List类型。
        List *t, *l = *(List **)(element);
        while (l) {
          t = l;
          l = l->next;
          free_elements (t->data, first + 1, schema); //这里迭代释放了。但是first + 1指向的是下一个同类型单元？
          if (t->data) free (t->data);  //以数据指针的值是否为空来判断是否有值。
          free (t);
        }
      } else {  //看起来像是一个“固定”的意思。即如果遇到的是一个固定长度
        for (i = 0; i < se->max; i++) { //通常，max最大值只有1。
          free_elements (element, first + 1, schema); //这里的first + 1是什么意思？？
          element += first->size;
        }
      }
    } else return;  //遇到都不符合上述两个if中的条件，就退出。从se_schema.c中可以看出，相当于是完成了一个对象的全部成员的释放，然后到了下一个对象的首行。
    se++;
  }
}


void free_object_elements (void *obj, int type, const Schema *schema) {
  const SchemaElement *se = &schema->elements[type];  //只要给出type值（ 在se_types.h文件中的最后一段宏定义中给出 ）就行了。
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
