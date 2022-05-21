// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup schedule Schedule

    这个schedule（调度）模块提供了用于2030.5 event 调度 的一些方法。
    
    The schedule module provides methods for IEEE 2030.5 event scheduling.
    Scheduling is done per function set such as Pricing, Distributed Energy
    Resources (DER), or Demand Response / Load Control (DRLC).
    这个调度模执行各个功能系列比如价格，分布式能源（Distributed Energy    
    Resources）...

    各个不同功能系列的事件都是独立的，并且各个EndDevice允许使用其中一个或者多个功能系列，因此一个EndDevice能够拥有一个或者多个调度器。
    The events of each function set are independent and each EndDevice may
    utilitize one or more function sets, therefore an EndDevice could have one
    or more independent event schedules.
    @{
*/

#define SCHEDULE_UPDATE (EVENT_NEW+10)

//一个事件调度的各个状态
enum EventStatus {Scheduled, Active, Canceled, CanceledRandom,
                  Superseded, Aborted, Completed, ActiveWait,
                  ScheduleSuperseded
                 };

/** @brief An EventBlock is an Event instance scheduled for a particular
    EndDevice.
    一个“事件块EventBlock”是面向一个特定的EndDevice的一个被调度的事件实例（Event instance）。
    
    The start and end times are effective times with the event
    randomization already applied, primacy is derived(adj. 导出的；衍生的，派生的) the context that the event
    occured.
    开始时间和结束时间都是有效时间，这些都是已近做了随机化处理的，以及已经被根据primacy排序了的。
    
*/
typedef struct _EventBlock {
  struct _EventBlock *next; ///< is a pointer to the next EventBlock  对于一个设备 (EndDevice) 来说，可能会拥有多个EventBlock，所以这里构建了一个链表
  void *event; ///< is a pointer to the Event Stub 一个指向EventStub的指针。应该可以通过这个指针来获取到具体的Event数据。
  void *program; ///< is a pointer to the DERProgram for DERControls 对于DERControl来说，就是一个指向所属的DERProgram对象的指针
  void *context; //< is a pointer to device specific information 对于DERContrrol来说，就是所属的 EndDevice 对象数据
  uint8_t status; ///< the schedule event status 当前的任务状态，由枚举型 EventStatus 来定义。
  uint8_t primacy;  ///< associated event primacy 权重（primacy）
  uint32_t der; ///< bitmask of active DER controls 
  int64_t start; ///< the effective start time  该任务开始时间
  int64_t end; ///< the effective end time  该任务结束时间
} EventBlock;

/** @brief A Schedule organizes the events of a particular function set for an
    EndDevice in terms of (依据；按照；) the rules in section 12.1.3 Event Rules and Guidlines.

    Events are assigned to an EventBlock which can belong to one of three
    queues, `scheduled`, `active`, and `superseded`. EventBlocks that
    correspond with server events that have been marked Canceled or Superseded
    belong to none of these queues and are eventually removed from the schedule
    when the events expire.

    最初的时候，EventBlocks插入到scheduled队列中，随后可能由于服务器端的修改，会将部分移动到scheduled队列
    
    Initially all EventBlocks are inserted into the `scheduled` queue using the
    rules for overlapping and nested events. Future events that are superseded
    by scheduling are marked `ScheduleSuperseded` and are placed in the
    superseded queue. If the set of events changes for some reason, these
    events may eventually rejoin the `scheduled` queue.

    
    当 EventBlocks 变成活动状态时，他们将传输到 active 队列中
    When EventBlocks in the scheduled queue become active (i.e when the current
    time matches the effective start time) they are transfered to the `active`
    queue, in the process they may be superseded by existing active EventBlocks
    or they may supersede other active EventBlocks. In these cases the
    superseded EventBlocks are removed from the schedule and a Response is sent
    to the server with the response status EventSuperseded.

    //
    If an EventBlock is sucessfully transfered to the `active` queue and the
    EventStatus.currentStatus retrieved from the server is Active then an
    internal EVENT_START event is generated for current time. If the
    EventStatus.currentStatus is Scheduled, then the event will be marked
    `ActiveWait` and remain in the active queue, but the EVENT_START event will
    only be generated when currentStatus for the event changes to Active. When
    the EventBlock expires (i.e the current time matches the effective end
    time) or it is superseded then an internal EVENT_END event is generated
    at that time for the EventBlock.
*/
typedef struct {
  int64_t next;
  Stub *device; ///< EndDevice that is the subject of scheduling 用来调度的对象（主题）
  void *context; //< is a pointer to device specific information 设备特定的上下文环境信息
  /*哈希表，使用Event mRID 作为哈希表的 key */ 
  HashTable *blocks; /**< HashTable of EventBlocks that belong to the schedule using the Event mRID as the hash key. */
  EventBlock *scheduled; ///< EventBlock queue sorted by effective start time 以“开始时间”排序的 EventBlock 队列
  EventBlock *active; ///< EventBlock queue sorted by effective end time 以“结束时间”排序的 EventBlock 队列
  EventBlock *superseded; ///< EventBlock queue sorted by effective start time 以“实际开始时间”排序的 EventBlock 队列
} Schedule;

/** @brief Send an event response to the server on the behalf of a device.
    发送一个 event 回复到服务器上，根据当前的设备的行为？？
    @param device is a pointer to an EndDevice Stub 一个设备资源，即当前执行动作的特定的设备。
    @param event is a pointer to an Event Stub Event资源
    @param status is the status of the event response (@ref ResponseType) 回复到服务器的状态信息。
*/
void device_response (Stub *device, Stub *event, int status);

/** @brief Add an event to a schedule with an associated primacy. 添加一个跟primacy相关联的event到调度器上。
    @param s is a pointer to a Schedule
    @param event is a pointer to an Event
    @param primacy is the associated primacy for the event
    @returns a pointer to an EventBlock belonging to the schedule
*/
EventBlock *schedule_event (Schedule *s, Stub *event, int primacy);

/** @brief Update the schedules associated with an event. 更新跟这个event相关的调度器
    @param event is a pointer to an event Stub
*/
void event_update (Stub *event);

/** @brief Update the schedule based upon the current time.
    @param s is a pointer to a Schedule
*/
void update_schedule (Schedule *s);

/** @brief Initialize the schedule.
    @param s is a pointer to a Schedule
*/
void schedule_init (Schedule *s);

/** @} */

#include <string.h>
#include <inttypes.h>

//判断一个资源类型是否可以作 "随机化" 处理
#define randomizable(type)				  \
  (type == SE_DERControl || type == SE_TimeTariffInterval \
   || type == SE_EndDeviceControl)  


//bound：一个传入的值，将对整个值做一个随机化处理。
int rand_bound (int bound) {
  if (bound) {  //如果是非零(不是大于零)
    int sign = bound < 0 ? -1 : 1, upper, result;
    bound *= sign;
    upper = RAND_MAX - RAND_MAX % bound;
    do result = rand ();
    while (result > upper);
    return sign * (result % (bound + 1));
  }
  return 0;
}

//对开始时间作随机化处理
int randomize_start (void *event) {
  SE_RandomizableEvent_t *ev = resource_data (event);
  return rand_bound (ev->randomizeStart);
}

//对持续时间作随机化处理
int randomize_duration (void *event) {
  SE_RandomizableEvent_t *ev = resource_data (event);
  return rand_bound (ev->randomizeDuration);
}

//回复服务器当前的设备的Event状态。
void device_response (Stub *device, Stub *event, int status) {
  SE_EndDevice_t *edev = resource_data (device);
  SE_Event_t *ev = resource_data (event);
  SE_DERControlResponse_t resp;
  if ((status == EventReceived && ev->responseRequired & 1) //  responseRequired中的bit1如果置位，表示要求回复。
      || (ev->responseRequired & 2)) {
    se_response (&resp, ev, edev->lFDI, status);  //构建回复给服务器的数据
    se_post (event->conn, &resp, SE_DERControlResponse, ev->replyTo); //
  }
}

//返回Control对象中的flag值。
uint32_t der_base (void *event) {
  SE_DERControl_t *derc = resource_data (event);
  return se_flags (&derc->DERControlBase);
}


//不知道什么意思。supersede 是 " vt. 替代，取代 " 的意思
int block_supersede (EventBlock *a, EventBlock *b) {
  SE_Event_t *x = resource_data (a->event),
              *y = resource_data (b->event);
  if ((a->primacy == b->primacy
       && x->creationTime > y->creationTime)  //如果两个事件的primacy的值相同，而前者创建比较早
      || a->primacy < b->primacy)
    return a->der ? (b->der &= ~a->der) == 0 : 1; //那么返回
  a->der &= ~b->der;  //只有某个bit，在a中是1，在b中是0，则结果中的该bit得以保留；其他情况结果都是0。即a中有的但是b中没有的--保留；其他情况都删除。
  return 0;
}

//新建一个 event block 
EventBlock *new_block (Stub *event, int primacy) {
  EventBlock *eb = type_alloc (EventBlock);
  SE_Event_t *ev = resource_data (event);
  int rand = randomizable (resource_type (event));
  eb->next = NULL;
  eb->start = ev->interval.start
              + (rand ? randomize_start (event) : 0);
  eb->end = eb->start + ev->interval.duration
            + (rand ? randomize_duration (event) : 0);
  eb->primacy = primacy;
  eb->status = 0;
  eb->event = event;
  return eb;
}

//返回一个event当前的状态。
int event_status (void *event) {
  SE_Event_t *ev = resource_data (event);
  return ev->EventStatus.currentStatus;
}

//比较开始时间
int compare_start (void *a, void *b) {
  EventBlock *x = a, *y = b;
  return x->start - y->start;
}

//比较结束时间。
int compare_end (void *a, void *b) {
  EventBlock *x = a, *y = b;
  return x->end - y->end;
}

typedef struct {
  uint64_t start, end;
} Interval;

//计算end time时刻。
void event_interval (Interval *i, Stub *event) {
  SE_Event_t *ev = resource_data (event);
  i->start = ev->interval.start;
  i->end = i->start + ev->interval.duration;
}

//将一个eventblock插入到一个Stub的scheduled队列中。
/*
eb是将要插入的EventBlock；s是某一个Schedule对象。
*/
void insert_block (Schedule *s, EventBlock *eb) {
  EventBlock *prev = (EventBlock *)&s->scheduled,
              *e = s->scheduled, *next;/* prev看起来都是指向的是s->scheduled这个指针值的地址；e是获取到了s->scheduled的值。 */
  Interval x, y;
  eb->status = Scheduled;
  if (resource_type (eb->event) == SE_DERControl) //resource和Stub指向同一个地址。
    eb->der = der_base (eb->event); //获取到这个DER的 DERControlBase 中的_flag值，即知道存在哪几个base设置项。详见 SE_DERControlBase_t 结构体。
  event_interval (&x, eb->event); //计算eb的结束时间。应该是首个EventBlock的时间。
  while (e) { //应该是在排序后插入，根据开始时间等参数。
    next = e->next;
    event_interval (&y, e->event);  //计算结束时间
    if (x.start < y.end && x.end > y.start) { //x被y包含
      if (block_supersede (eb, e)) {  //
        e->status = ScheduleSuperseded;
        link_insert (s->superseded, e);
        prev->next = e = next;
        continue;
      } else if (!eb->der) {
        eb->status = ScheduleSuperseded;
        link_insert (s->superseded, eb);
        return;
      }
    } else if (x.end == y.start)
      e->start = eb->end;
    else if (x.end == y.end)
      e->end = eb->end;
    prev = e;
    e = next;
  }
  s->scheduled = insert_sorted (s->scheduled, eb, compare_start); //插入到队列中去。
}

int active_poll_rate = 300; //这个意思是：活跃的DER Control Event的轮询周期。可以修改。CORE中的测试要求在60秒之内。

void activate_block (Schedule *s, EventBlock *eb) {
  Stub *event = eb->event;
  printf ("activate_block href : %s , status : %d\n", event->base.name, eb->status);
  if (eb->status != Active) {
    eb->status = Active;
    insert_event (eb, EVENT_START, 0);  //将在主程序中对该类型的系统event做出响应（搜索EVENT_START）
    device_response (s->device, event, EventStarted); //  向服务器回复“Event开始了”这个状态。
  }
  event->poll_rate = active_poll_rate;  //
  poll_resource (event);
}

/*
将一个EventBlock插入到一个Schedule中。
*/
void insert_active (Schedule *s, EventBlock *eb) {
  EventBlock *prev = (EventBlock *)&s->active, *a = s->active, *next;
  printf ("insert_active\n");
  if (resource_type (eb->event) == SE_DERControl)
    eb->der = der_base (eb->event);
  while (a) {
    next = a->next;
    if (block_supersede (eb, a)) {
      prev->next = next;
      a->end = eb->start;
      if (a->status == Active)
        insert_event (a, EVENT_END, 0);
      a->status = Superseded;
      device_response (s->device, a->event, EventSuperseded);
    } else if (!eb->der) {
      eb->status = Superseded;
      device_response (s->device, eb->event, EventSuperseded);
      return;
    }
    a = next;
  }
  switch (event_status (eb->event)) {
  case Scheduled:
    update_resource (eb->event);
    eb->status = ActiveWait;
    break;
  case Active:
    activate_block (s, eb);
    break;
  }
  s->active = insert_sorted (s->active, eb, compare_end);
}


/*通过mRID来获取到某一个 EventBlock */
EventBlock *get_block (Schedule *s, void *event) {
  SE_Event_t *ev = resource_data (event);
  return hash_get (s->blocks, ev->mRID); //根据Event的mRID值来获取到这个 event 的数据。
}

/*
将一个schedule列表中的多个 EventBlock激活 
*/

void activate_blocks (Stub *event) {
  List *l;
  // printf ("activate_blocks\n");
  foreach (l, event->schedules) {
    EventBlock *eb = get_block (l->data, event);
    if (eb->status == ActiveWait)
      activate_block (l->data, eb);
  }
}

/*

*/
void remove_block (Schedule *s, EventBlock *eb) {
  switch (eb->status) {
  case Scheduled:
    s->scheduled = list_remove (s->scheduled, eb);
    break;
  case Active:
    eb->end = se_time ();
    remove_event (eb);
    insert_event (eb, EVENT_END, 0);
  case ActiveWait:
    s->active = list_remove (s->active, eb);
    break;
  case ScheduleSuperseded:
    s->superseded = list_remove (s->superseded, eb);
    break;
  }
}

//移除一个 EventBlock
void remove_blocks (Stub *event, int response) {
  List *l;
  int status = event_status (event);
  foreach (l, event->schedules) {
    Schedule *s = l->data;
    EventBlock *eb = get_block (s, event);
    remove_block (s, eb);
    if (eb->status != status)
      device_response (s->device, event, response);
    eb->status = status;
  }
}

/*从数据库中删除这个 event 数据*/
void delete_blocks (Stub *event) {
  List *l;
  SE_Event_t *ev = resource_data (event);
  foreach (l, event->schedules) {
    Schedule *s = l->data;
    free (hash_delete (s->blocks, ev->mRID));
  }
  free_list (event->schedules);
  event->schedules = NULL;
}


/*

这个程序通常用作event对象(Stub)的 completion 函数。
在获取完毕服务器上的某一个资源 Stub / Resource 的全部数据之后，执行这个动作。

*/
void event_update (Stub *event) {
  int64_t now;
  static int64_t sync_time = 0;
  LOG_I ("event_update : %s\n",event->base.name);
  switch (event_status (event)) {
  case Scheduled: //进入到调度状态
    /* The client's clock is ahead of the server's clock, attempt to
       synchronize by setting the clock (time offset) backward, also retry
       the event retrieval in another second. */
    if (sync_time != (now = se_time ())) {
      se_time_offset--;
      sync_time = --now;
    }
    LOG_I("  event_update : insert_event,RESOURCE_UPDATE,now+1\n");
    insert_event (event, RESOURCE_UPDATE, now + 1); //now+1的意思是：RESOURCE_UPDATE动作的时间在后面一秒钟执行。
    break;
  case Active:
    activate_blocks (event);  //看起来像是提前激活
    break;
  case Canceled:
  case CanceledRandom:  //取消
    remove_blocks (event, EventCanceled);
    break;
  case Superseded:  //挂起
    remove_blocks (event, EventSuperseded);
    break;
  }
}

void block_update (EventBlock *eb, int status) {
  if (eb->status == Active) {
    eb->end = se_time ();
    insert_event (eb, EVENT_END, 0);
  }
  eb->status = status;
}


/*
大意是，将一个Event对象加入到调度器中
*/
EventBlock *schedule_event (Schedule *s, Stub *event, int primacy) {
  LOG_I("schedule_event : %s\n",event->base.name);
  EventBlock *eb; //这个是本代码内部定义的一个Event对象，专门用来做调度的
  SE_Event_t *ev = resource_data (event);
  int status = event_status (event);
  if (eb = hash_get (s->blocks, ev->mRID)) {  //如果已经存在于hash表中
    eb->primacy = primacy;
  } else {  //如果不存在，则添加一个新的
    event->schedules = insert_unique (event->schedules, s); //在这个list中插入这个 Schedule
    event->completion = event_update;
    eb = new_block (event, primacy);
    hash_put (s->blocks, eb);
    /**/
    LOG_I("  schedule_event : response to server EventReceived\n");
    device_response (s->device, event, EventReceived);  //回复给服务器说已经收到了该Event。
    if (eb->end <= se_time ()) {  //如果该Event中的结束时间已经过了，那么就不用去执行了，直接告诉服务器该事件已经结束。
      // specified end time is in the past
      eb->status = Completed;
      device_response (s->device, event, EventExpired); //回复给服务器。
      return eb;
    }
  }
  if (in_range (eb->status, Canceled, Completed)) return eb;  //如果已经取消或者执行成功了，那么就不用再放到调度器中去了，直接返回。
  // eb->status: Scheduled, Active, ActiveWait, ScheduleSuperseded
  switch (status) { //否则，再执行调度。
  case Scheduled:
    if (eb->status != ActiveWait) {
      insert_block (s, eb);
      break;
    }
  case Active:
    insert_active (s, eb);
    break;
  case Canceled:
  case CanceledRandom:
    device_response (s->device, event, EventCanceled);
    block_update (eb, status);
    break;
  case Superseded:
    device_response (s->device, event, EventSuperseded);
    block_update (eb, status);
  }
  return eb;
}

void *mrid_key (void *data) {
  EventBlock *eb = data;
  Stub *s = eb->event;
  SE_Event_t *e = resource_data (s);
  return e->mRID;
}

//构建一个以mRID作为key的哈希表
void schedule_init (Schedule *s) {
  s->blocks = new_int128_hash (64, mrid_key);
}


/*  检查当前active表中的EventBlock是否已经执行完毕了。 */
void update_schedule (Schedule *s) {
  LOG_I("update_schedule\n");
  EventBlock *eb = s->active, *next;
  int64_t now = se_time (), last = 0;
  while (eb) {  //检查是否有已经结束了的。
    if (eb->end <= now) { //当前时间已经越过了end时刻，说明这个事情结束了。
      if (eb->status == Active) {
        device_response (s->device, eb->event, EventCompleted); //向服务器报告事件已经结束。
        insert_event (eb, EVENT_END, 0);  //这里添加EVENT_END的主要功能仅仅是在主程序中执行数据的打印。
      }
      eb->status = Completed;
    } else {
      last = eb->end;
      break;
    }
    eb = eb->next;
  }
  s->active = eb;
  eb = s->scheduled;  //检查scheduled中的EventBlock是否应当开始执行
  while (eb) {
    next = eb->next;
    if (eb->start <= now) {
      insert_active (s, eb);
      last = last ? min (last, eb->end) : eb->end;
    } else {
      last = last ? min (last, eb->start) : eb->start;
      break;
    }
    eb = next;
  }
  s->scheduled = eb;
  eb = s->superseded; //检查scheduled表中的EventBlock被替代。
  while (eb) {
    if (eb->start <= now) {
      eb->status = Superseded;
      device_response (s->device, eb->event, EventSuperseded);
    } else {
      last = last ? min (last, eb->start) : eb->start;
      break;
    }
    eb = eb->next;
  }
  s->superseded = eb;
  printf ("update_schedule %" PRId64 " %" PRId64 "\n", now, last);
  if (last) {
    if (last != s->next) {
      remove_event (s);
      insert_event (s, SCHEDULE_UPDATE, last);  //看起来像是在下一个时间点上触发该事件，将重新调用本函数。
    }
    s->next = last;
  } else s->next = 0;
}

