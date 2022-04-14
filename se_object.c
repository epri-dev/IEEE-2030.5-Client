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
  unsigned short id;    //本代码中，为每一个资源（Resource）都定义了一个唯一的 id 。在se_types.h中定义。
  unsigned short base;  //这个什么含义不是很清楚？？
  unsigned short offset;//List元素在其所从属的上级数据结构中的地址偏移量。
  unsigned short type;  //为每一个Resource定义的一个唯一的id。在se_types.h中定义。
  Key key[3]; //？？？ 不清楚什么意思
} ListInfo;

/** @brief Get the list field of a IEEE 2030.5 list type object
    @param obj is a pointer to an IEEE 2030.5 list type object
    @param info is a pointer to the ListInfo for the list type
    @returns a pointer to the list field of the object
*/
#define se_list_field(obj, info) (List **)((obj)+(info)->offset)

/** @brief Is an IEEE 2030.5 object type derived from a base type?

    一个IEEE2030.5的类型是否是从一个基类中派生出来的？
    
    查询schema以决定某一个类型的对象是否是从一个基类中派生出来的？
    Queries the schema to determine if an object of certain type is derived
    from a base object. This is useful for dealing with certain classes of
    objects (e.g se_type_is_a (type, SE_Resource), or
    se_type_is_a (type, SE_Event)).

    这个有助于处理某些对象类型
    
    @param type is the type of the derived object    type参数是派生出来的类型（名）
    @param base is the type of the base object     base 是基类对象的类型（名）
    @returns 1 if object is derived from base, 0 otherwise  
*/
#define se_type_is_a(type, base) type_is_a (type, base, &se_schema) //在本应用中，se_schema是确定的，所以可以直接省略掉这个参数。


/** @brief Is an IEEE 2030.5 object type a List derived type?
    判断一个SE对象，是否是一个从“ SE_SubscribableList ” 对象中派生出来的子类（SE_SubscribableList是基类，SE中存在很多基类）
    @returns 1 if the type is a SubscribableList or a List type, 0 otherwise
*/


/* 搜索过程分解演示 ：

1）首先找到这个 SubscribableList 所在的行，对应代码行 base = schema->elements[base].index;  //获得base的值
  {.min = 1, .max = 1, .index = 539}, // SubscribableList

后面的过程，即执行:
while (se->index) { //有的index是0，也就是说碰到了index是0的数组的行，那么就不再继续往下搜索了，也就是得到了基类。
  if (se->index == base) return 1;
  se = &schema->elements[se->index];  //看起来是一个可以循环跳转的表
}

看起来像是不断的搜索“父类”的过程，Resource --》SubscribableResource --》SubscribableList
2）找到539行：这里才是 SE_SubscribableList，即这个类的具体描述。
    // SubscribableList (539)
    {.size = sizeof(SE_SubscribableList_t), .index = 496},
3）找到 496 行：
    // SubscribableResource (496)
    {.size = sizeof(SE_SubscribableResource_t), .index = 325},  //offset = 0 
4）继续找 325 行
    // Resource (325)
    {.size = sizeof(SE_Resource_t), .index = 0},
    {.offset = offsetof(SE_Resource_t, href), .min = 0, .max = 1, .attribute = 1, .xs_type = XS_ANY_URI, .n = 2},
index的值0，到此为止。

注意，在// SubscribableList (539) 行中，得到539这个数字，搜索se_schema.c，还能找到很多行中的“index=539”的情况，
这些都是 SE_SubscribableList 的“子类”。

基类是自己的基类。即如果下面宏定义中的type就是SE_SubscribableList，那么也会返回1。
如果有其他的子类，比如（搜索 539 这个值）
se_schema.c (Z:\) line 275 :   {.min = 1, .max = 1, .index = 539}, // SubscribableList
se_schema.c (Z:\) line 602 :   {.size = sizeof(SE_DERProgramList_t), .index = 539},
se_schema.c (Z:\) line 742 :   {.size = sizeof(SE_DERControlList_t), .index = 539},
se_schema.c (Z:\) line 908 :   {.size = sizeof(SE_FlowReservationResponseList_t), .index = 539},
se_schema.c (Z:\) line 1033 :   {.size = sizeof(SE_PrepaymentList_t), .index = 539},
se_schema.c (Z:\) line 1189 :   {.size = sizeof(SE_CustomerAgreementList_t), .index = 539},
se_schema.c (Z:\) line 1215 :   {.size = sizeof(SE_CustomerAccountList_t), .index = 539},
se_schema.c (Z:\) line 1235 :   {.size = sizeof(SE_BillingReadingSetList_t), .index = 539},
se_schema.c (Z:\) line 1269 :   {.size = sizeof(SE_BillingPeriodList_t), .index = 539},
se_schema.c (Z:\) line 1291 :   {.size = sizeof(SE_TextMessageList_t), .index = 539},
se_schema.c (Z:\) line 1317 :   {.size = sizeof(SE_MessagingProgramList_t), .index = 539},
se_schema.c (Z:\) line 1345 :   {.size = sizeof(SE_TimeTariffIntervalList_t), .index = 539},
se_schema.c (Z:\) line 1368 :   {.size = sizeof(SE_TariffProfileList_t), .index = 539},
se_schema.c (Z:\) line 1421 :   {.size = sizeof(SE_UsagePointList_t), .index = 539},
se_schema.c (Z:\) line 1441 :   {.size = sizeof(SE_ReadingSetList_t), .index = 539},
se_schema.c (Z:\) line 1448 :   {.size = sizeof(SE_ReadingList_t), .index = 539},
se_schema.c (Z:\) line 1472 :   {.size = sizeof(SE_MeterReadingList_t), .index = 539},
se_schema.c (Z:\) line 1521 :   {.size = sizeof(SE_EndDeviceControlList_t), .index = 539},
se_schema.c (Z:\) line 1547 :   {.size = sizeof(SE_DemandResponseProgramList_t), .index = 539},
se_schema.c (Z:\) line 1657 :   {.size = sizeof(SE_LogEventList_t), .index = 539},
se_schema.c (Z:\) line 2064 :   {.size = sizeof(SE_FunctionSetAssignmentsList_t), .index = 539},
se_schema.c (Z:\) line 2184 :   {.size = sizeof(SE_EndDeviceList_t), .index = 539},
这些都是 SE_SubscribableList 的子类。

*/
#define se_list(type) (se_type_is_a (type, SE_SubscribableList) \
		       || se_type_is_a (type, SE_List))


/** @brief Is an IEEE 2030.5 object type an Event derived type? 
  是否是一个Event类型派生出来的type？IEEE中的Event指的是需要定时完成的某一个任务。
  类似上面的 se_list 过程
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
  static int count = 0;
  int r;
  srand (time (NULL));
  r = rand ();
  memset (mrid, 0, 4);
  PACK32 (mrid + 4, r);
  PACK32 (mrid + 8, count++);
  PACK32 (mrid + 12, pen_id);
}

int compare_ids (const void *a, const void *b) {
  const int short *x = a, *y = b;
  return *x - *y;
}

/*

二分搜索 bsearch

其基本原理是待搜索的元素是经过排序的。通过每次检查排在正中间的那个元素，可以去掉一半的不匹配元素。
如此，最多可以通过log2(n)次比较找到要找的元素（或者确认它不存在）。
void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*cmp)(const void *,const void *));
key   指向要查找的元素
base  指向进行查找的数组
num   数组中元素的个数
size  数组中每个元素的大小，一般用sizeof()表示
cmp   比较两个元素的函数，定义比较规则。需要注意的是，查找数组必须是经过预先排序的，而排序的规则要和比较子函数cmp的规则相同。


*/

//这个函数的作用是通过type值，就是一个Resource值来找到对应的ListInfo结构体。
ListInfo *find_list_info (unsigned short type) {
  return bsearch (&type, se_list_info, SE_LIST_LENGTH, sizeof (ListInfo), compare_ids);
}

/*比较两个数字中的某一个bit位大小。*/
int compare_binary (uint8_t *a, uint8_t *b, int n) {
  while (n--) {
    if (a[n] < b[n]) return -1;
    if (a[n] > b[n]) return 1;
  }
  return 0;
}

#define compare_uint(a, b, type) \
  (*(type)a < *(type)b)? -1 : (*(type)a - *(type)b)

//a/b是两个指向数据的地址指针
int compare_key (void *a, void *b, Key *key) {
  int type, n;
  if (key->type < 0) {  //表示负数？
    void *t = a;
    a = b;
    b = t;
    type = -key->type;
  } else type = key->type;
  n = type >> 4;  //字节数
  type &= 0xf;
  a += key->offset;
  b += key->offset;
  switch (type) {
  case XS_STRING:
  case XS_ANY_URI:
    return strcmp (a, b);
  case XS_HEX_BINARY:
    return compare_binary (a, b, n);
  case XS_LONG:
    return *(int64_t *)a - *(int64_t *)b;
  case XS_INT:
    return *(int32_t *)a - *(int32_t *)b;
  case XS_SHORT:
    return *(int16_t *)a - *(int16_t *)b;
  case XS_BYTE:
    return *(int8_t *)a - *(int8_t *)b;
  case XS_ULONG:
    return compare_uint (a, b, uint64_t *);
  case XS_UINT:
    return compare_uint (a, b, uint32_t *);
  case XS_USHORT:
    return compare_uint (a, b, uint16_t *);
  case XS_UBYTE:
    return compare_uint (a, b, uint8_t *);
  }
  return -1;
}

int compare_keys (void *a, void *b, ListInfo *info) {
  Key *key = info->key;
  int i = 0, ret;
  while (i < 3 && key->type) {
    if (ret = compare_key (a, b, key)) return ret;
    i++;
    key++;
  }
  return ret;
}

//以排序后的顺序来插入一个新的 ListInfo 元素。
void *insert_se_object (List *list, List *n, ListInfo *info) {
  List *prev = NULL, *l = list;
  while (l && (compare_keys (n->data, l->data, info) > 0))
    prev = l, l = l->next;
  if (prev) prev->next = n;
  else list = n;
  n->next = l;
  return list;
}


/*初始化se输出对象。
指定schema，驱动函数等。*/
void se_output_init (Output *o, char *buffer, int size, int xml) {
  if (xml) output_init (o, &se_schema, buffer, size);
  else exi_output_init (o, &se_schema, buffer, size);
}


//将对象内容以文本方式打印出来
void print_se_object (void *obj, int type) {
  Output o;
  char buffer[1024];
  output_init (&o, &se_schema, buffer, 1024);
  while (output_doc (&o, obj, type)) printf ("%s", buffer);
}

#endif
