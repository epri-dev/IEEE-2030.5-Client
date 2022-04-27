// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdlib.h>

#include "der_client.c"
#include "query.c"

#define VERSION "0.2.11"

void version () {
  printf("IEEE 2030.5 client test version %s -- compiled: %s\n",
         VERSION, __DATE__);
  fflush (stdout);
}

void usage () {
  printf ("usage: client_test interface [device_cert ca_certs..]\n"
          "                   <subtype[:1][/path] | URI> [commands] \n\n");
  print_interfaces (0);
  exit (0);
}


/*下面的宏定义用于在test这个全局变量中标记将要测试的内容*/
#define GET_EDEV 1
#define GET_FSA (1<<1)
#define GET_DERP (1<<2)
#define GET_DERC (1<<3)
#define GET_TIME (1<<4)
#define GET_ALL (GET_EDEV|GET_FSA|GET_DERP|GET_DERC|GET_TIME)
#define GET_SELF (1<<5)
#define REGISTER_TEST (1<<6)
#define SCHEDULE_TEST (1<<7)
#define METER_TEST (1<<8)
#define INCLUDE_READINGS (1<<9)
#define ALARM_TEST (1<<10)
#define DEVICE_TEST (1<<11)
#define FSA_SUBSCRIBE (1<<12)
#define PUT_SETTINGS (1<<13)
#define PUT_PIN (1<<14)
#define DELETE_DEVICE (1<<15)
#define CLIENT_FOUND (1<<16)
#define INVERTER_CLIENT (1<<17)

/*
test记录了需要测试的内容，用bit位占位表示
secure的值表示是否是加密了连接
path就是dnssd参数中的/path
pin:在pIN测试中输入的pin值。
*/

int server = 0, test = 0, secure = 0, interval = 5 * 60, primary = 0, pin = 0;

Stub *edevs;  //在本测试代码中，获取到的EndDevice设备数据对象。

char *path = NULL;  //注意这里是一个全局变量。是在<subtype[:1][/path]       | URI>中的path值。可能为空。
uint64_t delete_sfdi;

//要解析的指令是 <subtype[:1][/path]       | URI> ， 可能跟msDNS功能有关
int subtype_query (char *arg, char *name) {
  
  printf("arg in subtyupe_query is %s\n",arg);

  char subtype[12] = {0}; // subtype 就是在 service_name 字符串数组中的值
  int qu = 0, n;
  char *p = strchr (arg, ':'), *q = strchr (arg, '/');
  if (p) {
    if (*(p + 1) == '1') qu = 1;  //在dnssd服务中，设置QU标志为1
    else {
      printf("No 1 in subtype,exit\n");
      return 0;
    }
    n = p - arg;
  } else if (q) n = q - arg;
  else n = strlen (arg);
  // n就是 subtype 这个指令参数字符串的长度
  if (n > 11) {
    printf("The n in subtype_query is more than 11,exit\n");
    return 0;
  }
  strncpy (subtype, arg, n);
  
  printf("subtype in subtype_query(%d bytes):%s\n",(int)strlen(subtype),subtype);

  if (server = se_subquery (subtype)) {//"bill", "dr", "derp", "file", "msg", "ppy", "rsps", "tp", "tm", "upt","edev", "mup", "sdev", "smartenergy"
    printf("Selected server value is %d\n",server);
    if (!secure) client_init (name, NULL);
    se_discover (server, qu);
    path = q;
    printf("path:%s\n",path);
    return 1;
  }
  return 0;
}

/* retrieval ： n. 找回，取回；（计算机系统信息的）检索；恢复，挽回 

解析URI字符串，得到字符串包含的URI各个要素。

*/
int uri_retrieval (char *arg) {
  Uri128 buf = {0};
  Uri *uri = &buf.uri;
  void *conn;
  Query q = {0};
  if (!uri_parse (&buf, arg, 127) || !uri->host) {
    if (uri->scheme) {
      printf ("error parsing URI %s\n", arg);
      exit (0);
    }
    return 0;
  }

  printf("Connecting to host...\n");
  
  conn = se_connect_uri (uri);
  if (uri->query && !parse_query (&q, uri->query)) {  //解析查询参数。
    printf ("error parsing URI query \"%s\"\n", uri->query);
    exit (0);
  }
  get_resource (conn, -1, uri->path, q.limit);  //获取这个资源，通常是dcap路径下的资源。
  return 1;
}

int cert_name (char *arg) {
  switch (file_type (arg)) {
  case FILE_DIR:
    load_cert_dir (arg);
    break;
  case FILE_REGULAR:
    load_cert (arg);
    break;
  default:
    return 0;
  }
  return 1;
}

/*导入本次测试所需要的各项参数
[0]         [1]       [2]                      [3]           
client_test interface [device_cert ca_certs..] <subtype[:1][/path] | URI>  [commands]
并执行

./build/client_test eth0 ./certs/csep_root.pem edev http://192.168.1.13:8080 fsa

./build/client_test eth0 http://192.168.1.13:8088/dcap sfdi 2694881444 edev

测试edev，测试软件调整到core 009
client_test eth0 http://192.168.1.13:8088/dcap edev

使用gdb调试步骤：
1） 编译的时候使用 ./build debug，即以带gdb功能方式编译。
2）进入到./build目录，然后启动gdb，输入 file client_test。
3）如果直接运行，则带上该程序运行参数：run eth0 http://192.168.1.13:8088/dcap edev

在vscode启用gdb调试工具的配置中，配置如下：

{
    "configurations": [
    
            {
                "type": "by-gdb",
                "request": "launch",
                "name": "Launch(gdb)",
                "program": "/home/vmuser/IEEE-2030.5-Client/build/client_test",
                "stopOnEntry": true,
                "programArgs": "eth0 http://192.168.1.13:8088/dcap edev",
                "cwd": "${workspaceFolder}"
            }   //使用vscode的智能提示功能，来得知该工具中使用那个参数来表示“arguments”：输入一个双引号然后观察给出的提示，选择一个合理的
        ]

}

现在使用了一个名字叫 “ GDB Debugger - Beyond  v0.9.6 ” 的gdb工具，感觉可以用。至少输出的log都是正常的。



*/
void options (int argc, char **argv) {
  int i = 2, index;
  char *name = argv[1]; //就是网卡名 interface

  printf("[options] argc:%d\n",argc);
  int j=0;
  for(j=0;j<argc;j++) printf("[options] arg[%d]:%s\n",j,argv[j]);
  
  if (argc < 3){
    printf("The input argument count less than 3,argv[0]=%s\n",argv[0]); 
    usage (); 
  }
  if ((index = interface_index (name)) < 0) {
    printf ("options: interface %s not found\n", name);
    exit (0);
  }
  //
  net_select (index); der_init ();

  // process certificates and base query
  while (i < argc) { //前面i的值是2，所以从[device_cert ca_certs..] 开始检查参数
    printf("[options] now dealing arg[%d]:%s\n",i,argv[i]);
    DerDevice *d;
    /*这里的意思是，选择其中一种方式来获取到数据，对应到前面的 <subtype[:1][/path] | URI> 这个参数项。
    尖括号表示"必须的选项"的意思，"|"表示“或”的意思。
    */
    if (subtype_query (argv[i], name)	|| uri_retrieval (argv[i])) { //获取到参数中所标注的这些资源
      printf("break;\n");
      i++;
      break; //如果参数中的存在这两项中的一项，解析下一个参数。
    } else if (!secure) { //应该是 [device_cert ca_certs...] 部分参数
      client_init (name, argv[i]);  //初始化有关加密和dnssd相关功能
      secure = 1;
      d = get_device (device_sfdi);
      memcpy (d->lfdi, device_lfdi, 20);
    } else if (!cert_name (argv[i])) {
      printf ("options: certificate file or directory %s does not exist\n",
              argv[i]);
      exit (0);
    }
    i++;

  }
  
  //这里看起来是要求使用证书文件
  if (!secure) {
    printf ("options: warning, no device certificate specified, "
            "TLS library will be uninitialized\n");
  } else if (i < 5) {
    printf ("options: no CA certificates specified, using ./certs\n");
    load_cert_dir ("certs");
  }

  
  printf("[options] process tests \n");
  
  // process tests，应该是指令中的[commands]部分。这一段是直接操作
  while (i < argc) {
    uint64_t sfdi;
    const char *const commands[] = {
      "sfdi", "edev", "fsa", "register", "pin", "primary", "all", "time",
      "self", "subscribe", "metering", "meter", "alarm", "poll", "load",
      "device", "delete", "inverter"
    };
    printf("parsing argv[%d]:%s ...\n",i,argv[i]);
    switch (string_index (argv[i], commands, 18)) { // 返回跟 command 相对应的一个字符串 。
    case 0: // sfdi，这项参数是必须要加上的
      if (++i == argc || !number64 (&device_sfdi, argv[i])) { //格式必须是 sfdi xxxxxx（sfdi值）
        printf ("sfdi command expects number argument\n");
        exit (0);
      }
      break;
    case 1: // edev
      printf("[options] edev\n");
      test |= GET_EDEV | GET_FSA;
      primary = 1;  //主要的？？
      break;
    case 2: // fsa
      test |= (GET_ALL ^ GET_DERC) | REGISTER_TEST;
      break;
    case 3: // register
      test |= GET_EDEV | REGISTER_TEST | PUT_SETTINGS;
      device_settings (device_sfdi, "settings");
      break;
    case 4: // pin
      test |= PUT_PIN;
      if (++i == argc || !number (&pin, argv[i])) {
        printf ("pin command expects number argument\n");
        exit (0);
      }
      break;
    case 5: // primary
      primary = 1;
    case 6: // all
      test |= GET_ALL | REGISTER_TEST | SCHEDULE_TEST | PUT_SETTINGS;
      device_settings (device_sfdi, "settings");
      break;
    case 7: // time
      test |= GET_TIME;
      break;
    case 8: // self
      test |= GET_SELF;
      break;
    case 9: // subscribe
      test |= GET_EDEV | GET_FSA | FSA_SUBSCRIBE;
      break;
    case 10: // metering
      test |= GET_ALL | REGISTER_TEST | INCLUDE_READINGS | PUT_SETTINGS;
      device_settings (device_sfdi, "settings");  //导入配置文件
    case 11: // meter
      test |= GET_EDEV | METER_TEST;
      break;
    case 12: // alarm
      test |= GET_ALL | REGISTER_TEST | ALARM_TEST;
      break;
    case 13: // poll
      if (++i == argc || !number (&active_poll_rate, argv[i])) {
        printf ("poll command expects an interval in seconds\n");
        exit (0);
      }
      break;
    case 14: // load
      if (++i == argc || !number64 (&sfdi, argv[i]) || ++i == argc) {
        printf ("load command expects SFDI and directory name\n");
        exit (0);
      }
      device_settings (sfdi, argv[i]);
      break;
    case 15: // device
      if (++i == argc || file_type (argv[i]) != FILE_DIR) {
        printf ("device command expects a directory name\n");
        exit (0);
      }
      device_certs (argv[i]);
      test |= GET_EDEV | GET_FSA | DEVICE_TEST;
      break;
    case 16: // delete
      if (++i == argc || !number64 (&delete_sfdi, argv[i])) {
        printf ("delete command expects number argument\n");
        exit (0);
      }
      test |= GET_EDEV | DELETE_DEVICE;
      break;
    case 17: // inverter
      test |= INVERTER_CLIENT;
      break;
    default:
      printf ("unknown command \"%s\"\n", argv[i]);
      exit (0);
    }
    i++;
  }
}

void test_fail (char *test, char *description) {
  printf ("%s failed: %s\n", test, description);
  exit (0);
}

Stub *dcap_mup = NULL;

void generic_alarm (Stub *r) {
  SE_LogEvent_t le = {0};
  static int id = 0;
  if (id == 10) {
    get_seq (r, 0, 255);
    r->completion = NULL;
    return;
  }
  le.createdDateTime = se_time ();
  sprintf (le.details, "Alarms Test");
  le.logEventCode = 1; // LE_GEN_SOFTWARE
  le.logEventID = id++;
  le.logEventPEN = 37244; // ZigBee Alliance, Inc.
  le.profileID = 2; // IEEE 2030.5
  se_post (r->conn, &le, SE_LogEvent, r->base.name);
}

void alarm_test (Stub *r) {
  printf ("alarm test\n");
  generic_alarm (r);
}

void alarm_dep (Stub *r) {
  generic_alarm (r->deps->data);
}

/*
通过PUT方法，上传一个DER设备的设置参数
这些参数放在本工程的“setting”下，由xml文件来描述。在本代码启动的时候，已经对这些数据作了解析。
*/
void put_der_settings (void *conn, Settings *ds, SE_DER_t *der) {
  link_put (conn, der, ds->dera, DERAvailability);
  link_put (conn, der, ds->dercap, DERCapability);
  link_put (conn, der, ds->derg, DERSettings);
  link_put (conn, der, ds->ders, DERStatus);
}

/*作为edev的 completion 函数*/
void edev_complete (Stub *r) {
  Stub *s, *t;
  SE_EndDevice_t *edev = resource_data (r);
  DerDevice *d = get_device (edev->sFDI);
  printf ("edev %s complete\n", edev->href);
  if (test & PUT_SETTINGS) {
    s = get_subordinate (r, SE_DERList);
    if (s && s->reqs) {
      Stub *t = s->reqs->data;
      put_der_settings (s->conn, &d->settings, resource_data (t));
    }
  }
  if (s = get_subordinate (r, SE_FunctionSetAssignmentsList)) {
    SE_FunctionSetAssignmentsList_t *fsal = resource_data (s);
    if (test & FSA_SUBSCRIBE) {
      /*
      if (se_exists (edev, SubscriptionListLink)
      && fsal->subscribable)
      subscribe (s, edev->SubscriptionListLink.href);
      */
    } else {
      s->poll_rate = se_exists (fsal, pollRate) ? fsal->pollRate : 900;
      poll_resource (s);
    }
    if (test & SCHEDULE_TEST) {
      schedule_der (r);
      r->completion = schedule_der;
    }
  }
  if (edev->sFDI == device_sfdi) {
    if (test & METER_TEST) {
      List *rds = NULL;
      SE_MirrorUsagePoint_t mup;
      create_mirror (&mup, NULL, 0, edev->lFDI);
      rds = create_reading (rds, "Real Power (W)", 38, 37);
      rds = create_reading (rds, "Apparent Power (VA)", 61, 37);
      rds = create_reading (rds, "Reactive Power (Var)", 63, 37);
      rds = create_reading (rds, "Power Factor (PF)", 65, 37);
      rds = create_reading (rds, "Voltage (V)", 29, 0);
      rds = create_reading (rds, "Current (A)", 5, 0);
      rds = create_reading (rds, "Frequency (Hz)", 33, 0);
      d->readings = rds;
      if (test & INCLUDE_READINGS)
        mup.MirrorMeterReading = rds;
      se_post (dcap_mup->conn, &mup, SE_MirrorUsagePoint,
               dcap_mup->base.name);
    }
    if (test & ALARM_TEST) {
      if (t = get_list_root (r->conn, edev, LogEventList))
        t->completion = alarm_test;
      else test_fail ("alarm", "no LogEventList defined for the EndDevice.");
    }
  }
}

/*获取所有的 DERProgram 下面的 DefaultDERControl和 DERControlList 和 DERCurveList 资源。*/
void der_program (Stub *d) {
  if (test & GET_DERC) {
    Stub *cl; //ControlList
    SE_DERProgram_t *dp = resource_data (d);
    get_dep (d, dp, DefaultDERControl); // 获取从属于dp的DefaultDERControl资源。
    if (cl = get_list_dep (d, dp, DERControlList)) {  //获取从属于dp的DERControlList资源。
      cl->poll_rate = active_poll_rate;
      poll_resource (cl); //获取到这些controlList
    }
    get_list_dep (d, dp, DERCurveList);
  }
}


/*仅仅是查询DERProgramList资源操作，主要作用函数是 poll_resource */
void poll_derpl (Stub *r) {
  SE_DERProgramList_t *derpl = resource_data (r);
  r->poll_rate = se_exists (derpl, pollRate) ? derpl->pollRate : 900;
  poll_resource (r);
}

//select the highest priority DERProgram from a DERProgramList

//按照前面的注释来看，意思是第一个的DERProgram的优先级（priority）最高？
void der_program_list (Stub *r) {
  if (primary && r->reqs) der_program (r->reqs->data);  //
}


/* 测试中获取到FSA资源 */
int fsa (Stub *r) {
  if (test & GET_DERP) {
    SE_FunctionSetAssignments_t *fsa = resource_data (r);
    Stub *d = get_list_dep (r, fsa, DERProgramList);//获取到 DERProgramList 资源 
    if (d && primary) {
      d->completion = der_program_list;
      return 1;
    }
  }
  return 0;
}

// select the highest priority FSA that has a DERProgramList
void fsa_list (Stub *r) {
  List *l;
  foreach (l, r->reqs) if (fsa (l->data)) break;
}

//
void get_edev_subs (Stub *edevs) {
  List *l;
  if (test & REGISTER_TEST) return; // wait for registration
  foreach (l, edevs->reqs) {
    Stub *s = l->data;
    SE_EndDevice_t *e = resource_data (s);
    if (test & INVERTER_CLIENT && e->sFDI != device_sfdi) continue;
    s->completion = edev_complete;
    get_list_dep (s, e, DERList); //获取到 EndDevice 下面的 DERList。
    if (test & GET_FSA) {
      if ((s = get_list_dep (s, e, FunctionSetAssignmentsList)) && primary) //获取到 EndDevice 下面的 FunctionSetAssignmentsList
        s->completion = fsa_list;
    }
  }
}

void check_registration (Stub *r) {
  SE_Registration_t *reg = resource_data (r);
  if (reg->pIN == 111115) {
    printf ("registration succeeded\n");
    test &= ~REGISTER_TEST;
    if (edevs) get_edev_subs (edevs);
  } else test_fail ("registration", "pIN does not match 111115");
}

void end_device (Stub *r) {
  SE_EndDevice_t *e = resource_data (r);
  if (test & DELETE_DEVICE && e->sFDI == delete_sfdi) {
    delete_stub (r);
    test &= ~DELETE_DEVICE;
  }
  if (e->sFDI == device_sfdi) {
    test |= CLIENT_FOUND;
    if (device_sfdi == delete_sfdi) return;
    if (test & REGISTER_TEST) {
      if (se_exists (e, RegistrationLink)) {
        if (test & PUT_PIN) {
          SE_Registration_t reg = {0};  
          reg.dateTimeRegistered = se_time ();  //看起来是只要填充下面这两项数据就够了。
          reg.pIN = pin;  //这个pin的意思不是很清楚。
          se_put (r->conn, &reg, SE_Registration, e->RegistrationLink.href);
          test &= ~REGISTER_TEST;
          if (edevs) get_edev_subs (edevs);
        } else get_root (r->conn, e, Registration);
      } else test_fail ("registration",
                          "EndDevice does not contain RegistrationLink.");
    }
  }
}

/* 作为 dcap 对象的 completion 函数 */
void edev_list (Stub *r) {
  edevs = r;
  if (test & DELETE_DEVICE) {
    test_fail ("delete device", "client EndDevice instance not found");
  }
  if (!(test & CLIENT_FOUND)) {
    if (test & GET_EDEV) {
      SE_EndDevice_t edev = {0};
      edev.sFDI = device_sfdi;
      se_post (r->conn, &edev, SE_EndDevice, r->base.name); //POST
    }
    if (test & DEVICE_TEST)
      test_fail ("device test", "client EndDevice instance not found");
  } else if (device_sfdi != delete_sfdi) {
    if (test & DEVICE_TEST) {
      List *l;
      foreach (l, r->reqs) {
        DerDevice *d;
        SE_EndDevice_t *edev = resource_data (l->data);
        if (d = find_device (&edev->sFDI)) {
          if (memcmp (edev->lFDI, d->lfdi, 20))
            test_fail ("device test",
                       "retrieved LFDI does not match computed LFDI");
        } else
          test_fail ("device test", "no device certificate for EndDevice");
      }
      printf ("device test: EndDeviceList validated\n");
    }
    get_edev_subs (r);
  }
  r->completion = NULL;
}


void dcap (Stub *r) {
  SE_DeviceCapability_t *dcap = resource_data (r);
  if (test & GET_TIME) {
    if (!get_root (r->conn, dcap, Time))  //获取到dcap中的Time资源。
      test_fail ("time", "no TimeLink in DeviceCapability");
  }
  if (test & GET_EDEV) {  
    if (r = get_list_root (r->conn, dcap, EndDeviceList)) // 获取
      r->completion = edev_list;
    else test_fail ("edev", "no EndDeviceListLink in DeviceCapability");
  }
  if (test & GET_SELF) {
    if (!get_root (r->conn, dcap, SelfDevice))
      test_fail ("self", "no SelfDevice in DeviceCapability");
  }
  if (test & METER_TEST) {
    if (!(dcap_mup = get_list_root (r->conn, dcap, MirrorUsagePointList)))
      test_fail ("meter", "no MirrorUsagePointLink in DeviceCapability");
  }
}

//请求时间同步
void time_sync (Stub *s) {
  SE_Time_t *tm = resource_data (s);  //获取到时间资源
  s->poll_rate = se_exists (tm, pollRate) ? tm->pollRate : 900; //默认是900秒来获取一次数据。
  poll_resource (s);  //添加一个异步请求
  set_time (tm);  //？？不知道为什么这里需要重新设置一下时间。
}

void update_mup (Stub *s) {
  SE_MirrorUsagePoint_t *mup = resource_data (s);
  uint64_t sfdi = sfdi_gen (mup->deviceLFDI);
  DerDevice *d = find_device (&sfdi);
  if (d && d->readings) {
    d->mup = s;
    d->metering_rate = se_exists (mup, postRate) ? mup->postRate : 5 * 60;
    if (!(test & INCLUDE_READINGS)) {
      List *r;
      foreach (r, d->readings)
      se_post (d->mup->conn, r->data, SE_MirrorMeterReading,
               resource_name (d->mup));
    }
    insert_event (d, DEVICE_METERING, 0);
  }
}

void post_readings (DerDevice *d) {
  List *r;
  int64_t now = se_time (), next = now + d->metering_rate;
  printf ("post_readings\n");
  foreach (r, d->readings) {
    SE_MirrorMeterReading_t *mmr = r->data;
    SE_Reading_t *rd = &mmr->Reading;
    se_clear (mmr, ReadingType);
    se_set (mmr, Reading);
    if (mmr->nextUpdateTime) {
      se_set (mmr, lastUpdateTime);
      mmr->lastUpdateTime = mmr->nextUpdateTime;
    } else se_set (mmr, nextUpdateTime);
    mmr->nextUpdateTime = next;
    rd->_flags = SE_value_exists | SE_timePeriod_exists;
    rd->timePeriod.start = now;
    switch (mmr->ReadingType.uom) {
    case 5:
      rd->value = 20;
      break; // Amps
    case 29:
      rd->value = 230;
      break; // Volts
    case 33:
      rd->value = 60;
      break; // Hz
    case 38:
    case 61:
    case 63:
      rd->value = 5000;
    }
    se_post (d->mup->conn, mmr, SE_MirrorMeterReading,
             resource_name (d->mup));
  }
  insert_event (d, DEVICE_METERING, next);
}

void log_event (Stub *log) {
  List *l;
  foreach (l, log->reqs) {
    SE_LogEvent_t *le = resource_data (l->data);
    switch (le->functionSet) {
    case 12: // Time
      switch (le->logEventCode) {
      case 0: // TM_TIME_ADJUSTED
        printf ("server log event TM_TIME_ADJUSTED\n");
      }
    }
  }
}

void self_device (Stub *r) {
  SE_SelfDevice_t *self = resource_data (r);
  r = get_list_dep (r, self, LogEventList);
  r->poll_rate = 30;
  r->completion = log_event;
  poll_resource (r);
}


/*这里相当于列举出了各个重要的于服务器交互的内容
这个函数是 process_http中  的   DepFunc 函数，应用于*/
void test_dep (Stub *r) {
  switch (resource_type (r)) {
  case SE_Time:
    time_sync (r);  //时间同步？
    break;
  case SE_DERProgram:
    if (!primary) der_program (r);  //获取 DERProgram 下面的三个主要资源。
    break;
  case SE_DERProgramList:
    poll_derpl (r);
    break;
  case SE_FunctionSetAssignments:
    if (!primary) fsa (r);  //获取FSA下面的部分资源。
    break;
  case SE_DeviceCapability:
    dcap (r);
    break;
  case SE_EndDevice:
    end_device (r);
    break;
  case SE_Registration:
    check_registration (r);
    break;
  case SE_MirrorUsagePoint:
    update_mup (r);
    break;
  case SE_SelfDevice:
    self_device (r);
  }
}


//client_test。这个是一个测试程序，用于展示这套代码提供的API在通过SunSpec 测试程序中的顺应性测试。

int main (int argc, char **argv) {
  Service *s;
  void *any;  //这个any是一个void类型的指针，即意思为任意类型的数据。这些数据类型的确定，由当时新增该事件的时候的type所决定。所以此时已知晓type之后，就可以正常处理了。
  version ();
  platform_init ();

  options (argc, argv);

  while (1) {
    switch (der_poll (&any, -1)) {  //这个函数的执行结果是由内部的 client_poll 提供的
    case SERVICE_FOUND: //主要针对DNSSD的
      s = any;
      print_service (s);
      if (test) get_dcap (s, secure); //test中任意一个bit置位的，则必须先查询到dcap资源
      else if (path)
        get_resource (service_connect (s, secure), -1, path, 0);
      else get_path (s, secure);
      break;
    case TCP_ACCEPT:  //作为服务器来说的一个事件。注意下面没有break。
      // accept_notifier (any);
    case TCP_PORT:      //在连接到服务器成功后，socket上一旦有活动的数据，将触发该事件。
      if (conn_session (any)) { //如果是连接的
        http_debug (any, 1);  //使能debug。
        // if (http_client (any))
        process_http (any, test_dep);
        // else process_notifications (any, test_dep);
      }
      break;
    case TCP_TIMEOUT: //连接不上对方服务器，这仅仅是清理现场并且给出一个提示。
      cleanup_http (any);
      printf ("Connection timed out\n");
      return 0;
    case TCP_CLOSED:
      cleanup_http (any);
      printf ("Connection closed\n");
      return 0;
    case DEVICE_SCHEDULE:
      print_event_schedule (any);
      break;
    case EVENT_START:
      print_event_start (any);
      break;
    case EVENT_END:
      print_event_end (any);
      break;
    case DEFAULT_CONTROL:
      print_default_control (any);
      break;
    case DEVICE_METERING:
      post_readings (any);  //上报所有设备的读取到的数据
      break;
    }
  }
}
