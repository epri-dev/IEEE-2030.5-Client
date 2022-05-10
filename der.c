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
#include "settings.c"


/** A DerDevice is a representation of a DER EndDevice. 一个DER EndDevice的对象 */
typedef struct {
  uint64_t sfdi; ///< is the SFDI of the EndDevice
  uint8_t lfdi[20]; ///< is the LFDI of the EndDevice
  int metering_rate; ///< is the post rate for meter readings 上传数据的是时间周期
  Stub *mup; ///< is a pointer to the MirrorUsagePoint for this EndDevice
  List *readings; ///< is a list of MirrorMeterReadings
  List *derpl; ///< is a list of DER programs DER program List
  SE_DERControlBase_t base; //基本设定值？
  SE_DefaultDERControl_t *dderc; ///< is the default DER control
  Schedule schedule; ///< is the DER schedule for this device 需要执行的一系列事件
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

/** @brief Create a DER schedule for an EndDevice.
    @param edev is a pointer to an EndDevice Stub
*/
void schedule_der (Stub *edev);

/** @} */

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

/*这个函数只有在下面的 update_der 函数中被用到，修改了不影响整体。
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

void remove_programs (Schedule *s, List *derpl) {
  EventBlock *eb;
  HashPointer p;
  if (derpl == NULL) return;
  foreach_h (eb, &p, s->blocks) {
    if (find_by_data (derpl, eb->program)) {
      remove_block (s, eb);
      if (eb->status == Active) {
        device_response (s->device, eb->event, EventAbortedProgram);
      }
      eb->status = Aborted;
    }
  }
  free_list (derpl);
}

void schedule_der (Stub *edev) {
  SE_EndDevice_t *e = resource_data (edev);
  DerDevice *device = get_device (e->sFDI);
  Schedule *schedule = &device->schedule;
  Stub *fsa = NULL, *s, *t;
  List *l, *m, *derpl = NULL;
  SE_DefaultDERControl_t *dderc = NULL;
  LOG_I("in function schedule_der\n");
  if (!(fsa = get_subordinate (edev, SE_FunctionSetAssignmentsList))) return; //获取这个edev下面的FSA数据
  // add the lFDI if not provided by the server
  if (!se_exists (e, lFDI)) {
    se_set (e, lFDI);
    memcpy (e->lFDI, device->lfdi, 20);
  }
  // collect all DERPrograms for the device (sorted by primacy) 收集所有的 DERPrograms 并且排序
  foreach (l, fsa->reqs)
  if (s = get_subordinate (l->data, SE_DERProgramList)) //收集所有的 SE_DERProgramList
    foreach (m, s->reqs)
    derpl = insert_stub (derpl, m->data, s->base.info); //derpl 
  // handle program removal
  remove_programs (schedule, list_subtract (device->derpl, derpl));
  /* event block schedule might change as a result of program removal and
     primacy change so clear the block lists */
  schedule->scheduled = schedule->active = schedule->superseded = NULL;
  schedule->device = edev;
  // insert DER Control events into the schedule
  foreach (l, derpl) {
    s = l->data;
    SE_DERProgram_t *derp = resource_data (s);
    if (t = get_subordinate (s, SE_DERControlList))
      foreach (m, t->reqs) {
      EventBlock *eb;
      eb = schedule_event (schedule, m->data, derp->primacy);
      eb->program = s;
      eb->context = device;
    }
    if (!dderc && (t = get_subordinate (s, SE_DefaultDERControl)))
      dderc = resource_data (t);
  }
  device->derpl = derpl;
  device->dderc = dderc;
  insert_event (schedule, SCHEDULE_UPDATE, 0);
  // update_schedule (schedule);
  insert_event (device, DEVICE_SCHEDULE, 0);
}
