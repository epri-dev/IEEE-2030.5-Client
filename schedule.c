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

#define SCHEDULE_UPDATE (EVENT_NEW+10)  //首次触发是由schedule_der触发，后续将调用 update_schedule 函数，然后update_schedule将在有control需要执行的情况下连续的调用自己。

//一个事件调度的各个状态
enum EventStatus {Scheduled, Active, Canceled, CanceledRandom,
                  Superseded, Aborted, Completed, ActiveWait,
                  ScheduleSuperseded
                 };

char* ResponseTypeString[] = {"EventReceived", "EventStarted", "EventCompleted",
                    "EventOptOut", "EventOptIn", "EventCanceled", "EventSuperseded",
                    "EventPartialOptOut", "EventPartialOptIn", "EventCompleteOptOut",
                    "EventAcknowledge", "EventNoDisplay", "EventAbortedServer",
                    "EventAbortedProgram",
                    "EventInapplicable"/*252*/, "EventInvalid", "EventExpired"
                   };

char *EventStatusString[] = {
  "Scheduled", "Active", "Canceled", "CanceledRandom",
  "Superseded", "Aborted", "Completed", "ActiveWait",
  "ScheduleSuperseded"};
  
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
  int64_t next; //下一个事件发生的时刻。
  Stub *device; ///< EndDevice that is the subject of scheduling 用来调度的对象（主题）
  void *context; //< is a pointer to device specific information 设备特定的上下文环境信息。指的是 DerDevice 对象。
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

char * ResponseType_to_string( int ResponseTypeCode )
{
  if( ResponseTypeCode >= EventInapplicable){
    ResponseTypeCode = ResponseTypeCode - EventInapplicable + 14;
  }else if(ResponseTypeCode >= 1){
    ResponseTypeCode -= 1;
  }else{
    return "Invalid code";
  }
  return ResponseTypeCode < (sizeof(ResponseTypeString)/sizeof(char*))? ResponseTypeString[ResponseTypeCode] : "NotDefined";
}

char * EventStatus_to_string(int status)
{
  return status < sizeof(EventStatusString)/sizeof(char*) ? EventStatusString[status] : "NotDefined";
}

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
  LOG_I("device_response,device:%s,event:%s,status:%d(%s)\n",device->base.name,event->base.name,status,ResponseType_to_string(status));
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
  if ((a->primacy == b->primacy && x->creationTime > y->creationTime) || a->primacy < b->primacy)//如果两个事件的primacy的值相同，而前者创建比较早
    return a->der ? (b->der &= ~a->der) == 0 : 1; //如果b中的ber标志位完全被清空了（被a中的相同bit清空了），那么就返回ture，表示b被a取代了。否则就是没有被取代。
  a->der &= ~b->der;  //将a->der中的某些bit位清零。这些bit位是对应在b->der中的bit位。
  return 0; //没有符合上述条件的:1)a的优先级没有b那么高。2）优先级相同，但是b的创建时间更早。那么，就返回0，表示b没有被a圈圈替代。
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
  while (e) { //应该是在排序后插入，根据开始时间等参数。这里处理插入后，对别的已经存在的control的影响。
    next = e->next;
    event_interval (&y, e->event);  //计算结束时间
    if (x.start < y.end && x.end > y.start) { //x和y有相互叠加部分，不一定是相互包含。
      if (block_supersede (eb, e)) {  //
        e->status = ScheduleSuperseded;
        link_insert (s->superseded, e); // scheduled 队列中的第一个被 superseded 替代了？？
        prev->next = e = next;  //直接跳过了这一个
        continue;
      } else if (!eb->der) {  //如果当前的这个eb->der为空，那么也设置成 superseded 状态
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

int active_poll_rate = 300; //这个意思是：活跃的DER Control Event的轮询周期。CORE中的测试要求在60秒之内。这个指令可以在测试指令中通过 poll 选项来修改。

//将一个event激活，执行，并且回复给上面的服务器当前的 EventStarted 状态
void activate_block (Schedule *s, EventBlock *eb) {
  Stub *event = eb->event;
  LOG_I ("activate_block , href : %s , status : %d\n", event->base.name, eb->status);
  if (eb->status != Active) {
    eb->status = Active;
    insert_event (eb, EVENT_START, 0);  //将在主程序中对该类型的系统event做出响应（搜索EVENT_START），即执行对下层的逆变器设备的控制。
    device_response (s->device, event, EventStarted); //  向服务器回复“Event开始了”这个状态。
  }
  event->poll_rate = active_poll_rate;  //以测试指令中通过 poll <interval> 指定的周期来轮询，
  poll_resource (event);  //维持轮询
}

/*
将一个EventBlock插入到一个active 这个 Schedule 队列中。
*/
void insert_active (Schedule *s, EventBlock *eb) {
  EventBlock *prev = (EventBlock *)&s->active, *a = s->active, *next;
  LOG_I ("insert_active\n");
  if (resource_type (eb->event) == SE_DERControl)
    eb->der = der_base (eb->event);
  while (a) {
    next = a->next;
    if (block_supersede (eb, a)) {  //如果a被eb完全替代了。
      prev->next = next;  //则移除a
      a->end = eb->start; //
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
  LOG_I ("activate_blocks,href:%s\n",event->base.name);
  foreach (l, event->schedules) {
    EventBlock *eb = get_block (l->data, event);
    if (eb->status == ActiveWait)
      activate_block (l->data, eb);
  }
}

/*

*/
void remove_block (Schedule *s, EventBlock *eb) {
  LOG_I("remove_block : event name : %s\n",((Stub*)eb->event)->base.name);
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
    insert_event (eb, EVENT_END, 0);  //将在主程序中打印这个event的结束信息，没有做其他事情。
  }
  eb->status = status;
}


/*
大意是，将一个Event对象加入到调度器中
*/
EventBlock *schedule_event (Schedule *s, Stub *event, int primacy) {
  EventBlock *eb; //这个是本代码内部定义的一个Event对象，专门用来做调度的
  SE_Event_t *ev = resource_data (event);
  int status = event_status (event);
  LOG_I("schedule_event : %s,primacy:%d,event status:%d(%s)\n",event->base.name,primacy,status,EventStatus_to_string(status) );
  if (eb = hash_get (s->blocks, ev->mRID)) {  //如果已经存在于hash表中，那么就不用重复添加了。
    eb->primacy = primacy;
  } else {  //如果不存在，则添加一个新的
    event->schedules = insert_unique (event->schedules, s); //在这个schedules List中插入这个 Schedule。一个设备可能包含了多个 schedule
    LOG_W("  schedule_event : set event's completion() to event_update\n");
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
  if (in_range (eb->status, Canceled, Completed)) return eb;  //如果已经取消、退出、替代或者执行成功了，那么就不用再放到调度器中去了，直接返回。
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

/*通过一个EventBlock数据来反推出mRID数据*/
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


/* 更新三个queue中的各个event的状态 */
void update_schedule (Schedule *s) {
  LOG_I("update_schedule , sys time :%ld\n",(int64_t)time(NULL));
  EventBlock *eb = s->active, *next;
  int64_t now = se_time (), last = 0;
  
  while (eb) {  //检查active队列中是否有已经结束了的。遇到结束的就报告结束，直到遇到一个还没有到结束时间的。
    LOG_D("  update_schedule : active queue : %s,start:%ld,end:%ld\n",((Stub*)eb->event)->base.name,eb->start,eb->end);
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
  s->active = eb; //定位到还没结束的那个EventBlock
  
  eb = s->scheduled;  //检查scheduled中的EventBlock是否应当开始执行，如果应该开始执行了，则放到active队列中去。
  while (eb) {
    LOG_D("  update_schedule : scheduled queue : %s,start:%ld,end:%ld\n",((Stub*)eb->event)->base.name,eb->start,eb->end);
    next = eb->next;
    if (eb->start <= now) {
      LOG_I("  update_schedule : move from scheduled to active queue : %s",((Stub*)eb->event)->base.name);
      insert_active (s, eb);  //从scheduled队列移动到active队列中去
      last = last ? min (last, eb->end) : eb->end;
    } else {
      last = last ? min (last, eb->start) : eb->start;
      break;
    }
    eb = next;
  }
  s->scheduled = eb;
  
  eb = s->superseded; //检查scheduled表中的EventBlock被替代。如果已经被替代了，则修改状态，发送Response给服务器。
  while (eb) {
    LOG_D("  update_schedule : superseded queue : %s,start:%ld,end:%ld\n",((Stub*)eb->event)->base.name,eb->start,eb->end);
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
  
  LOG_I ("  update_schedule %" PRId64 "(now) %" PRId64 "(last)\n", now, last);
  if (last) { //last有可能是0。
    if (last != s->next) {  //决定下一次调用 update_schedule 的时间。
      remove_event (s);
      insert_event (s, SCHEDULE_UPDATE, last);  //看起来像是在下一个时间点上触发该事件，将重新调用本函数。
    }
    s->next = last;
  } else s->next = 0;
}

