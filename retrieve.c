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
  int status; ///< is the HTTP status, 0 for a new Stub, -1 for an update
  time_t poll_next; ///< is the next time to poll the resource 下次需要检索的数据的时间
  int16_t poll_rate; ///< is the poll rate for the resource 这个资源应当执行的查询频率
  unsigned complete : 1; ///< marks the Stub as complete  表示这个stub已近查询( retrieve ) 或者填充完毕了。包含子级的Stub数据。
  unsigned subscribed : 1;
  //下面两个flag数据的用法，详见 new_dep 这个函数 
  uint32_t flag; ///< is the marker for this resource in its dependents 对于一个子级资源（Stub）来说，这个flag表示的是自己的一个特定标志。
  uint32_t flags; ///< is a bitwise requirements checklist 一个bit位表示的对下级资源的“需求”表示，置位则表示有需求。
  uint32_t offset; ///< is the offset used for list paging 对于List类型的数据元素来说，offset表示本Stub在整个List中的序号
  uint32_t all; ///< is the total number of list items  List的总量
  struct _Stub *moved; ///< is a pointer to the new resource
  List *list; ///< is a list of old requirements for updates
  List *deps; ///< is a list of dependencies 依赖？存储的是一个个Stub内容单元，表示的是以本Stub为依赖对象的子层级的Stub。
  List *reqs; ///< is a list of requirements 要求？
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

/** @brief Get a subordinate（n. 部属，下级；从属，次要） resource given a parent object.

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


/** @brief Get a subordinate %List resource given a parent object.

    在给出了一个父级的对象后，获取到一个下层的List资源。跟上面的get_root不同，这里是带有“List的资源”
    
    The same as @ref get_root but for subordinate %List resources. Links to
    %List resources include the "all" parameter, this indicates the number of
    items contained in the %List resource and is used as a query parameter in
    the HTTP GET request.
    
    通过提供在GET请求中的all参数，可以获取到一个list中的全部资源。
    
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
获取一个下一层的 resource Stub并且让父级resource变成一个“受依赖方”。
    Get a subordinate resource Stub and make the parent resource a dependent（n. 受供养者）.
    父层resource将构建一个bit位表示法来代表对于下层资源的需求，如果这些资源被满足，
    那么就将这些bit位清零。

    创建一个双路的链接，连接父级和子级资源。
    父级资源存储了一个bit位表示的“需求”，代表了对子级资源的需求。如果对某一个子级存在需求，则将
    该bit位置位。当该子级资源被获取到之后，则将该bit位清零。
    Creates a two way link between the subordinate and parent resources, the
    parent stores a bit representing the requirement for the subordinate
    resource and the subordinate resource clears this bit once all of its
    requirements are met (full retrieval of itself and all of its
    subordinates).
    @param r is a pointer to a Stub, the parent resource r指向一个Stub，是一个父级资源。
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


//增加一个依赖关系。Stub *r是父层级资源，Stub *d是下一层级资源。
void add_dep (Stub *r, Stub *d) {
  d->deps = insert_unique (d->deps, r); //对d->deps作了更新，保证r资源被插入。通过deps这个表，能够找到每一个子级Stub。
  d->poll_rate = min (d->poll_rate, r->poll_rate);  //设定好更新频率
  r->complete = 0;  //一个新加入的Stub由于目前还未被查询过，所以此时complete的值表示“未完成”。
}


/*
新建一个 dependency。Stub *r是父层级资源，Stub *d是下一层级资源。flag用来标志代表d自己的唯一标识。
flag 通常是 SE_##type##Link_exists 形式的。
这个函数主要作用是在父级Stub中的flag上做一个标记--将某一个bit置位，来表示父级Stub需要某一个特定的子级Stub来填充，
以便后面通过请求该资源来填充到该资源。
*/
Stub *new_dep (Stub *r, Stub *d, int flag) {
  if (d) {
    add_dep (r, d);
    d->flag = flag; //标记本Stub数据，是一个唯一标记。
    r->flags |= flag; //在父级Stub中标记上，表明父级Stub要用到本Stub资源。后续将通过网络请求来获取到这项数据。目前仅仅是标记了一下。
  }
  return d;
}

//移除 requirements?
void remove_req (Stub *s, Stub *r) {
  r->deps = list_delete (r->deps, s);
  if (!r->deps) insert_event (r, RESOURCE_REMOVE, 0); //异步调用
}

void remove_reqs (Stub *s, List *reqs) {
  List *l;
  foreach (l, reqs) remove_req (s, l->data);
  free_list (reqs);
}

void remove_deps (Stub *s, List *deps) {
  List *l;
  foreach (l, deps) {
    Stub *t = l->data;
    if (t->status < 0)
      t->list = list_delete (t->list, s);
    else t->reqs = list_delete (t->reqs, s);
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
  set_request_context (s->conn, s);
}

/**/
void dep_complete (Stub *s) {
  List *l;
  if (s->completion && !s->complete)
    s->completion (s);
  s->complete = 1;
  foreach (l, s->deps) {
    Stub *d = l->data;
    int complete = 0;
    if (d->base.info) { //之前在新建这个Resource的时候，已经找过了。
      d->reqs = insert_stub (d->reqs, s, d->base.info);
      complete = list_length (d->reqs) == d->all;
    } else {
      d->reqs = insert_unique (d->reqs, s);
      d->flags &= ~s->flag;
      complete = !d->flags;
    }
    if (complete) {
      if (d->list) {
        remove_reqs (d, list_subtract (d->list, d->reqs));
        d->list = NULL;
      }
      dep_complete (d);
    }
  }
}

void dep_reset (Stub *s) {
  List *l;
  s->complete = 0;
  foreach (l, s->deps) {  //取出 deps 子层级中的Stub
    Stub *d = l->data;    //
    if (d->base.info) //如果数据存在
      d->reqs = list_delete (d->reqs, s); //将d->reqs中的包含s数据的项目删除并释放内存占用
    else d->flags |= s->flag; //??
    dep_reset (d);    //迭代
  }
}

/*通过网络访问来更新某一个资源*/
void update_resource (Stub *s) {
  if (s->status >= 0) { //is the HTTP status, 0 for a new Stub, -1 for an update
    if (s->all) s->offset = 0;
    s->list = s->reqs;
    s->reqs = NULL;
    if (s->status && !se_event (resource_type (s)))
      dep_reset (s);
    else s->complete = 0;
    s->status = -1;
    get_seq (s, 0, s->all); //发生网络访问
  }
}


/* 获取到某一个type类型的 Stub */
void *get_subordinate (Stub *s, int type) {
  List *l;
  foreach (l, s->reqs) {
    Stub *t = l->data;
    if (resource_type (t) == type)
      return t->moved ? t->moved : t;
  }
  return NULL;
}


//从服务器获取一个资源。先连接到服务器，然后再获取stub资源。
Stub *get_resource (void *conn, int type, const char *href, int count) {
  Stub *s;
  Uri128 buf;
  Uri *uri = &buf.uri;
  if (!http_parse_uri (&buf, conn, href, 127)) return NULL;
  if (uri->host) conn = se_connect_uri (uri); //首先是连接到服务器
  s = get_stub (uri->path, type, conn);
  if ((time (NULL) - s->base.time) > s->poll_rate) {  /*如果当前时间已经超过了poll_rate，即表示这个资源需要更新了*/
    s->all = count;
    update_resource (s);  //在这个函数中去获取到资源（将访问网络），获取到之后，填充到哈希表中去
  }
  return s;
}

/*
poll资源，主要是针对 SE_Event_t 数据 。但是看起来他并没有去做向服务器Poll的动作？？
*/

void poll_resource (Stub *s) {
  time_t now = se_time ();
  time_t next = now + s->poll_rate;
  if (se_event (resource_type (s))) { //如果是event类型的数据，则需要定时的查询一下。
    SE_Event_t *ev = resource_data (s); 
    time_t end = ev->interval.start + ev->interval.duration;
    if (s->poll_rate >= (end - now)) return;
  }
  
  //如果下一次轮询的时刻还没有到，则放到event poll queue中
  if (s->poll_next <= now) {
    s->poll_next = next;
    //这里的类型RESOURCE_POLL，跟在der_poll中调用这个函数的时候的case值严格一致了。
    insert_event (s, RESOURCE_POLL, next);  //添加一个异步事件，在主程序中被响应并且执行后续的数据获取动作。
  }
}

/*获取dcap资源*/
Stub *get_dcap (Service *s, int secure) {
  void *conn = service_connect (s, secure);
  return get_resource (conn, SE_DeviceCapability, service_dcap (s), 0);
}

/*获取到路径*/
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

void update_existing (Stub *s, void *obj, DepFunc dep) {
  Resource *r = &s->base;
  List *l;
  if (!r->data) r->data = obj;
  else if (se_event (r->type)) {
    SE_Event_t *ex = r->data, *ev = obj;
    memcpy (&ex->EventStatus, &ev->EventStatus,
            sizeof (SE_EventStatus_t));
    free_se_object (obj, r->type);
  } else replace_se_object (r->data, obj, r->type);
  dep (s);
  if (!s->flags) dep_complete (s);
}

// update paging and return number of items needed
int list_seq (Stub *s, void *obj) {
  int results;
  if (se_type_is_a (s->base.type, SE_SubscribableList)) {
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
  return s->all - s->offset;
}

char *object_path (Uri128 *buf, void *conn, void *data) {
  SE_Resource_t *sr = data;
  if (sr->href && http_parse_uri (buf, conn, sr->href, 127))
    return buf->uri.path;
  return NULL;
}

// process list object with dependency function
int list_object (Stub *s, void *obj, DepFunc dep) {
  Resource *r = &s->base;
  int count = list_seq (s, obj);
  List **list = se_list_field (obj, r->info), *input, *l;
  input = *list;
  *list = NULL;
  if (!r->data) r->data = obj;
  else replace_se_object (r->data, obj, r->type);
  dep (s);
  foreach (l, input) {
    Uri128 buf;
    char *path;
    if (path = object_path (&buf, s->conn, l->data)) {
      Stub *d = get_stub (path, r->info->type, s->conn);
      add_dep (s, d);
      update_existing (d, l->data, dep);
    } else {
      // subordinate resource with no href or invalid href
      free_se_object (l->data, r->info->type);
    }
  }
  free_list (input);
  if (count > 0) get_seq (s, s->offset, count);
  else if (!s->all) dep_complete (s);
  return count;
}

Stub *find_target (void *conn) {
  Stub *head;
  return find_stub (&head, http_path (conn), conn);
}


Stub *match_request (void *conn, void *data, int type) {
  Stub *s = http_context (conn);  //找出前面发送的那个Stub。
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

void process_response (void *conn, int status, DepFunc dep) {
  Stub *s;
  void *obj;
  int type, count = 0;
  switch (http_method (conn)) {
  case HTTP_GET:
    if (obj = se_body (conn, &type)) {
      print_se_object (obj, type);
      printf ("\n");
      if (s = match_request (conn, obj, type)) {
        s->base.time = time (NULL);
        if (s->base.info)
          count = list_object (s, obj, dep);
        else update_existing (s, obj, dep);
        if (!count) s->status = status;
      } else free_se_object (obj, type);
    }
    break;
  case HTTP_POST:
    if (s = find_target (conn)) {
      if (s->base.info) {
        char *location = http_location (conn);
        int type = s->base.info->type;
        add_dep (s, get_resource (conn, type, location, 0));
      }
    }
    break;
  case HTTP_DELETE:
    remove_stub (http_context (conn));
    break;
  }
}

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

//处理HTTP回复的数据（看起来像是面向服务器端写的程序）
int process_http (void *conn, DepFunc dep) {
  int status;
  Stub *s;
  switch (se_receive (conn)) {
  case HTTP_RESPONSE:
    switch (status = http_status (conn)) {
    case 200:
    case 201:
    case 204:
      process_response (conn, status, dep); //如果成功回复，则将执行dep函数
      return status;
    case 300:
    case 301:
      process_redirect (conn, status);
      break;
    default:  // 如果是访问资源但是回复的数据不是上述表示成功的情况，则表示该资源（看起来）已经被删除了，所以要再本地也同样的删除掉。
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
