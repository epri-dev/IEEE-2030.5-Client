// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup retrieve Retrieve

    The retrieve module provides high level methods to retrieve resources from
    IEEE 2030.5 servers. These methods use a placeholder called a Stub to track
    the state of retrieval and provide local storage for a resource.
    这些方法用一个 叫做“Stub”的“占位符placeholder”来跟踪“检索retrieval”的状态和提供一个本地的对一个资源的存储。
    The Stubs can be linked together to create and manage hierarchies of both dependent
    and required resources. 这些Stubs能够相互连接到一起，用以构建和管理各个依赖和需求的资源的层级关系。

    当一个资源和它全部的子层级资源都被全面和成功检索到了之后，然后这些资源将被视作“完整的”，这个状态被这个检索模块监控并且
    可以可选的通知用户提供的程序，以这个资源作为一个参数。
    When a resource and all of its subordinates are fully and successfully
    retrieved then the resource is considered complete, this condition is
    monitored by the retrieval algorithm and can optionally signal a user
    supplied routine using the resource as a parameter.

    @ingroup resource
    @{
*/

extern const char * const se_names[];


#define RESOURCE_POLL (EVENT_NEW+6)
#define RESOURCE_UPDATE (EVENT_NEW+7)
#define RESOURCE_REMOVE (EVENT_NEW+8)
#define RETRIEVE_FAIL (EVENT_NEW+9)

/*
time_t类型在time.h中定义：
#ifndef __TIME_T
#define __TIME_T
typedef  long  time_t;
#endif
可见，time_t实际是一个长整型。
其值表示为从UTC(coordinated universal time)时间1970年1月1日00时00分00秒(也称为Linux系统的Epoch时间)到当前时刻的秒数。
由于time_t类型长度的限制，它所表示的时间不能晚于2038年1月19日03时14分07秒(UTC)。
为了能够表示更久远的时间，可用64位或更长的整形数来保存日历时间，这里不作详述。
*/

/** A Resource Stub. 
这个Stub数据是在本次表示的对象数据。是这个应用的核心数据。
这个数据包含了：
1）本身的数据信息。
2）他所“依赖”的子级数据（Stub），放在List *deps 中 
3）他所需求的父级Stub数据，放在List *reqs 中
*/
typedef struct _Stub {  /* ... the local representation of a resource（在der_client.md文档中定义） */
  Resource base; ///< is a container for the resource  这个Stub所要代表的数据本身，一个数据容器
  void *conn; ///< is a pointer to an SeConnection 这个Stub的关联的连接
  /*除了0和-1之外，剩下的就是常规的HTTP状态数据，比如200。
  标记成-1，是表示刚刚已经发出获取数据的请求，但是还未获取到数据。
  等到HTTP回复的时候，该值将被替换成HTTP响应代码。
  在新建Stub的时候，该status值是0，表示Stub是“空的”，还需要真实的值来填充。
  */
  int status; ///< is the HTTP status, 0 for a new Stub, -1 for an update。status是0的话，就表示这个Stub是刚刚创建的，还未填充过数据。
  time_t poll_next; ///< is the next time to poll the resource 下次需要检索数据的时间。通常是需要访问网络的。初始值总是0。
  int16_t poll_rate; ///< is the poll rate for the resource 这个资源应当执行的查询频率
  unsigned complete : 1; ///< marks the Stub as complete  。默认值是0。在创建该Stub的时候，该值是0。
  /*表示这个stub已近查询( retrieve ) 或者填充完毕了。包含子级的Stub数据。
  当有新的子级资源与该本级Stub建立依赖关系后，该值将标记为0，表示当前子级资源还未被请求完毕，即未被满足。
  如果后面全部从服务器侧获取到了数据，则将标记为1。
  该值可以在 dep_reset 中清除为0。
  在dep_complete函数中，只有在complete这flag为0的情况下，才能执行completion函数，然后立即置位。即每次资源齐备之后，只能执行一次completion函数。*/
  unsigned subscribed : 1;
  //下面两个flag数据的用法，详见 new_dep 这个函数 
  uint32_t flag; ///< is the marker for this resource in its dependents 新Stub的默认值是0。
  /*当本Stub作为一个子级Stub，对于一个子级资源（Stub）来说，这个flag表示的是自己是否存在的一个特定标志。
  通常以 SE_##type##Link_exists 这个宏来表达，type是全部资源类型名（在se_types.h中定义）*/
  uint32_t flags; ///< is a bitwise requirements checklist 
  /*一个bit位表示的对下级资源的“需求”表示，置位则表示还缺这个bit代表的资源，一旦该资源从服务器上获取到了，则清除。0表示全部满足了。
  每次在向服务器获取数据之前，函数new_dep都会在某个bit上标记一下（每一个成员都有一个特定的bit位来表达自己）*/
  uint32_t offset; ///< is the offset used for list paging 对于List类型的数据元素来说，offset表示本Stub在整个List中的序号
  uint32_t all; ///< is the total number of list items  List的总量
  struct _Stub *moved; ///< is a pointer to the new resource 这个可能跟资源的重定向有关。应该是资源重定向之后的对象位置，包含了最新的URL。
  List *list; ///< is a list of old requirements for updates 在update_resource函数中，用来备份reqs列表，表示在更新资源之前旧的reqs数据。
  List *deps; ///< is a list of dependencies 存储的是一个个Stub内容单元，表示的是以本 Stub “依赖” 的 Stub 就是他的父级。 这个是子级Stub指向父级Stub的链接。通常只有一个成员。
  List *reqs; ///< is a list of requirements 需求：就是一个Stub的子级对象，当前Stub所需要的子级对象List。List中的data对象就是子级Stub。这个是父级Stub指向子级Stub的链接。
  union {
    void *context; ///< is a user defined completion context
    List *schedules; //< is a list of schedules for event resources 
    //由于这个Stub结构体兼做程序内部的EventBlock对象，所以包含了一个schedules数据。跟前面的一些数据之前没有紧密关联。
  };
  void (*completion) (struct _Stub *); ///< is a user defined completion routine
} Stub;


/** @brief Get an IEEE 2030.5 resource.
    @param conn is a pointer to an SeConnection
    @param type is an IEEE 2030.5 schema type  在se_types.h这个文件中定义好了的一个值，见文件最后的一系列宏定义。
    @param href is URI string, the location of the resource
    @param count is the number of items to get for %List resources, should be 0
    for non-List resources
    @returns a pointer to the Stub for the resource
*/
Stub *get_resource (void *conn, int type, const char *href, int count);

/** @brief Get a subordinate（n. 部属，下级；从属，次要） resource given a parent object. 在给出了一个父级Stub之后，获取到一个子级Stub。

    子级资源通过在父级对象中的链接来检索到。
    Subordinate resources are retrieved from links in the parent object.
    
    Links follow a naming convention（名称规范）, they are named after the type of the
    subordinate resource with the string "Link" appended. This macro
    provides a convienent way to retrieve a subordinate resource, taking
    advantage of the naming pattern used to link subordinate resources from
    their parent object.
    这个宏定义提供了一个很方便的方法来检索到一个子级资源，利用了在链接子层资源中的“naming pattern”优势。
    
    @param conn is a pointer to an SeConnection
    @param obj is a pointer to an IEEE 2030.5 object 一个IEEE对象
    @param type is the type name of the subordinate resource 统一定义的一个值。这里指的是上面的obj对象中的其中的一个资源 (Resource) 。
    @returns a pointer to the Stub for the resource if the link exists,
    NULL otherwise

    SE_##type 最后构成了一个宏定义。在 se_types.h 这个文件中定义了全部相关的资源的名称。
    比如SE_Reading。

    这个宏的意思是，获取到包含在 obj 对象中的type类型资源，比如 dcap 中的 Time 资源。在获取之前，将首先判断一下这个资源链接是否存在。
    
*/
#define get_root(conn, obj, type)				\
  (se_exists (obj, type##Link)?					\
   get_resource (conn, SE_##type, (obj)->type##Link.href, 0) : NULL)


/** @brief Get a subordinate %List resource given a parent object. 在给出父级Stub之后，通过网络访问来获取到子级Stub（List类型的）

    在给出了一个父级的对象后，获取到一个下层的 List 资源。跟上面的get_root不同，这里是“List类型的资源”
    
    The same as @ref get_root but for subordinate %List resources. Links to
    %List resources include the "all" parameter, this indicates the number of
    items contained in the %List resource and is used as a query parameter in
    the HTTP GET request.
    
    通过提供在GET请求中的all参数，可以获取到一个list中的全部资源。在前面的get_root中，该all的值是0，而这里总是大于0。
    
    @param conn is a pointer to an SeConnection
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the type name of the subordinate resource
    @returns a pointer to the Stub for the resourceif the link exists,
    NULL otherwise
*/
#define get_list_root(conn, obj, type)				\
  (se_exists (obj, type##Link)?					\
   get_resource (conn, SE_##type, (obj)->type##Link.href,	\
		 (obj)->type##Link.all) : NULL)

/** 
    获取一个下一层的 resource Stub 并且让父级resource变成一个“依赖”，即子级Stub依赖于父级Stub。

    Get a subordinate resource Stub and make the parent resource a dependent （依赖），即
    父层resource将构建一个bit位表示法来代表对于下层资源的需求，如果这些资源被满足，
    那么就将这些bit位清零。

    创建一个双路的链接，连接父级和子级资源。
    父级资源存储了一个bit位表示的“需求”，代表了对子级资源的需求。如果对某一个子级存在需求，则将
    该bit位置位。当该子级资源被获取到之后，则将该bit位清零。

    本函数基于前面的get_root函数，增加了建立依赖功能（ 参考 der_client.md 文档中的说明 ）
    
    Creates a two way link between the subordinate and parent resources, the
    parent stores a bit representing the requirement for the subordinate
    resource and the subordinate resource clears this bit once all of its
    requirements are met (full retrieval of itself and all of its subordinates).
    
    @param r is a pointer to a Stub, the parent resource r 指向一个Stub，是一个父级资源。
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the type name of the subordinate resource
    @returns a pointer to the subordinate resource Stub


    用SE_##type##Link_exists这个构建一个flag，表示该资源存在。
    
*/
#define get_dep(r, obj, type)					\
  new_dep (r, get_root ((r)->conn, obj, type), SE_##type##Link_exists)

/** @brief Get a subordinate %List resource Stub and make the parent resource a
    dependent.

    获取一个子级的“List”资源Stub并且将父级资源设置成为一个 “依赖” 。
    
    Performs the same function as @ref get_dep except for %List resources.

    跟前面的get_list_root这个函数功能接近，除了获取的内容是一个“List”内容（前面的那个获取的不是List，而是一个单个资源）。
    
    @param r is a pointer to a Stub, the parent resource
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the type name of the subordinate resource
    @returns a pointer to the subordinate resource Stub
    

    这个函数名可以理解成 get list resource depends on <r> , converter to <obj> , which type is <tpye>
    在这个函数执行过程中，第一步先建立依赖关系，即知道谁是自己的父级。然后再发出请求。
    在后续以异步的方式收到数据之后，就能根据XML数据中携带的路径信息，找到原先建立的数据对象，然后再得到依赖关系，对其父级执行complete值的修正。
    路径是每一个资源的唯一标识符，用它来找到对应的资源在本地的存储位置。
    在发出数据请求之前，该资源（Stub）是从属于谁的，就已经定好了。这个定的过程，是人为控制的，程序作者保证了各个层级之间的对应关系。
    
*/
#define get_list_dep(r, obj, type)				\
  new_dep (r, get_list_root ((r)->conn, obj, type), SE_##type##Link_exists)


/** @brief Dependency function  依赖函数？？ 这个就是前面说明中讲到的“在完成了数据获取之后讲执行一个依赖函数，以Stub作为参数”。*/

typedef void (*DepFunc) (Stub *s);

/** @brief Process HTTP data from an SeConnection.

    For each retrieved resource that matches a pre-existing Stub resource,
    store the resource and process it with the dependency function to retrieve
    any subordinate resources and create the requirement/dependency
    relationship.
    
    对于每一个已经被检索到的，且匹配之前已经存在的Stub的资源，存储这个资源并处理使用dependency函数来检索任意一个子层资源并创建“需求/依赖”关系。

    @param conn is a pointer to an SeConnection
    @param dep is a pointer to a DepFunc
 */
int process_http (void *conn, DepFunc dep);

/** @} */

/* 看起来像是要获取一个 List 资源中的部分或者全部。
count表示了需要获取的数量
*/ 
void get_seq (Stub *s, int offset, int count) {
  char *name = resource_name (s);
  if (count) {  //如果count大于0，通常是一个List。
    char uri[64];
    if (count > 255) count = 255;
    if (offset) sprintf (uri, "%s?s=%d&l=%d", name, offset, count);
    else sprintf (uri, "%s?l=%d", name, count); // 在GET请求中，加入了查询数量
    http_get (s->conn, uri);
  } else http_get (s->conn, name);
  set_request_context (s->conn, s); 
  /*看起来是这么一回事情：在发送请求的时候，设置好了这个 Context，
  而由于在这个应用用，HTTP的发送和接收是过程是异步处理的，所以在后面接收到了这个HTTP回复的数据之后，可以通过这个context来知道该怎么处理。*/
}


//增加一个依赖关系。Stub *r是父层级资源，Stub *d是下一层级资源。子级Stub中的deps中记录要“依赖”的父级Stub。
void add_dep (Stub *r, Stub *d) {
  LOG_I("add_dep : r : %s , complete flag set to 0 , d : %s\n",r->base.name,d->base.name);
  d->deps = insert_unique (d->deps, r); //在子级d的“依赖”表中，加入通过deps这个表，能够找到每一个父级Stub。
  d->poll_rate = min (d->poll_rate, r->poll_rate);  //设定好子级 Stub 更新频率
  r->complete = 0;  
  /*一个新加入的Stub由于目前还未被查询过以及没有被执行过completion函数，所以，此时complete的值表示“未完成”。
  等到所缺少的Stub从服务器获取到了之后，则标记为1。*/
}


/*
新建一个 dependency。
Stub *r是父层级资源，Stub *d是下一层级资源。 flag用来标志代表d自己的唯一标识。
flag 通常是 SE_##type##Link_exists 形式的。
这个函数主要作用是在父级Stub中的flag上做一个标记--将某一个bit置位，来表示父级Stub需要某一个特定的子级Stub来填充，
以便后面通过请求该资源来填充到该资源，然后将flags清零。
这个函数只有在 get_dep 和 get_list_dep 中用到，用来在向服务器请求数据之前线做好标记。
*/
Stub *new_dep (Stub *r, Stub *d, int flag) {
  if (d) {
    add_dep (r, d);
    d->flag = flag; //标记本Stub数据，是一个唯一标记。
    r->flags |= flag; 
    /*在父级Stub中标记上，表明父级Stub（参数中的r）要用到该子级Stub(参数中的d)资源（某个bit位置位）。
      后续将通过网络请求来获取到这项数据。目前仅仅是标记了一下。在获取到之后，在dep_complete函数中将标记清零。
      一个数据项目的定义的成员，往往是要多于实际的成员，所以这里相当于是建立一个需求，然后再去满足。*/
  }
  return d;
}

//在Stub r中移除Stub s 这个父级的dep资源。所以，移除“依赖”需要从“父级”那边的链接开始，移除父级指向子级的链接。 
void remove_req (Stub *s, Stub *r) {
  LOG_I("remove_req : remove s(child) : %s from r(parent) : %s\n",s->base.name,r->base.name);
  r->deps = list_delete (r->deps, s); //本地的deps（即子级资源）将被立即就地删除数据，而不用再去发送请求到服务器。
  if (!r->deps) insert_event (r, RESOURCE_REMOVE, 0); 
  /*异步调用。删除当前的s之后，如果deps表变成了空表，那么就请求服务器去删除本Stub。
  服务器也作同样的事情，即自动删除本级Stub以下的Stub和本级Stub。*/
}

/*移除本级和以下的子级Stub成员*/
void remove_reqs (Stub *s, List *reqs) {
  LOG_I("remove_reqs : href : %s\n",s->base.name);
  List *l;
  foreach (l, reqs) {
    remove_req (s, l->data);
  }
  free_list (reqs);
}

/*函数功能：在“各个”父级Stub中移除本Stub资源（链接）。父级Stub对于子级Stub来说就是 dependent
所以函数名 remove_deps 的意思是：从所有原本“req”本Stub的Stub中移除链接。

参数中的deps在实际调用的时候，通常是s对象中的那个deps。
*/
void remove_deps (Stub *s, List *deps) {
  LOG_I("remove_deps : href : %s\n",s->base.name);
  List *l;
  foreach (l, deps) { //遍历每一个依赖对象，即父级Stub
    Stub *t = l->data;//依赖对象，即父级Stub
    if (t->status < 0)  //通常此时status=-1，表示向服务器发出的数据还在路上，要等待其回来。发出请求的原因是首次请求或者更新数据。
      t->list = list_delete (t->list, s);     //在父级Stub中的list中移除本Stub。
    else t->reqs = list_delete (t->reqs, s);  //从父级Stub中的reqs列表中移除本Stub。reqs是父级Stub中记录的“需要”的子级stub的一张表。
  }
  free_list (deps);
}


//从数据库中找回之前存储的某一个数据。这里的name参数应该是某个资源的URL。
void *find_stub (Stub **head, char *name, void *conn) {
  if (*head = find_resource (name)) {   //通过哈希表来寻找到这个资源。相当于说资源和URL是一一对应的关系。
    Stub *s;
    foreach (s, *head) if (s->conn == conn) return s;
  }
  return NULL;
}


//将已经存储了的数据取出来，如果之前没有存储，那么就新建一个，并且存储到数据库中（hash表）。后续可以从服务器侧去获取。
void *get_stub (char *name, int type, void *conn) {
  /*首先在哈希表中寻找*/
  Stub *head, *s = find_stub (&head, name, conn); //*head原本是一个空的指针，在调用了find_stub之后，就有了特定含义。
  if (!s) {/*如果没有找到那么就新建一个Stub，数据为空*/
    s = new_resource (sizeof (Stub), name, NULL, type); //新建的时候，数据是空的。
    s->conn = conn;
    s->poll_rate = 900; //poll_rate 默认是900？
    if (head) link_insert (list_next (head), s);  //此时数据为空
    else insert_resource (s);
  }
  return s; //无论在哈希表中找到没有，都返回找个对象
}


//将一个Stub下面的所有的Request全部移除掉
void delete_reqs (Stub *s) {
  LOG_I("delete_reqs : href : %s\n",s->base.name);
  remove_reqs (s, s->reqs);
  remove_reqs (s, s->list);
  s->reqs = s->list = NULL;
}

/*建之前存储在数据库中的Stub资源删除掉？？*/
void remove_stub (Stub *s) {
  Stub *head = find_resource (s->base.name),
        *t = list_remove (head, s); //head返回这个资源所在的哈希表的list（可能list中只有他单个成员）。t返回删除了s之后的list。
  if (t) {
    if (t != head) insert_resource (t); //这个是什么意思？？
  } else delete_resource (head);  //如果该list只有他一个成员，那么在前面的list_remove之后，哈希表中就不存在该成员了，要从哈希表中移除。
  if (s->moved) remove_req (s, s->moved);
  else delete_reqs (s);//删除掉相关的“依赖”和“需求”
  remove_deps (s, s->deps); //在本地数据库或者说数据网络中删除掉该资源以及相关的依赖关系。
  remove_event (s);
  free_resource (s);
}

/*
参数中的 info 用于下面的比较函数，用于确定数据插入的位置。
函数功能：在list中插入List性质的s对象，以info数据作为排序依据。
*/
void *insert_stub (List *list, Stub *s, ListInfo *info) {
  List *prev = NULL, *l = list, *n;
  void *data = resource_data (s);
  if (find_by_data (list, s)) return list;  //如果这个list已经存在了，那么就不用插入了。
  while (l) {
    Resource *r = l->data;
    if (compare_keys (data, r->data, info) < 0) break;
    prev = l, l = l->next;
  }
  n = list_insert (l, s);
  if (prev) prev->next = n;
  else list = n;
  return list;
}


/*发送消息到服务器，要求删除该资源*/
void delete_stub (Stub *s) {
  http_delete (s->conn, resource_name (s));
  set_request_context (s->conn, s); //凡是服务器需要回复的，都要先设置好context。
}

/* 
主要是为了执行completion函数，以及将本Stub于其自己或者父级构建req/dep逻辑连接关系。

看起来是将一个Stub填充到其父级中去。
其中又分两种情况：一种是父级是一个List类型的数据；一种不是List类型。处理方式不同。
这里的complete的意思不是全部的成员都“到齐”了，而是之前主动请求的成员此时全部到齐了。那些之前没有主动去请求的，显然不会自己来。
每一个资源都需要之前在代码中主动去请求。


在什么情况下调用本函数？
在list_object或者update_existing中，当一个子级成员自身已经齐备了，则此时调用该函数。
该函数通过deps连接访问到其父级，在父级的reqs链接中，将子级加入进去，这样就成了reqs一员。
每次加入的时候，将检查父级是否全部成员都到齐了（无论是list类型的或者是非list类型）。
如果齐了，那么就往上走一个层，到达父级，再次执行本dep_complete函数。这样的过程可以持续，直到到达顶层。

*/
void dep_complete (Stub *s) {
  List *l;
  static int enter_counter=0;
  LOG_I("dep_complete(%d) : resource:%d(%s),href:%s , completion:%s , complete:%d\n",
    ++enter_counter,s->base.type,se_names[s->base.type],s->base.name,((s->completion != NULL )? "set" : "null"),s->complete);
  if (s->completion && !s->complete){  //complete设置为0，表示之前是不“齐备”的。而现在
    LOG_I("  dep_complete(%d) : call completion function\n",enter_counter);
    s->completion (s);  //每一个资源的定制的回调函数。
  }
  s->complete = 1;  //表示本资源已经满足了。在调用本函数之前，应该是在请求或者更新该资源，之前设置的是complete值为0。
  foreach (l, s->deps) {  //可能存在多个父级。从不同的GroupLevel的资源可能指向同一个底层资源。
    Stub *d = l->data;  //d这里是父级，s这里是子级（传输的s对象）。 后面的代码，都是站在父级Stub的角度对当前的这个Stub来操作。
    LOG_I("  dep_complete(%d) : adding reqs : d(parent) : %s with s(child) : %s\n",enter_counter,d->base.name,s->base.name);
    int complete = 0; //0表示未完成。
    if (d->base.info) { //如果这个父级是一个List类型的数据
      d->reqs = insert_stub (d->reqs, s, d->base.info); //那么本Stub数据作为reqs成员的一员，插入到reqs表中。
      complete = list_length (d->reqs) == d->all; //看是否已经完成了。
    } else {  //否则，表示insert_unique父级资源不是一个List性质的资源，那么就查看各个子级资源是否已经齐备。
      d->reqs = insert_unique (d->reqs, s);
      d->flags &= ~s->flag; //将父级的表示 “dep” 的flags中的，用来表示“自己是否存在”的标志位清零。某一个bit置位表示还缺该bit代表的资源，或者说前面已经去向服务器获取了，在new_dep函数中。
      complete = !d->flags; //如果父级中的flags有任何一个bit置位了，则 complete =0，即表示没有完成。如果全部bit都清除掉了，则表示全部获取到了。
    }
    if (complete) { //如果对于父级来说，所有在子级都到齐了，那么就往上走，在父级上执行本函数。这样循环向上，直到顶端。
      //LOG_I("  dep_complete(%d) : complete=1,excute dep_complete recursivly\n",enter_counter);
      if (d->list) {  //list中存储的应该是之前准备要去更新的内容，而现在由于本数据到位了，所以从这个表中移除。 
        LOG_I("  dep_complete(%d) : call remove_reqs\n",enter_counter);
        remove_reqs (d, list_subtract (d->list, d->reqs));  //看起来像是这一次的获取中，少了几个成员，这样，之前旧的成员就要删除掉。实现本地和服务器端的同步。
        d->list = NULL; //在调用dep_complete的时候，数据都到齐了，所以之前多出来的请求要都要删除掉。
      }
      dep_complete (d); //如果父级在本Stub补充上去之后变得齐备了，那么继续往上追溯。比如当前是EndDevice对象，接下去是EndDeviceList对象。
    }
  }
  //LOG_D("dep_complete(%d) exit,resource:%d(%s)\n",--enter_counter,((Resource*)s)->type,se_names[((Resource*)s)->type]);
  --enter_counter;
}

/* 
这个函数名的意思是，在本Stub对象的父级对象中，将连接本Stub的连接断开，同时再往上迭代，直到最高层级的父级的Stub。
本级以及往上的各个层级都设定成“不完整”。

这个函数名中的reset，指的是整体复位。
在本系统中，所有的Stub构建成一个"金字塔"形状的结构。现在中间或者底层的某个成员需要更新，则依赖该成员的所有对象，都要更新一遍。
*/
void dep_reset (Stub *s) {
  static int depth=0;
  depth++;
  LOG_I("dep_reset (%d): %s (%s) complete = 0\n",depth,s->base.name,se_names[s->base.type]);
  List *l;
  s->complete = 0;  //本函数的主要目的，将本Stub标记为“不完整”
  foreach (l, s->deps) {  //"跳跃到"父级，从父级中去掉子级这个 reqs 链接 。 有些情况下可能存在多个deps成员。
    Stub *d = l->data;    //d是父级对象。
    if (d->base.info){    //如果父级Stub是一个List性质的Stub对象，那么从父级对象指向本Stub的链接删除。
      LOG_I("  dep_reset (%d) : d(parent) %s delete reqs s(child) : %s\n",depth,d->base.name,s->base.name);
      d->reqs = list_delete (d->reqs, s); //从父级的reqs表中删除本Stub
    }else{
      LOG_I("  dep_reset (%d) : d(parent) %s set flag of s(child) : %s\n",depth,d->base.name,s->base.name);
      d->flags |= s->flag; //如果不是List性质的Stub对象，那么就直接将表示该资源的bit位置位，表示需求该资源。flags为0表示全部子级都到位了。
    }
    dep_reset (d);    //往父级方向迭代。最终的结果是本Stub网上的对象都设定成complete值为0，表示有资源未到位。
  }
  //LOG_D("dep_reset (%d): exit\n",--depth);
  --depth;
}

/* 通过网络访问来更新某一个资源 ，在更新这个资源的时候，将做几件事情：
1）断开跟子级Stub之间的连接，清空了reqs。但是跟父级之间的连接依然保持。List资源中，本级和子级资源总是一起到来的。
2）如果不是首次去获取数据，且数据不是Event数据，那么就将自己以及上级的complete标志都清零，表示这些资源待更新。如果是首次去获取数据
或者该资源是event类型资源，那么仅仅将自己的complete清零即可，不涉及到上级。
3）去获取资源。这个是立即执行的，不判断是否到了更新时间。


注意：这个函数跟前面的poll_resource不同，这个是立即去执行，而不像poll_resource一样判断是否到达了更新时间。

*/
void update_resource (Stub *s) {
  if (s->status >= 0) { /*
    status =0表示这个Stub是新建的，内部的数据是空的。>0表示数据已经回复了，由HTTP状态码来对其赋值。其实还有比如200这样的表示http回复的值。
    如果当前值是-1，则不会执行请求（请求的结果还未到达，可能还在路上）。
    即仅仅对刚刚新建的，或者之前已经获取过的资源（旧的资源）执行请求。*/
    LOG_I("update_resource (s->reqs = NULL): %s\n",s->base.name);
    if (s->all) s->offset = 0;//重新获取全部成员。
    s->list = s->reqs;        //将旧的请求先备份起来。s->list仅仅是为了备份。
    s->reqs = NULL;           //清空"需求"对象表，也就是子级资源。
    
    /*如果s->status大于0(前面已经判断过大于等于0了)，表示这个Stub不是一个“新建”的Stub，
    且Stub对象不是一个event对象，那么就执行下面的dep_reset函数。
    意思就是说，如果对一个旧的Stub执行更新，那么将其上层级的所有关联Stub都设定为complete为0，表示需要等待更新*/
    if (s->status && !se_event (resource_type (s))){ 
      LOG_I("  update_resource : call dep_reset\n");
      dep_reset (s);//如果是大于0的，即该资源之前已经请求过了，现在属于更新，且不属于SE_event类型资源，那么reset一下。如果是首次，不用reset。
    }else{  //其他情况比如说这个资源是新建的，则统一将自己这个Stub的complete标记成0，而不涉及到其他Stub。这样的话，在该资源的子级资源都到位后，将触发调用completion函数。
      LOG_I("  update_resource : set Stub self complete flag to 0\n");
      s->complete = 0; //仅仅将自己标记成“未完整”。
    }
    s->status = -1; //在发出数据请求的时刻，将其修改成-1，表示请求刚刚发出，还在路上。等待回来之后，将根据HTTP的状态码来赋值。
    get_seq (s, 0, s->all); //发生网络访问
  }
}


/* 直接返回一个Stub下面的某一个类型的Stub。这个Stub是已经查询完备了的。 */
void *get_subordinate (Stub *s, int type) {
  List *l;
  foreach (l, s->reqs) {
    Stub *t = l->data;
    if (resource_type (t) == type)
      return t->moved ? t->moved : t; //如果moved存在就使用moved资源，否则返回原来的？？（不知道什么意思）
  }
  return NULL;
}


//从服务器获取一个资源。先连接到服务器，然后再获取stub资源。count的值有些情况下可以设置成0。
Stub *get_resource (void *conn, int type, const char *href, int count) {
  LOG_I("get_resource : href:%s\n",href);
  Stub *s;
  Uri128 buf;
  Uri *uri = &buf.uri;
  if (!http_parse_uri (&buf, conn, href, 127)) return NULL;
  if (uri->host) conn = se_connect_uri (uri); //首先是连接到服务器
  s = get_stub (uri->path, type, conn);   //无论这个Stub是否已经存在，这个函数总是会返回一个Stub对象（有可能是空的）
  if ((time (NULL) - s->base.time) > s->poll_rate) {  /*如果当前时间已经超过了poll_rate，即表示这个资源需要更新了，发出异步请求更新一次。*/
    s->all = count;   //如果Stub之前是不存在即使新建的（空的），那么 s->base.time 就是 0 。
    update_resource (s);  //在这个函数中去获取到资源（将访问网络），获取到之后，填充到哈希表中去。但是这个操作是异步访问网络。
  }else{
    LOG_D("  get_resource : time no up,exit\n");
  }
  return s;
}

/*
插入一个周期性事件，将在next时间点上去请求该资源。
*/
void poll_resource (Stub *s) {
  LOG_I("poll_resource : type:%d(%s),herf:%s\n",s->base.type,se_names[s->base.type],s->base.name);
  time_t now = se_time ();
  time_t next = now + s->poll_rate;
  
  //如果是event类型的数据，则需要定时的查询一下。
  if (se_event (resource_type (s))) { 
    SE_Event_t *ev = resource_data (s); 
    time_t end = ev->interval.start + ev->interval.duration;
    if (s->poll_rate >= (end - now)) {
      LOG_I("  poll_resource : s->poll_rate >= (end - now),end:%ld,now:%ld,return\n",end,now);
      return;
    }else{
      LOG_I("  poll_resource : s->poll_rate < (end - now),end:%ld,now:%ld\n",end,now);
    }
  }
  
  //如果现在时间已经超过预定要执行轮询的时间了（poll_next），则放到event poll queue中，将会异步的执行一次轮训。
  //对于一个Stub来说，poll_next值通常是0，所以首次获取到该数据之后通常是满足的下面的判断条件的。
  if (s->poll_next <= now) {
    s->poll_next = next;
    //这里的类型RESOURCE_POLL，跟在der_poll中调用这个函数的时候的case值严格一致了。
    LOG_I("  poll_resource : insert_event RESOURCE_POLL , next time : %ld\n",(int64_t)next);
    insert_event (s, RESOURCE_POLL, next);  //添加一个“延迟执行”的任务，将在next时刻到了之后执行。
  }
}


/*获取dcap资源*/
Stub *get_dcap (Service *s, int secure) {
  LOG_I("get_dcap\n");
  void *conn = service_connect (s, secure);
  return get_resource (conn, SE_DeviceCapability, service_dcap (s), 0);
}

/*通过之前dnsmd查询到的数据，来获取到路径*/
Stub *get_path (Service *s, int secure) {
  void *conn;
  int type, count;
  char *path = service_path (s);
  if (!path) return get_dcap (s, secure);
  conn = service_connect (s, secure);
  type = service_type (s);
  count = se_list (type) ? 255 : 0;
  return get_resource (conn, type, path, count);
}

//在收到服务器的数据之后，首次填充或者更新掉这个数据。
void update_existing (Stub *s, void *obj, DepFunc dep) {
  Resource *r = &s->base;
  LOG_I("update_existing : type:%d(%s) , href : %s\n",r->type,se_names[r->type],r->name);
  List *l;
  if (!r->data) r->data = obj;  //如果该项数据原先不存在，那么就直接填充。
  else if (se_event (r->type)) {  //如果不为空，且类型为 SE_Event
    LOG_I("  update_existing : s is an event type\n");
    SE_Event_t *ex = r->data, *ev = obj;
    memcpy (&ex->EventStatus, &ev->EventStatus,
            sizeof (SE_EventStatus_t));//如果是一个已经存在的 SE_Event 类型的数据，那么仅仅更新Status数据就够了。应该是对于SE_Event_t，数据更新的只有Status。
    free_se_object (obj, r->type);
  } else replace_se_object (r->data, obj, r->type); //其他的类型的数据如果已经存在的话，就直接替换。
  LOG_I("  update_existing : call dep(s)\n");
  dep (s);  //dep函数都要执行一遍。但是未必对这个数据执行什么操作。
  if (!s->flags) {  //如果这个资源的所有子级都是到齐的，那么才能执行下面的 dep_complete 函数 。 在 list_object 中，也同时判断和调用该 dep_complete 函数。
    LOG_I("  update_existing : s->flags == 0,call dep_complete\n");
    dep_complete (s);  //如果flags等于0，表示该Stub的所有之前向服务器发出的“需求”都被满足了(仅仅是发出的，未必是全部的成员)。此时执行dep_complete函数。
  }else{
    LOG_I("  update_existing : s->flags != 0 , exit\n");
  }
}

// update paging and return number of items needed 返回目前还需要的list对象中的“个数”
int list_seq (Stub *s, void *obj) {
  int results;
  if (se_type_is_a (s->base.type, SE_SubscribableList)) { //判断s->base.type的基类是否是 SE_SubscribableList。现在我们用到的List基本上是这个（根据IEEE文档）
    SE_SubscribableList_t *sl = obj;
    s->all = sl->all;
    results = sl->results;
  } else {
    SE_List_t *sl = obj;
    s->all = sl->all;
    results = sl->results;
  }
  // printf ("list_seq %d %d %d\n", s->offset, results, s->all);
  s->offset += results;
  return s->all - s->offset;  //返回还有多少个没有获取到
}

//每一个对象都有一个唯一的路径。取出该对象绑定的那个路径。
char *object_path (Uri128 *buf, void *conn, void *data) {
  SE_Resource_t *sr = data; 
  /*SE数据类型的定义总是遵循一个原则：flag和href总是定义在一个数据的最开始位置，
  所以，这里不用管具体的传入的类型，也可以直接定位到flag和href的地址。href是一个数据指针。*/
  if (sr->href && http_parse_uri (buf, conn, sr->href, 127))
    return buf->uri.path;
  return NULL;
}

extern const char * const se_names[];
// process list object with dependency function    使用"dependency"函数，处理"列表"对象，构建与父级之间的依赖连接关系。 
int list_object (Stub *s, void *obj, DepFunc dep) {
  Resource *r = &s->base;
  int count = list_seq (s, obj);  //得到还剩下多少个没有获取到
  //LOG_I("list_object:left count:%d\n",count);
  List **list = se_list_field (obj, r->info), *input, *l; //获取到对象中的List域，通过info这个用来指示List结构的数据。
  input = *list;  //有可能这个list是一个空的，下面的foreach不会执行。
  *list = NULL;
  LOG_I("\n** list_object : resource type:%d(%s),href:%s\n",r->type,se_names[r->type],s->base.name);
  if (!r->data) r->data = obj;  //如果之前是空的，那么就新建
  else replace_se_object (r->data, obj, r->type); //如果之前已经存在的，那么就替换。
  LOG_I("  list_object : dep(s)...\n");
  dep (s);  //这个List作为一个整体对象，执行一遍对这个List的dep函数。在我们的本demoe代码中，没有执行 EndDeviceList 的操作。
  
  //接下去是对每一个List成员执行操作。构建List对象和其下的子级资源之间的dependent关系。先处理每一个成员。
  foreach (l, input) {
    Uri128 buf;
    char *path;
    if (path = object_path (&buf, s->conn, l->data)) {  //取出URL路径
      LOG_I("  \n* list_object : list member path : %s,call update_existing\n",path);
      Stub *d = get_stub (path, r->info->type, s->conn); //此时这个对象通常是已经建立完毕了的，这里应该是直接获取。
      add_dep (s, d);   //s是父级就是这里的List，d是子级就是List成员，每添加一个d到s，则s的complete将被设定成0，即不完整。
      /*增加依赖关系。s是List对象，是父级，而d是其中的一个子级对象。
      比如EndDeviceList和EndDevice之间的关系，就是s和d之间的关系。刚刚收到并且解析出来的数据，仅仅是无逻辑联系的空白数据而已，
      这里给他加入了逻辑关联。*/
      update_existing (d, l->data, dep);  //通常会继续调用dep_complete函数，在最后一个成员到齐后，父级，就是List对象将被设定成complete为1。
    } else {
      // subordinate resource with no href or invalid href
      LOG_W("  list_object : subordinate resource with no href or invalid href\n");
      free_se_object (l->data, r->info->type);
    }
  }
  free_list (input);  //之前解析的时候，是动态申请的，所以这里释放掉。

  //然后再处理本身这个List对象。
  if (count > 0) get_seq (s, s->offset, count); //有时候，一个list是分批获取的，所以有时候可能count值是大于0的，这种情况下就要继续去获取。
  else if (!s->all) { 
    LOG_I("  list_object : s->all==0,call dep_complete\n");
    dep_complete (s); //如果这个list对象的长度是0，那么此时收到的是一个没有成员而只有头的list，也就到此结束了，不用继续再去获取了。通常不会是0。
  }
  LOG_I("list_object exit , href:%s\n\n",r->name);
  return count;
}

Stub *find_target (void *conn) {
  Stub *head;
  return find_stub (&head, http_path (conn), conn);
}

//当前的Stub是否跟之前请求的Stub对应的上。
Stub *match_request (void *conn, void *data, int type) {
  Stub *s = http_context (conn);  //找出前面发送的那个Stub，以Stub作为context对象。
  Uri128 buf;
  char *path = object_path (&buf, conn, data);
  if (path && streq (resource_name (s), path)) {
    int rtype = resource_type (s);
    if (rtype < 0) resource_type (s) = type;
    else if (rtype != type) return NULL;
    return s;
  }
  return NULL;
}
//处理响应
void process_response (void *conn, int status, DepFunc dep) {
  Stub *s;
  void *obj;
  int type, count = 0;
  switch (http_method (conn)) { //请求类型
  case HTTP_GET:  //对GET请求的处理。GET请求将获取到一个或者多个SE数据，其中可能包含了 List 性质的数据。
    LOG_I("process_response:case HTTP_GET\n");
    if (obj = se_body (conn, &type)) {
      print_se_object (obj, type);
      printf ("\n");
      if (s = match_request (conn, obj, type)) {  //如果现在服务器回复的数据是之前刚刚发出的请求的数据，那么就对应上了。
        s->base.time = time (NULL); //刷新该项数据获取到的时间，注意用的是系统本地时间。
        if (s->base.info) //如果是一个list类型的数据。
          count = list_object (s, obj, dep);  //如果是一个List对象，则处理这个list。返回的count表示还未获取到的成员数量。
        else update_existing (s, obj, dep);   //如果是一个单体对象（非List），那么就更新数据库。
        if (!count) s->status = status; /*
        count如果是0，表示list中的全部成员已经到齐，否则继续维持原来的值（0表示新建Stub，-1表示还在更新中）。
        status存放常规的HTTP响应代码比如200,204等。*/
      } else free_se_object (obj, type);      //如果来的数据不是之前我们请求的数据，则直接放弃。
    }
    break;
  case HTTP_POST:
    LOG_I("  process_response : case HTTP_POST\n");
    if (s = find_target (conn)) {
      if (s->base.info) {
        char *location = http_location (conn);
        int type = s->base.info->type;
        add_dep (s, get_resource (conn, type, location, 0));
      }
    }
    break;
  case HTTP_DELETE:
    LOG_I("  process_response : case HTTP_DELETE\n");
    remove_stub (http_context (conn));  // 如果是移除一个资源，那么就在本地删除之。
    break;
  }
}


/*暂时还不懂这个函数的意思*/
void get_moved (Stub *s, char *location) {
  if (location) {
    Stub *t = get_resource (s->conn, resource_type (s), location, s->all);
    if (s->moved && t && s->moved != t) remove_req (s, s->moved);
    if (t) {
      add_dep (s, t);
      s->moved = t;
    }
  }
  delete_reqs (s);
}

//处理HTTP重定向
void process_redirect (void *conn, int status) {
  Stub *s;
  if (s = find_target (conn)) {
    get_moved (s, http_location (conn));
    s->status = status;
  }
  free_se_body (conn);
}

//处理HTTP回复的数据（看起来像是面向服务器端写的程序）这个程序是是一个收到数据后的统一处理程序。
int process_http (void *conn, DepFunc dep) {
  int status,recv_val;
  Stub *s;
  switch (recv_val = se_receive (conn)) {  //正常情况下，收到数据并解析成SE对象。
  //LOG_I("process_http : se_receive:%d\n",recv_val);
  case HTTP_RESPONSE:
    LOG_I("  process_http : HTTP_RESPONESE\n");
    switch (status = http_status (conn)) {
    case 200:
    case 201:
    case 204:
      printf("    process_http : HTTP_RESPONSE : http_status return:%d\n",status);
      process_response (conn, status, dep); //如果服务器成功回复，则将执行dep函数
      return status;
    case 300:
    case 301:
      process_redirect (conn, status);
      break;
    default:  // 如果是访问资源但是回复的数据不是上述表示成功的情况，则表示该资源（看起来）已经被删除了，所以要再本地也同样的删除掉。
      LOG_I("    process_http : HTTP_RESPONSE : default\n");
      if (http_method (conn) == HTTP_GET
          && (s = find_target (conn))) {
        s->status = status;
        insert_event (s, RETRIEVE_FAIL, 0);
      }
      free_se_body (conn);
    }
    break;
  }
  return 0;
}

void cleanup_http (void *conn) {
  HttpRequest *r = http_queued (conn), *next;
  while (r) {
    next = r->next;
    if (r->method == HTTP_GET)
      remove_stub (r->context);
    free (r);
    r = next;
  }
}
