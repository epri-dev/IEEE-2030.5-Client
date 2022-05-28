// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup event_queue Event Queue

    Provides a timed event queue. 提供一个基于时间排序的状态序列
    @{
*/

#define EVENT_TIMER (EVENT_NEW+2) /*application defined events */
#define EVENT_START (EVENT_NEW+3) /*DER Controls becoming active and inactive (`EVENT_START`, `EVENT_END`)*/
#define EVENT_END (EVENT_NEW+4)
// #define EVENT_UPDATE (EVENT_NEW+5)

/** @brief Insert an event into the queue.  //将一个Event放到队列中
    @param data is a pointer to the event data  //该Event数据
    @param type is the type of the event  Event类型，由本程序自己定义（EVENT_NEW值之后的值）
    @param time is the time at which the event becomes active 该事件（应当，预计）发生的时间
*/
void insert_event (void *data, int type, int64_t time);

/** @brief Remove all events from the queue that match the event data.将所有同一类型的Event移除。
    @brief data is a pointer to the event data
*/
void remove_event (void *data);

// 从Event队列中移出下一个已经激活的Event。
/** @brief Returns (and removes) the next active event from the queue if any.
    @param any is a pointer to value that receives the event data
    @returns the type of event or EVENT_NONE if there are no active events in
    the queue.
*/
int next_event (void **any);

/** @brief Create an event queue timer to wake up @ref event_poll. */
void event_init ();

/** @} */

typedef struct _Event {
  struct _Event *next;
  void *data; //任意类型数据，由本程序自己定义。
  int type; //类型
  int64_t time; //（发生的？）时刻？
} Event;

Event *cur_event = NULL;  // 表示当前已近记录了的event，但是还没到执行时间。“往后”要执行的任务，都放在这个队列中。
Queue im_event = {0};     //（立即）要执行的Event的Queue。

char *EventTypeString[] = {
  "EVENT_NONE",
  "SYSTEM_EVENT",
  "UDP_PORT", /*16*/
  "TCP_PORT",
  "TCP_CONNECT",
  "TCP_ACCEPT",
  "TCP_CLOSED",
  "TCP_TIMEOUT",
  "TIMER_EVENT",
  "POLL_TIMEOUT",
  "EVENT_NEW",  /*32*/
  "SERVICE_FOUND", /*EVENT_NEW+1*/
  "EVENT_TIMER",  /*+2*/
  "EVENT_START",  /*+3*/
  "EVENT_END",    /*+4*/
  "EVENT_UPDATE", /*+5*/
  "RESOURCE_POLL",/*+6*/
  "RESOURCE_UPDATE",  /*+7*/
  "RESOURCE_REMOVE",  /*+8*/
  "RETRIEVE_FAIL",/*+9*/
  "SCHEDULE_UPDATE",/*+10*/
  "DEVICE_SCHEDULE",  /*+11*/
  "DEVICE_METERING",  /*+12*/
  "DEFAULT_CONTROL" /*+13*/
  };

char * EventTypeCode_to_string(int ec)
{
  if(ec < 0 || ec > (EVENT_NEW+13)) return "invalid code";
  if(ec < 16){
    return EventTypeString[ec];
  }
  if(ec < 32){
    return EventTypeString[ec-16+2];
  }else{
    return EventTypeString[ec-32+10];
  }
}


//比较两个event的时间差。
int event_compare (void *a, void *b) {
  Event *x = a, *y = b;
  return x->time - y->time;
}

void print_events () {
  Event *e = cur_event;
  printf ("print_events %d: ", list_length (e));
  while (e) {
    printf ("%p %d %ld, ", e->data, e->type, e->time);
    e = e->next;
  }
  printf ("\n");
}

/* 这里的type值，取的是本代码自定义的类型，是大于 EVENT_NEW 的值，比如 SCHEDULE_UPDATE 
这个的参数“void *data”的数据类型是不确定的（也正是这里设计成void *类型的原因），经过查看全局代码，发现可以是der类型，也可以是Stub等。
最终通过type来确定前面的data是什么类型*/
void insert_event (void *data, int type, int64_t time) {
  LOG_I("insert_event,type:%d(%s),time:%ld\n",type,EventTypeCode_to_string(type),time);
  Event *e = type_alloc (Event);
  e->data = data;
  e->type = type;
  e->time = time; //表示即将发生的时间。
  if (time == 0) queue_add (&im_event, e);  //如果time值是0，则表示需要立即执行的，直接添加到队列末尾。在这个工程中，很多都是用到了time 0的情况。
  else cur_event = insert_sorted (cur_event, e, event_compare); //根据时间先后关系，将e插入到 cur_event queue list中并更新 cur_event 。
}

//根据data指针，将一个event数据从list中移出，并释放该event所占用的空间。
void *remove_by_data (void *list, void *data) {
  List **first = list, *l = *first, *prev = NULL, *next;
  while (l) {
    next = l->next;
    if (l->data == data) {
      if (prev) prev->next = next;
      else *first = next;
      free (l);
    } else prev = l;
    l = next;
  }
  return prev;
}


void remove_event (void *data) {
  im_event.last = remove_by_data (&im_event.first, data);
  remove_by_data (&cur_event, data);
}

//
Timer *ev_timer;  //这个Timer是这个系统中除了前面_tcp_timer之外第二个Timer。用于处理 event 执行调度。


//先查询    im_event 看看是否需要立即执行的任务，后查询cur_event，看看下一个开始时间是什么时候
int next_event (void **any) {
  Event *e;
  int event;
  
  /*
  如果im_event存在哪怕一个元素，则移出，返回该事件。每次进入到这个函数的时候，首先执行这个 Queue 中的event。
  等全部执行完毕了，则执行下面的cur_event。
  */
  if (queue_peek (&im_event)) { //peek一下im_event中的首个元素，看是否存在首个元素
    e = queue_remove (&im_event); 
    event = e->type;
    *any = e->data; //取出实际有用的数据，
    free (e); //而用于队列载具的event则此时无用，可以删除。
    return event;
  }

  //如果im_event不存在任何事件，判断 cur_event 是否为非空。
  
  if (e = cur_event) {  //注意这里是“=”号，表示将cur_event值赋给e然后判断是否为空。
    int64_t now = se_time ();
    //printf ("next_event %d %d\n", e->time, now);
    if (e->time <= now) {
      event = e->type;
      cur_event = e->next;  //有可能存在下一个，也有可能下一个没有。
      *any = e->data; //返回数据
      free (e); 
      return event;
    }
    set_timer (ev_timer, e->time - now);  //如果当前的这个event定时时间还没有到，则设定一个定时器，在该event定时时刻唤醒
  } else set_timer (ev_timer, 0);
  return EVENT_NONE;
}

//创建 event timer
void event_init () {
  ev_timer = add_timer (EVENT_TIMER);
}
