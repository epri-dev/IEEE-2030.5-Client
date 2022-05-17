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
  int status; ///< is the HTTP status, 0 for a new Stub, -1 for an update。除了0和-1之外，剩下的就是常规的HTTP状态数据，比如200。
  time_t poll_next; ///< is the next time to poll the resource 下次需要检索数据的时间。通常是需要访问网络的。初始值总是0。
  int16_t poll_rate; ///< is the poll rate for the resource 这个资源应当执行的查询频率
  unsigned complete : 1; ///< marks the Stub as complete  
  /*表示这个stub已近查询( retrieve ) 或者填充完毕了。包含子级的Stub数据。当有新的子级资源与该本级Stub建立依赖关系后，该值将标记为0。
  如果还有未完成的数据待获取，则标记为1*/
  unsigned subscribed : 1;
  //下面两个flag数据的用法，详见 new_dep 这个函数 
  uint32_t flag; ///< is the marker for this resource in its dependents 
  /*当本Stub作为一个子级Stub，对于一个子级资源（Stub）来说，这个flag表示的是自己是否存在的一个特定标志。
  通常以 SE_##type##Link_exists 这个宏来表达，type是全部资源类型名（在se_types.h中定义）*/
  uint32_t flags; ///< is a bitwise requirements checklist 
  /*一个bit位表示的对下级资源的“需求”表示，置位则表示还缺这个bit代表的资源，一旦该资源从服务器上获取到了，则清除。0表示全部满足了。
  每次在向服务器获取数据之前，函数new_dep都会在某个bit上标记一下（每一个成员都有一个特定的bit位来表达自己），如果*/
  uint32_t offset; ///< is the offset used for list paging 对于List类型的数据元素来说，offset表示本Stub在整个List中的序号
  uint32_t all; ///< is the total number of list items  List的总量
  struct _Stub *moved; ///< is a pointer to the new resource 这个可能跟资源的重定向有关
  List *list; ///< is a list of old requirements for updates 之前需要准备区获取的数据。list长度于前面的flags中的置位的bit位个数一致。
  List *deps; ///< is a list of dependencies 存储的是一个个Stub内容单元，表示的是以本 Stub “依赖” 的父层级的 Stub 。 这个是子级Stub指向父级Stub的链接。
  List *reqs; ///< is a list of requirements 需求：就是子级对象，当前Stub所需要的子级对象List。List中的data对象就是子级Stub。这个是父级Stub指向子级Stub的链接。
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
  d->deps = insert_unique (d->deps, r); //对d->deps作了更新，保证r资源被插入。通过deps这个表，能够找到每一个父级Stub。
  d->poll_rate = min (d->poll_rate, r->poll_rate);  //设定好子级 Stub 更新频率
  r->complete = 0;  //一个新加入的Stub由于目前还未被查询过以及没有被执行过completion函数，所以，此时complete的值表示“未完成”。
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
  r->deps = list_delete (r->deps, s);
  if (!r->deps) insert_event (r, RESOURCE_REMOVE, 0); //异步调用
}

void remove_reqs (Stub *s, List *reqs) {
  List *l;
  foreach (l, reqs) remove_req (s, l->data);
  free_list (reqs);
}

/*函数功能：在“各个”父级Stub中移除本Stub资源（链接）。父级Stub对于子级Stub来说就是 dependent
所以函数名 remove_deps 的意思是：从所有原本“req”本Stub的Stub中移除链接。

参数中的deps在实际调用的时候，通常是s对象中的那个deps。
*/
void remove_deps (Stub *s, List *deps) {
  List *l;
  foreach (l, deps) { //遍历每一个依赖对象，即父级Stub
    Stub *t = l->data;//依赖对象，即父级Stub
    if (t->status < 0)  //准备将要去更新的？
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
  remove_reqs (s, s->reqs);
  remove_reqs (s, s->list);
  s->reqs = s->list = NULL;
}

/*建之前存储在数据库中的Stub资源删除掉？？*/
void remove_stub (Stub *s) {
  Stub *head = find_resource (s->base.name),
        *t = list_remove (head, s); //返回删除了s之后的list。
  if (t) {
    if (t != head) insert_resource (t); //这个是什么意思？？
  } else delete_resource (head);
  if (s->moved) remove_req (s, s->moved);
  else delete_reqs (s);//删除掉相关的“依赖”和“需求”
  remove_deps (s, s->deps);
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

/* 看起来是将一个Stub填充到其父级中去。其中又分两种情况：一种是父级是一个List类型的数据；一种不是List类型。处理方式不同。
这里的complete的意思不是全部的成员都“到齐”了，而是之前主动请求的成员此时全部到齐了。那些之前没有主动去请求的，显然不会自己来。
每一个资源都需要之前在代码中主动去请求。
*/
void dep_complete (Stub *s) {
  List *l;
  LOG_I("dep_complete,resource:%d(%s)\n",((Resource*)s)->type,se_names[((Resource*)s)->type]);
  if (s->completion && !s->complete){  //complete设置为0，表示之前是不“齐备”的，也就是之前还没有执行过 completion 函数。
    LOG_I("dep_complete:excuting completion function\n");
    s->completion (s);  //如果completion函数之前设置过（现在存在），且数据也都获取成功了（complete为0表示不需要再获取什么东西了），则执行completion函数。
  }
  s->complete = 1;
  foreach (l, s->deps) {
    Stub *d = l->data;  //d这里是父级，s这里是子级（传输的s对象）。 后面的代码，都是站在父级Stub的角度对当前的这个Stub来操作。
    int complete = 0; //0表示未完成。
    if (d->base.info) { //如果这个父级是一个List类型的数据
      d->reqs = insert_stub (d->reqs, s, d->base.info); //那么本Stub数据作为reqs成员的一员，插入到reqs表中。
      complete = list_length (d->reqs) == d->all; //看是否已经完成了。
    } else {  //否则，表示insert_unique父级资源不是一个List性质的资源，那么就查看各个子级资源是否已经齐备。
      d->reqs = insert_unique (d->reqs, s);
      d->flags &= ~s->flag; //将父级的表示 “dep” 的flags中的，用来表示“自己是否存在”的标志位清零。某一个bit置位表示还缺该bit代表的资源，或者说前面已经去向服务器获取了，在new_dep函数中。
      complete = !d->flags; //如果父级中的flags有任何一个bit置位了，则 complete =0，即表示没有完成。如果全部bit都清除掉了，则表示全部获取到了。
    }
    if (complete) {
      LOG_I("dep_complete:complete=1,excute dep_complete recursivly\n");
      if (d->list) {  //list中存储的应该是之前准备要去更新的内容，而现在由于本数据到位了，所以从这个表中移除。 
        remove_reqs (d, list_subtract (d->list, d->reqs));
        d->list = NULL; //前面的flags数据全部清零了，表示需要的dep已经全部被满足了。
      }
      dep_complete (d); //如果父级在本Stub补充上去之后变得齐备了，那么继续往上追溯。比如当前是EndDevice对象，接下去是EndDeviceList对象。
    }
  }
  LOG_D("dep_complete exit,resource:%d(%s)\n",((Resource*)s)->type,se_names[((Resource*)s)->type]);
}

/* 这个函数名的意思是，从“dep”对象中，将本Stub的连接断开，同时再往上迭代。 */
void dep_reset (Stub *s) {
  List *l;
  s->complete = 0;
  foreach (l, s->deps) {  //对每一个父级Stub执行操作。
    Stub *d = l->data;    //
    if (d->base.info) //如果父级Stub是一个List性质的Stub对象，那么
      d->reqs = list_delete (d->reqs, s); //从父级的reqs表中删除本Stub
    else d->flags |= s->flag; //如果不是List性质的Stub对象，那么就直接将表示该资源的bit位置位，表示需求该资源。
    dep_reset (d);    //迭代
  }
}

/* 通过网络访问来更新某一个资源 */
void update_resource (Stub *s) {
  if (s->status >= 0) { //is the HTTP status, 0 for a new Stub, -1 for an update
    if (s->all) s->offset = 0;
    s->list = s->reqs;
    s->reqs = NULL;
    if (s->status && !se_event (resource_type (s))) //如果s->status大于0(前面已经判断过大于等于0了)，且Stub对象不是一个event对象，那么就执行下面的dep_reset函数。
      dep_reset (s);
    else s->complete = 0; //？？
    s->status = -1;
    get_seq (s, 0, s->all); //发生网络访问
  }
}


/* 查询某个 Stub 下面的子层设备的某一个type的数据 */
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
  LOG_I("get_resource:href:%s\n",href);
  Stub *s;
  Uri128 buf;
  Uri *uri = &buf.uri;
  if (!http_parse_uri (&buf, conn, href, 127)) return NULL;
  if (uri->host) conn = se_connect_uri (uri); //首先是连接到服务器
  s = get_stub (uri->path, type, conn);   //无论这个Stub是否已经存在，这个函数总是会返回一个Stub对象（有可能是空的）
  if ((time (NULL) - s->base.time) > s->poll_rate) {  /*如果当前时间已经超过了poll_rate，即表示这个资源需要更新了，发出异步请求更新一次。*/
    s->all = count;   //如果Stub之前是不存在即使新建的（空的），那么 s->base.time 就是 0 。
    update_resource (s);  //在这个函数中去获取到资源（将访问网络），获取到之后，填充到哈希表中去。但是这个操作是异步访问网络。
  }
  return s;
}

/*
poll资源，主要是针对 SE_Event_t 数据 。
这个函数添加了一个延时执行任务：insert_event (s, RESOURCE_POLL, next);
看起来他并没有去做向服务器Poll的动作。这里仅仅是添加了一个事件，且建在该资源设定时间点上去执行。
*/
void poll_resource (Stub *s) {
  LOG_I("poll_resource:type:%d(%s)\n",((Resource*)s)->type,se_names[((Resource*)s)->type]);
  time_t now = se_time ();
  time_t next = now + s->poll_rate;
  
  //如果是event类型的数据，则需要定时的查询一下。
  if (se_event (resource_type (s))) { 
    SE_Event_t *ev = resource_data (s); 
    time_t end = ev->interval.start + ev->interval.duration;
    if (s->poll_rate >= (end - now)) return;
  }
  
  //如果现在时间已经超过预定要执行轮询的时间了（poll_next），则放到event poll queue中，将会异步的执行一次轮训。
  //对于一个Stub来说，poll_next值通常是0，所以首次获取到该数据之后通常是满足的下面的判断条件的。
  if (s->poll_next <= now) {
    s->poll_next = next;
    //这里的类型RESOURCE_POLL，跟在der_poll中调用这个函数的时候的case值严格一致了。
    LOG_I("poll_resource:insert_event RESOURCE_POLL\n");
    insert_event (s, RESOURCE_POLL, next);  //添加一个“延迟执行”的任务，将在next时刻到了之后执行。
  }
}


/*获取dcap资源*/
Stub *get_dcap (Service *s, int secure) {
  LOG_I("in function get_dcap\n");
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
  LOG_I("update_existing:type:%d(%s)\n",r->type,se_names[r->type]);
  List *l;
  if (!r->data) r->data = obj;  //如果该项数据原先不存在，那么就直接填充。
  else if (se_event (r->type)) {  //如果不为空，且类型为 SE_Event
    SE_Event_t *ex = r->data, *ev = obj;
    memcpy (&ex->EventStatus, &ev->EventStatus,
            sizeof (SE_EventStatus_t));//如果是一个已经存在的 SE_Event 类型的数据，那么仅仅更新Status数据就够了。应该是对于SE_Event_t，数据更新的只有Status。
    free_se_object (obj, r->type);
  } else replace_se_object (r->data, obj, r->type); //其他的类型的数据如果已经存在的话，就直接替换。
  LOG_I("update_existing:dep(s)...\n");
  dep (s);  //dep函数都要执行一遍。但是未必对这个数据执行什么操作。
  if (!s->flags) dep_complete (s);  //如果flags等于0，表示该Stub的所有之前向服务器发出的“需求”都被满足了(仅仅是发出的，未必是全部的成员)。此时执行dep_complete函数。
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
  LOG_I("list_object:resource type:%d(%s)\n",r->type,se_names[r->type]);
  if (!r->data) r->data = obj;  //如果之前是空的，那么就新建
  else replace_se_object (r->data, obj, r->type); // 如果之前已经存在的，那么就替换。
  LOG_I("list_object:dep(s)...\n");
  dep (s);  //这个List作为一个整体对象，执行一遍对这个List的dep函数。在我们的本demoe代码中，没有执行EndDeviceList的操作。
  foreach (l, input) {  //接下去是对每一个List成员执行操作。
    Uri128 buf;
    char *path;
    if (path = object_path (&buf, s->conn, l->data)) {  //取出URL路径
      LOG_I("list_object:list member path:%s\n",path);
      Stub *d = get_stub (path, r->info->type, s->conn); //这个函数的内在逻辑是：如果这个对象不存在，则新建一个。名字有点迷惑。
      add_dep (s, d); 
      /*增加依赖关系。s是List对象，是父级，而d是其中的一个子级对象。
      比如EndDeviceList和EndDevice之间的关系，就是s和d之间的关系。刚刚收到并且解析出来的数据，仅仅是无逻辑联系的空白数据而已，
      这里给他加入了逻辑关联。*/
      update_existing (d, l->data, dep);
    } else {
      // subordinate resource with no href or invalid href
      LOG_W("list_object:subordinate resource with no href or invalid href\n");
      free_se_object (l->data, r->info->type);
    }
  }
  free_list (input);  //之前解析的时候，是动态申请的，所以这里释放掉。
  if (count > 0) get_seq (s, s->offset, count); //将剩下的没有获取到的再继续获取一下。有可能不需要这个步骤。
  else if (!s->all) dep_complete (s); //如果这个list对象的长度是0，那么此时收到的是一个空的list，也就到此结束了，不用继续再去获取了。
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
          count = list_object (s, obj, dep);  //如果是一个List对象，则处理这个list。
        else update_existing (s, obj, dep);   //如果是一个单体对象（非List），那么就更新数据库。
        if (!count) s->status = status;
      } else free_se_object (obj, type);      //如果来的数据不是之前我们请求的数据，则直接放弃。
    }
    break;
  case HTTP_POST:
    LOG_I("function process_response:case HTTP_POST\n");
    if (s = find_target (conn)) {
      if (s->base.info) {
        char *location = http_location (conn);
        int type = s->base.info->type;
        add_dep (s, get_resource (conn, type, location, 0));
      }
    }
    break;
  case HTTP_DELETE:
    LOG_I("function process_response:case HTTP_DELETE\n");
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
  LOG_I("in function process_http\n");
  int status;
  Stub *s;
  switch (se_receive (conn)) {  //正常情况下，收到数据并解析成SE对象。
  case HTTP_RESPONSE:
    LOG_I("process_http:case HTTP_RESPONESE\n");
    switch (status = http_status (conn)) {
    case 200:
    case 201:
    case 204:
      printf("process_http: se_receive return HTTP_RESPONSE,http_status return status:%d\n",status);
      process_response (conn, status, dep); //如果服务器成功回复，则将执行dep函数
      return status;
    case 300:
    case 301:
      process_redirect (conn, status);
      break;
    default:  // 如果是访问资源但是回复的数据不是上述表示成功的情况，则表示该资源（看起来）已经被删除了，所以要再本地也同样的删除掉。
      LOG_I("process_http:case default\n");
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
