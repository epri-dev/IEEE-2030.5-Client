// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup retrieve Retrieve

    The retrieve module provides high level methods to retrieve resources from
    IEEE 2030.5 servers. These methods use a placeholder called a Stub to track
    the state of retrieval and provide local storage for a resource. The Stubs
    can be linked together to create and manage hierarchies of both dependent
    and required resources.

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

/** A Resource Stub. */
typedef struct _Stub {
  Resource base; ///< is a container for the resource
  void *conn; ///< is a pointer to an SeConnection
  int status; ///< is the HTTP status, 0 for a new Stub, -1 for an update
  time_t poll_next; ///< is the next time to poll the resource
  int16_t poll_rate; ///< is the poll rate for the resource
  unsigned complete : 1; ///< marks the Stub as complete
  unsigned subscribed : 1;
  uint32_t flag; ///< is the marker for this resource in its dependents
  uint32_t flags; ///< is a bitwise requirements checklist
  uint32_t offset; ///< is the offset used for list paging
  uint32_t all; ///< is the total number of list items
  struct _Stub *moved; ///< is a pointer to the new resource
  List *list; ///< is a list of old requirements for updates
  List *deps; ///< is a list of dependencies
  List *reqs; ///< is a list of requirements
  union {
    void *context; ///< is a user defined completion context
    List *schedules; //< is a list of schedules for event resources 
  };
  void (*completion) (struct _Stub *); ///< is a user defined completion routine
} Stub;

/** @brief Get an IEEE 2030.5 resource.
    @param conn is a pointer to an SeConnection
    @param type is an IEEE 2030.5 schema type
    @param href is URI string, the location of the resource
    @param count is the number of items to get for %List resources, should be 0
    for non-List resources
    @returns a pointer to the Stub for the resource
*/
Stub *get_resource (void *conn, int type, const char *href, int count);

/** @brief Get a subordinate resource given a parent object.

    Subordinate resources are retrieved from links in the parent object.
    Links follow a naming convention, they are named after the type of the
    subordinate resource with the string "Link" appended. This macro
    provides a convienent way to retrieve a subordinate resource, taking
    advantage of the naming pattern used to link subordinate resources from
    their parent object.
    @param conn is a pointer to an SeConnection
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the type name of the subordinate resource
    @returns a pointer to the Stub for the resource if the link exists,
    NULL otherwise
*/
#define get_root(conn, obj, type)				\
  (se_exists (obj, type##Link)?					\
   get_resource (conn, SE_##type, (obj)->type##Link.href, 0) : NULL)

/** @brief Get a subordinate %List resource given a parent object.

    The same as @ref get_root but for subordinate %List resources. Links to
    %List resources include the "all" parameter, this indicates the number of
    items contained in the %List resource and is used as a query parameter in
    the HTTP GET request.
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

/** Get a subordinate resource Stub and make the parent resource a dependent.

    Creates a two way link between the subordinate and parent resources, the
    parent stores a bit representing the requirement for the subordinate
    resource and the subordinate resource clears this bit once all of its
    requirements are met (full retrieval of itself and all of its
    subordinates). 
    @param r is a pointer to a Stub, the parent resource
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the type name of the subordinate resource    
    @returns a pointer to the subordinate resource Stub
*/
#define get_dep(r, obj, type)					\
  new_dep (r, get_root ((r)->conn, obj, type), SE_##type##Link_exists)

/** @brief Get a subordinate %List resource Stub and make the parent resource a
    dependent.

    Performs the same function as @ref get_dep except for %List resources.
    @param r is a pointer to a Stub, the parent resource
    @param obj is a pointer to an IEEE 2030.5 object
    @param type is the type name of the subordinate resource
    @returns a pointer to the subordinate resource Stub
*/
#define get_list_dep(r, obj, type)				\
  new_dep (r, get_list_root ((r)->conn, obj, type), SE_##type##Link_exists)

/** @brief Dependency function */
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

void get_seq (Stub *s, int offset, int count) {
  char *name = resource_name (s);
  if (count) { char uri[64];
    if (count > 255) count = 255;
    if (offset) sprintf (uri, "%s?s=%d&l=%d", name, offset, count);
    else sprintf (uri, "%s?l=%d", name, count);
    http_get (s->conn, uri);
  } else http_get (s->conn, name);
  set_request_context (s->conn, s);
}

void add_dep (Stub *r, Stub *d) {
  d->deps = insert_unique (d->deps, r);
  d->poll_rate = min (d->poll_rate, r->poll_rate);
  r->complete = 0;
}

Stub *new_dep (Stub *r, Stub *d, int flag) {
  if (d) {
    add_dep (r, d); d->flag = flag; r->flags |= flag;
  } return d;
}

void remove_req (Stub *s, Stub *r) {
  r->deps = list_delete (r->deps, s);
  if (!r->deps) insert_event (r, RESOURCE_REMOVE, 0);
}

void remove_reqs (Stub *s, List *reqs) { List *l;
  foreach (l, reqs) remove_req (s, l->data);
  free_list (reqs);
}

void remove_deps (Stub *s, List *deps) { List *l;
  foreach (l, deps) { Stub *t = l->data;
    if (t->status < 0)
      t->list = list_delete (t->list, s);
    else t->reqs = list_delete (t->reqs, s);
  } free_list (deps);
}

void *find_stub (Stub **head, char *name, void *conn) {
  if (*head = find_resource (name)) { Stub *s;
    foreach (s, *head) if (s->conn == conn) return s;
  } return NULL;
}

void *get_stub (char *name, int type, void *conn) {
  Stub *head, *s = find_stub (&head, name, conn);
  if (!s) {
    s = new_resource (sizeof (Stub), name, NULL, type);
    s->conn = conn; s->poll_rate = 900; 
    if (head) link_insert (list_next (head), s);
    else insert_resource (s);
  } return s;
}

void delete_reqs (Stub *s) {
  remove_reqs (s, s->reqs); remove_reqs (s, s->list);
  s->reqs = s->list = NULL;
}

void remove_stub (Stub *s) {
  Stub *head = find_resource (s->base.name),
    *t = list_remove (head, s); 
  if (t) { if (t != head) insert_resource (t); }
  else delete_resource (head);
  if (s->moved) remove_req (s, s->moved);
  else delete_reqs (s);
  remove_deps (s, s->deps); remove_event (s); free_resource (s);
}

void *insert_stub (List *list, Stub *s, ListInfo *info) {
  List *prev = NULL, *l = list, *n;
  void *data = resource_data (s);
  if (find_by_data (list, s)) return list;
  while (l) { Resource *r = l->data;
    if (compare_keys (data, r->data, info) < 0) break;
    prev = l, l = l->next;
  }
  n = list_insert (l, s);
  if (prev) prev->next = n; else list = n;
  return list;
}

void delete_stub (Stub *s) {
  http_delete (s->conn, resource_name (s));
  set_request_context (s->conn, s);
}

void dep_complete (Stub *s) { List *l;
  if (s->completion && !s->complete)
    s->completion (s);
  s->complete = 1;
  foreach (l, s->deps) {
    Stub *d = l->data; int complete = 0;
    if (d->base.info) {
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

void dep_reset (Stub *s) { List *l;
  s->complete = 0;
  foreach (l, s->deps) {
    Stub *d = l->data;
    if (d->base.info)
      d->reqs = list_delete (d->reqs, s);
    else d->flags |= s->flag;
    dep_reset (d);
  }
}

void update_resource (Stub *s) {
  if (s->status >= 0) {
    if (s->all) s->offset = 0;
    s->list = s->reqs; s->reqs = NULL;
    if (s->status && !se_event (resource_type (s)))
      dep_reset (s);
    else s->complete = 0;
    s->status = -1; get_seq (s, 0, s->all);
  }
}

void *get_subordinate (Stub *s, int type) { List *l;
  foreach (l, s->reqs) {
    Stub *t = l->data;
    if (resource_type (t) == type)
      return t->moved? t->moved : t;
  } return NULL;
}

Stub *get_resource (void *conn, int type, const char *href, int count) {
  Stub *s; Uri128 buf; Uri *uri = &buf.uri;
  if (!http_parse_uri (&buf, conn, href, 127)) return NULL;
  if (uri->host) conn = se_connect_uri (uri);
  s = get_stub (uri->path, type, conn);
  if ((time (NULL) - s->base.time) > s->poll_rate) {
    s->all = count; update_resource (s);
  } return s;
}

void poll_resource (Stub *s) {
  time_t now = se_time ();
  time_t next = now + s->poll_rate;
  if (se_event (resource_type (s))) {
    SE_Event_t *ev = resource_data (s);
    time_t end = ev->interval.start + ev->interval.duration;
    if (s->poll_rate >= (end - now)) return;
  }
  if (s->poll_next <= now) {
    s->poll_next = next; insert_event (s, RESOURCE_POLL, next);
  }
}

Stub *get_dcap (Service *s, int secure) {
  void *conn = service_connect (s, secure);
  return get_resource (conn, SE_DeviceCapability, service_dcap (s), 0);
}

Stub *get_path (Service *s, int secure) {
  void *conn; int type, count;
  char *path = service_path (s);
  if (!path) return get_dcap (s, secure);
  conn = service_connect (s, secure);
  type = service_type (s); count = se_list (type)? 255 : 0;
  return get_resource (conn, type, path, count);
}

void update_existing (Stub *s, void *obj, DepFunc dep) {
  Resource *r = &s->base; List *l;
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
int list_seq (Stub *s, void *obj) { int results;
  if (se_type_is_a (s->base.type, SE_SubscribableList)) {
    SE_SubscribableList_t *sl = obj;
    s->all = sl->all; results = sl->results;
  } else { SE_List_t *sl = obj;
    s->all = sl->all; results = sl->results;
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
  Resource *r = &s->base; int count = list_seq (s, obj);
  List **list = se_list_field (obj, r->info), *input, *l;
  input = *list; *list = NULL;
  if (!r->data) r->data = obj;
  else replace_se_object (r->data, obj, r->type);
  dep (s);
  foreach (l, input) { Uri128 buf; char *path;
    if (path = object_path (&buf, s->conn, l->data)) {
      Stub *d = get_stub (path, r->info->type, s->conn);
      add_dep (s, d); update_existing (d, l->data, dep);
    } else {
      // subordinate resource with no href or invalid href
      free_se_object (l->data, r->info->type);
    }
  } free_list (input);
  if (count > 0) get_seq (s, s->offset, count);
  else if (!s->all) dep_complete (s);
  return count;
}

Stub *find_target (void *conn) { Stub *head;
  return find_stub (&head, http_path (conn), conn);
}

Stub *match_request (void *conn, void *data, int type) {
  Stub *s = http_context (conn); Uri128 buf;
  char *path = object_path (&buf, conn, data);
  if (path && streq (resource_name (s), path)) {
    int rtype = resource_type (s);
    if (rtype < 0) resource_type (s) = type;
    else if (rtype != type) return NULL;
    return s;
  } return NULL;
}

void process_response (void *conn, int status, DepFunc dep) {
  Stub *s; void *obj; int type, count = 0;
  switch (http_method (conn)) {
  case HTTP_GET:
    if (obj = se_body (conn, &type)) {
      print_se_object (obj, type); printf ("\n");
      if (s = match_request (conn, obj, type)) {
	s->base.time = time (NULL);
	if (s->base.info)
	  count = list_object (s, obj, dep);
	else update_existing (s, obj, dep);
	if (!count) s->status = status;
      } else free_se_object (obj, type);
    } break;
  case HTTP_POST:
    if (s = find_target (conn)) {
      if (s->base.info) {
	char *location = http_location (conn);
	int type = s->base.info->type;
	add_dep (s, get_resource (conn, type, location, 0));
      }
    } break;
  case HTTP_DELETE: remove_stub (http_context (conn)); break;
  }
}

void get_moved (Stub *s, char *location) {
  if (location) {
    Stub *t = get_resource (s->conn, resource_type (s), location, s->all);
    if (s->moved && t && s->moved != t) remove_req (s, s->moved);
    if (t) { add_dep (s, t); s->moved = t; }
  } delete_reqs (s);
}

void process_redirect (void *conn, int status) { Stub *s;
  if (s = find_target (conn)) {
    get_moved (s, http_location (conn));
    s->status = status;
  } free_se_body (conn);
}

int process_http (void *conn, DepFunc dep) {
  int status; Stub *s;
  switch (se_receive (conn)) {
  case HTTP_RESPONSE:
    switch (status = http_status (conn)) {
    case 200: case 201: case 204:
      process_response (conn, status, dep); return status;
    case 300: case 301:
      process_redirect (conn, status); break;
    default:
      if (http_method (conn) == HTTP_GET
	  && (s = find_target (conn))) {
	s->status = status;
	insert_event (s, RETRIEVE_FAIL, 0);
      } free_se_body (conn);
    } break;
  } return 0;
}

void cleanup_http (void *conn) {
  HttpRequest *r = http_queued (conn), *next;
  while (r) { next = r->next;
    if (r->method == HTTP_GET)
      remove_stub (r->context);
    free (r); r = next;
  }
}
