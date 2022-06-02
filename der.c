// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup der DER Device

    The DER module provides methods and structures for managing DER EndDevices.
    @{
*/

/*

详见 der_client.md，选取部分章节

Included with `der_client.c` is the module `der.c` that provides:

-   a DerDevice structure that stores information associated with a DER EndDevice
    instance.
    
-   a hash table container for DerDevices that uses the EndDevice SFDI as a
    retrieval key
    
-   a function `scheduler_der` that creates a local event schedule for an
    EndDevice based upon the group assignments for that EndDevice.

*/

#define DEVICE_SCHEDULE (EVENT_NEW+11)
#define DEVICE_METERING (EVENT_NEW+12)  //读取设备数据并且上传
#define DEFAULT_CONTROL (EVENT_NEW+13)  //

#include "debug_log.h"
#include "debug_log.h"
#include "settings.c"


/** A DerDevice is a representation of a DER EndDevice. 一个DER EndDevice的对象 ，代表了当前EndDevice中关联的DER，比如一个逆变器或者储能设备。*/
typedef struct {
  uint64_t sfdi; ///< is the SFDI of the EndDevice
  uint8_t lfdi[20]; ///< is the LFDI of the EndDevice
  int metering_rate; ///< is the post rate for meter readings 上传数据的是时间周期
  Stub *mup; ///< is a pointer to the MirrorUsagePoint for this EndDevice
  List *readings; ///< is a list of MirrorMeterReadings
  List *derpl; ///< is a list of DER programs DER program List 在这个设备上绑定的DERProgramList，这个数据目前仅仅用在schedule_der函数中，起到备份作用。
  SE_DERControlBase_t base; //基本设定值？
  SE_DefaultDERControl_t *dderc; ///< is the default DER control  这个设备的DefaultDERControl表，在DERControl执行完毕后，要执行这个 defaultControl
  Schedule schedule; ///< is the DER schedule for this device 需要执行的一系列事件的一个调度器
  Settings settings; ///< is the DER device settings 设定值
} DerDevice;  //一个DER设备的对象表示，最终需要操作的就是这个对象

/** @brief Get a DerDevice with the matching SFDI.
    @param sfdi is the EndDevice SFDI
    @returns a pointer to a DerDevice
    通过哈希表的搜索方法，找到这个设备的找个 “对象” 数据。get_device 通过global_hash实现。
*/
DerDevice *get_device (uint64_t sfdi);

/** @brief Load device settings from the directory.
    @param sfdi is the device SFDI
    @param path is the name of a directory containing XML files with the
    device settings
*/
void device_settings (uint64_t sfdi, char *path);

/** @brief Load a device certificate and store in the DerDevice hash.
    @param path is the file name of the device certificate
*/
void device_cert (const char *path);

/** @brief Load a set of device certificates.
    @param path is the name of the directory containing the device certificates.
*/
void device_certs (char *path);

/** @brief Create a DER schedule for an EndDevice.  创建一个DER调度器
    @param edev is a pointer to an EndDevice Stub
*/
void schedule_der (Stub *edev);

/** @} */

//用sfdi作为一个设备的key。
void *device_key (void *data) {
  DerDevice *d = data;
  return &d->sfdi;
}

// find_device, insert_device, remove_device, device_init
//以64bit的sfdi作为key的哈希表中的寻找响应的资源
global_hash (device, int64, 64) 

//通过对 sfdi 作哈希运算快速的得到设备数据指针。如果这个设备不存在，那么就新建一个。
DerDevice *get_device (uint64_t sfdi) {
  DerDevice *d = find_device (&sfdi);
  if (!d) {
    d = type_alloc (DerDevice);
    d->sfdi = sfdi;
    schedule_init (&d->schedule); //构建一个以mRID为索引的哈希表
    d->schedule.context = d;  //context就是这个对象自己？
    insert_device (d);  //如果这个对象不存在，则加入一个新的
  }
  return d;
}

//将该设备的settings变量填充（在"setting"文件夹中的内容）
void device_settings (uint64_t sfdi, char *path) {
  DerDevice *d = get_device (sfdi);
  process_dir (path, &d->settings, load_settings);
}

void _device_cert (const char *path, void *ctx) {
  uint8_t lfdi[20];
  uint64_t sfdi = load_device_cert (lfdi, path);
  DerDevice *d = get_device (sfdi);
  memcpy (d->lfdi, lfdi, 20);
}

void device_cert (const char *path) {
  _device_cert (path, NULL);
}

void device_certs (char *path) {
  process_dir (path, NULL, _device_cert);
}

#define copy_field(a, b, field) \
  if (se_exists (b, field)) a->field = b->field
#define copy_boolean(a, b, field) \
  if (se_true (b, field)) se_set_true (a, field)

/*这个函数只有在下面的 update_der 函数中被用到，修改了不影响整体。相当于是一个孤儿函数。
但是最新的sep.xsd文件中描述的数据结构跟现在这个函数不符，后续要用到的话需要对这个函数作一下修改。*/
void copy_der_base (SE_DERControlBase_t *a,
                    SE_DERControlBase_t *b, uint32_t mask) {
  uint32_t flags = b->_flags;
  b->_flags &= mask;
  a->_flags |= mask;
  copy_boolean (a, b, opModConnect);
  copy_boolean (a, b, opModEnergize);
  //copy_field (a, b, opModFixedPF);  //在最新的sep.xsd文件中，是不存在这个参数项的，而所以手动将其屏蔽掉。详见本次修改中的sep.xsd文件变化内容。
  copy_field (a, b, opModFixedVar);
  copy_field (a, b, opModFixedW);
  copy_field (a, b, opModFreqDroop);
  copy_field (a, b, opModFreqWatt);
  copy_field (a, b, opModHFRTMustTrip);
  copy_field (a, b, opModHVRTMomentaryCessation);
  copy_field (a, b, opModHVRTMustTrip);
  copy_field (a, b, opModLFRTMustTrip);
  copy_field (a, b, opModLVRTMomentaryCessation);
  copy_field (a, b, opModLVRTMustTrip);
  copy_field (a, b, opModVoltVar);
  copy_field (a, b, opModVoltWatt);
  copy_field (a, b, opModWattPF);
  copy_field (a, b, opModWattVar);
  copy_field (a, b, rampTms);
  b->_flags = flags;
}

/* 这个函数看起来像是一份“孤儿函数”，注释掉之后不影响整个编译。
void update_der (EventBlock *eb, int event) {
  Device *d = eb->info; int flags;
  SE_DERControl_t *c = resource_data (eb->event);
  switch (event) {
  case EVENT_START:
    copy_der_base (&d->base, &c->DERControlBase, eb->der); break;
  case EVENT_END:
    d->base._flags &= eb->der;
    if (d->dderc &&
	(flags = base_supersede (d->base, d->dderc->DERControlBase)))
      copy_der_base (&d->base, &d->dderc->DERControlBase, flags);
  }
}
*/

/*将当前已经激活了的任务，设置成退出状态*/
void remove_programs (Schedule *s, List *derpl) {
  LOG_I("remove_programs\n");
  EventBlock *eb;
  HashPointer p;
  if (derpl == NULL){
    LOG_I("  remove_programs : derpl is NULL , return\n");
    return;  /*在实际场景中，针对的情况就是前后两次derp内容没有变化的情况*/
  }
  
  foreach_h (eb, &p, s->blocks) {
    if (find_by_data (derpl, eb->program)) {
      remove_block (s, eb);
      if (eb->status == Active) { //如果当前已经是处于激活状态了，那么就发送一条消息到服务器中。
        device_response (s->device, eb->event, EventAbortedProgram);
      }
      eb->status = Aborted;
    }
  }
  free_list (derpl);
}

//对一个DER安排调度。在Demo测试指令中如果指定了测试schedule的话，那么将指定本函数为completion函数。在每次EndDevice的子级数据刷新之后，将被调用。
void schedule_der (Stub *edev) {
  SE_EndDevice_t *e = resource_data (edev);
  DerDevice *device = get_device (e->sFDI); //该EndDevice关联到的设备。
  Schedule *schedule = &device->schedule; //设备上的调度器
  Stub *fsa = NULL, *s, *t;
  List *l, *m, *derpl = NULL;
  SE_DefaultDERControl_t *dderc = NULL;
  int inserted_derp=0,added_derc=0;
  LOG_I("schedule_der (EndDevice completion) : href : %s\n",edev->base.name);
  if (!(fsa = get_subordinate (edev, SE_FunctionSetAssignmentsList))) {
    LOG_W("  schedule_der : no FSA on edev %s , return\n",edev->base.name);
    return; //获取这个edev下面的FSA数据
  }
  // add the lFDI if not provided by the server
  if (!se_exists (e, lFDI)) {
    se_set (e, lFDI);
    memcpy (e->lFDI, device->lfdi, 20);
  }
  
  // collect all DERPrograms for the device (sorted by primacy) 收集该DER下面所有的 DERPrograms 并且排序
  foreach (l, fsa->reqs) {
    LOG_I("  schedule_der : Dealing with FSA , href : %s\n",((Stub*)l->data)->base.name);
    if (s = get_subordinate (l->data, SE_DERProgramList)) { //l->data就是某一个fsa。这一行是为了得到FSA中的 DERProgramList 对象
      LOG_I("  schedule_der : DERProgramList href : %s\n",s->base.name);
      foreach (m, s->reqs) {  //m就是每一个DERProgram
        LOG_I("  schedule_der : insert a DERProgram to derpl , href : %s\n",((Stub*)m->data)->base.name);
        derpl = insert_stub (derpl, m->data, s->base.info); //derpl 得到了所有的 FSA下面的 所有的 DERProgram ，构成一个List
        inserted_derp++;
      }
    }
  }
  if(inserted_derp){
    LOG_I("  schedule_der : inserted %d DERPrograms\n",inserted_derp);
  }
  // handle program removal 
  /*
    device->derpl是上一次的，derpl是这一次的。
    list_subtract (device->derpl, derpl) 的意思是从上一次的derpl中删除本次中出现的derp，剩下的放到device->derpl中。
    然后再从schedule中删除这个留下来的部分。
    
    这个逻辑主要解决一种情况：如果本次derpl中的derp数量变少了，意思就是服务器端已经删除掉了，这样就要求同步到Client端本地来。
    如果derpl数量增加了，那么这一行相当于不存在。
  */
  remove_programs (schedule, list_subtract (device->derpl, derpl));
    
  /* event block schedule might change as a result of program removal and
     primacy change so clear the block lists */
  //这里全部删除（仅仅是删除链接，而Event的Stub依然存在本地），然后在好偶徐步骤中再重新加入进来。相当于每次调用这个 schedule_der 的时候都重新添加了。
  schedule->scheduled = schedule->active = schedule->superseded = NULL; 
  schedule->device = edev;  //指定调度器中的device为当前的edev。
  
  // insert DER Control events into the schedule 从DERProgram中取出DERControlList，然后放到调度器中。
  foreach (l, derpl) {
    s = l->data;  //s 是这里的 DERProgram 对象。
    SE_DERProgram_t *derp = resource_data (s);
    if (t = get_subordinate (s, SE_DERControlList)) /* t 是 SE_DERControlList 对象（ DERProgram的三个主要对象之一 ） */
      foreach (m, t->reqs) {  /*将 DERControlList 其下的 DERControl 全部放到 schedule 中*/
        EventBlock *eb;
        added_derc++;
        LOG_I("  schedule_der : call schedule_event on DERProgramList %s,with %s\n",s->base.name,t->base.name);
        eb = schedule_event (schedule, m->data, derp->primacy);   // m 是DERControl，将这个任务放到调度队列中去。这个函数不会重复添加已经存在的event。
        eb->program = s;
        eb->context = device;
    }
      
    /*如果 DefaultDERControl 存在以及之前没有获取过，那么就获取一下*/
    if (!dderc && (t = get_subordinate (s, SE_DefaultDERControl)))
      dderc = resource_data (t);
  }

  if(added_derc == 0){
    LOG_W("  schedule_der : no DERControl added to schedule\n");
  }else{
    LOG_I("  schedule_der : %d DERControl added\n",added_derc);
  }
  
  device->derpl = derpl;  //保存最新的derpl
  device->dderc = dderc;  //保存最新的dderc。
  //后面将周期性的调用 update_schedule 函数。这里是首次触发调用，相当于触发了一下。update_schedule将检查三个schedule队列中的状态变化，发送响应。
  insert_event (schedule, SCHEDULE_UPDATE, 0);
  // update_schedule (schedule);
  insert_event (device, DEVICE_SCHEDULE, 0);    //仅仅是在主任务队列中显示一下当前全部的调度情况。
}
