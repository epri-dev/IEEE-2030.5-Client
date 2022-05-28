// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdio.h>

#include "se_core.h"
#include "client.c"
#include "list_util.c"
#include "time.c"
#include "event.c"
#include "hash.c"
#include "resource.c"
#include "retrieve.c"
// #include "subscribe.c"
#include "schedule.c"
#include "der.c"
#include "metering.c"

char *timestamp () {
  time_t now = (time_t)se_time ();
  return ctime (&now);
}

void print_event_start (EventBlock *eb) {
  Resource *r = eb->event;
  DerDevice *device = eb->context;
  SE_Event_t *ev = r->data;
  SE_DERControl_t *derc;
  printf ("Event Start \"%s\" -- %s", ev->description, timestamp ());
  printf ("EndDevice: %ld\n", device->sfdi);
  switch (r->type) {
  case SE_DERControl:
    derc = r->data;
    print_se_object (&derc->DERControlBase, SE_DERControlBase); //应当在这里插入对实际的逆变器的操作。本Demo代码没有涉及到对底层设备的接口。
  }
  printf ("\n");
}

void print_event_end (EventBlock *eb) {
  Resource *r = eb->event;
  DerDevice *device = eb->context;
  SE_Event_t *ev = r->data;
  printf ("Event End \"%s\" -- %s", ev->description, timestamp ()); //应当在这里插入对实际的逆变器的操作代码。在这个Demo程序中，仅仅是为了展示。
  printf ("EndDevice: %ld\n\n", device->sfdi);
}

void print_default_control (DerDevice *device) {
  SE_DefaultDERControl_t *dc = device->dderc;
  LOG_I ("Default Control : \"%s\" -- %s", dc->description, timestamp ());//应当在这里插入对实际的逆变器的操作代码。本Demo代码没有涉及到对底层设备的接口。
  printf ("EndDevice: %ld\n", device->sfdi);
  print_se_object (dc, SE_DefaultDERControl);
  printf ("\n");
}

void print_blocks (EventBlock *eb) {
  while (eb) {
    SE_Event_t *ev = resource_data (eb->event);
    printf ("  %-11ld %-11ld %-31s\n", eb->start, eb->end, ev->description);
    eb = eb->next;
  }
}

void print_event_schedule (DerDevice *d) {
  Schedule *s = &d->schedule;
  EventBlock *eb = s->scheduled;
  printf ("Event Schedule for device %ld -- %s", d->sfdi, timestamp ());
  printf ("  Start       End         Description\n");
  print_blocks (s->scheduled);
  printf ("Active Blocks:\n");
  print_blocks (s->active);
  printf ("\n");
}

/*

-   provides an event polling function `der_poll` that handles events such as
    schedule updates, and resource polling updates.
1）检查DER设备是否需要作状态更新，资源数据获取等操作。2）执行 dnssd 客户端查询函数 client_poll 。
*/

int der_poll (void **any, int timeout) {
  Schedule *s;
  int event;
  while (event = next_event (any)) {  // 查询下一个event是否已经到来。这里的event指的是系统中自定义的一些事件，由即值排在EVENT_NEW之后的event。
    switch (event) {
    case SCHEDULE_UPDATE: //调度器中设定的调度时刻到了，将调用 update_schedule 函数（这个函数中又会自己设置下次启动事件），就这样持续循环。
      LOG_I("\n** der_poll : next_event=SCHEDULE_UPDATE,call update_schedule\n");
      s = *any;
      update_schedule (s);  //更新各个事件的调度状态
      if (!s->active) { //如果active队列空掉了，那么
        DerDevice *d = s->context;  //得到原schedule中设备的对象数据
        if (d->dderc) insert_event (d, DEFAULT_CONTROL, 0); //如果存在DefaultDERControl数据对象，那么就执行该Default control。这个是IEEE规范。
      }
      break;
    
    //注意下面两个case是连贯执行的。所以在处理完毕 RESOURCE_POLL 系统消息之后，就立即开始着手处理下面的UPDATE，涉及到网络通信。
    case RESOURCE_POLL: //在函数 poll_resource 中执行一次 insert_event (s, RESOURCE_POLL, next);，然后这边就检测到了。所以只需要执行过一次poll_resource之后，后面就会自动的周期轮询。
      LOG_I("\n** der_poll : next_event=RESOURCE_POLL,poll_resource : %s\n",((Stub*)(*any))->base.name );
      poll_resource (*any);
    case RESOURCE_UPDATE: 
      /*在函数 event_update 中执行了一次 insert_event (event, RESOURCE_UPDATE, now + 1) 操作，
      然后就会执行到这里。这个case专门是针对SE_Event_t的。*/
      LOG_I("\n** der_poll : next_event=RESOURCE_UPDATE,update_resource : %s\n",((Stub*)(*any))->base.name );
      update_resource (*any); //访问网络，获取数据。
      break;

    case RESOURCE_REMOVE: //  异步调用该功能：在服务器上移除一个资源。
      if (se_event (resource_type (*any)))  //如果是一个event类型的资源
        delete_blocks (*any); //
    case RETRIEVE_FAIL:
      remove_stub (*any); //访问该数据失败，则意味着可能服务器已经删除掉了该资源，所以将本地的数据删除？？
      break;
    default:
      return event;
    }
  }
  return client_poll (any, timeout);  //定时器查询、dnssd查阅、网络层面数据活动查阅。这个client意思是dnssd的client。
}


void der_init () {
  device_init ();   //  初始化 设备哈希表
  resource_init (); //  同样的，构建resource哈希表资源
  event_init ();  //初始化Event Timer，这Event指的是SE中的Event资源。
}
