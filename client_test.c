// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdlib.h>

#include "debug_log.h"
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
#define REGISTER_TEST (1<<6)  //注册测试。在某一次测试完毕后，就不用再次测试了。所以这个mask同时用来作为一个flag使用。
#define SCHEDULE_TEST (1<<7)
#define METER_TEST (1<<8)
#define INCLUDE_READINGS (1<<9)
#define ALARM_TEST (1<<10)
#define DEVICE_TEST (1<<11)
#define FSA_SUBSCRIBE (1<<12)
#define PUT_SETTINGS (1<<13)
#define PUT_PIN (1<<14) //通过PUT指令来向服务器注册一个DER，PIN值为用户在测试指令中通过“pin xxxxxx”来指定的一个PIN值。
#define DELETE_DEVICE (1<<15)
#define CLIENT_FOUND (1<<16)  //这个只是一个标记位，并不是一个测试项目标记。这里的CLINET的意思就是DER Client，且Pin值为111115的DER Client。
#define INVERTER_CLIENT (1<<17)

/*
test记录了需要测试的内容，用bit位占位表示。
凡是有任意一个bit置位了，则将执行dcap的获取动作。

secure的值表示是否是加密了连接
path就是dnssd参数中的/path
pin:在pIN测试中输入的pin值。
*/

int server = 0, test = 0, secure = 0, primary = 0, pin = 0;

Stub *edevs;  //在本测试代码中，获取到的EndDeviceList设备数据对象，包含了List和各个EndDevice。

char *path = NULL;  //注意这里是一个全局变量。是在<subtype[:1][/path]       | URI>中的path值。可能为空。
uint64_t delete_sfdi;

//要解析的指令是 <subtype[:1][/path]       | URI> ， 可能跟msDNS功能有关。我们的测试代码中，用的是直接连接服务器，所以这个函数用不到。
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

解析URI字符串，得到字符串包含的URI各个要素。在完成解析URL地址后，将尝试连接到服务器。
连接到服务器之后，
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

  LOG_I("Connecting to host...\n");
  
  conn = se_connect_uri (uri);  //连接到服务器
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

client_test eth0 http://192.168.1.13:8088/dcap primary

使用gdb调试步骤：
1） 编译的时候使用 ./build debug，即以带gdb功能方式编译。
2）进入到./build目录，然后启动gdb，输入 file client_test。
3）如果直接运行，则带上该程序运行参数：run eth0 http://192.168.1.13:8088/dcap edev


IEEE model : https://zepben.github.io/evolve/docs/2030-5/

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
  
    /* 这里的意思是，选择其中一种方式来获取到数据，对应到前面的 <subtype[:1][/path] | URI> 这个参数项。
    尖括号表示"必须的选项"的意思，"|"表示“或”的意思。*/
    if (subtype_query (argv[i], name)	|| uri_retrieval (argv[i])) { //获取到参数中所标注的这些资源（将开始连接到服务器并且发送GET请求）
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
  
  // process tests，应该是指令中的 [commands] 部分。这一段是直接操作
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
    case 4: // pin 将执行 PUT 操作。
      test |= PUT_PIN;
      if (++i == argc || !number (&pin, argv[i])) {
        printf ("pin command expects number argument\n");
        exit (0);
      }
      break;
    case 5: // primary
      primary = 1;  //注意这里是没有break的，将跟下面的联合
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
    LOG_W("  generic_alarm : set alarm's completion() to NULL\n");
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
  LOG_I("put_der_settings\n");
  link_put (conn, der, ds->dera, DERAvailability);
  link_put (conn, der, ds->dercap, DERCapability);
  link_put (conn, der, ds->derg, DERSettings);
  link_put (conn, der, ds->ders, DERStatus);
}

/*作为 edev 的 completion 函数*/
void edev_complete (Stub *r) {
  Stub *s, *t;
  SE_EndDevice_t *edev = resource_data (r);
  DerDevice *d = get_device (edev->sFDI);
  LOG_I("edev_complete (edev completion): %s\n",edev->href);

  /*如果之前测试的时候有设定要做“PUT_SETTINGS”操作，那么就在这里执行。这里执行的是上传DER的所有设置参数操作。*/
  if (test & PUT_SETTINGS) {//在all和primary中存在该测试项目
    s = get_subordinate (r, SE_DERList);
    if (s && s->reqs) {
      Stub *t = s->reqs->data;
      put_der_settings (s->conn, &d->settings, resource_data (t));
    }
  }
  
  /*如果这个edev存在 FunctionSetAssignmentsList ，那么就即将其加入到轮询中，后面要持续更新*/
  if (s = get_subordinate (r, SE_FunctionSetAssignmentsList)) {
    SE_FunctionSetAssignmentsList_t *fsal = resource_data (s);
    if (test & FSA_SUBSCRIBE) {
      /*
      if (se_exists (edev, SubscriptionListLink)
      && fsal->subscribable)
      subscribe (s, edev->SubscriptionListLink.href);
      */
    } else {//在all和primary中走这个路径
      s->poll_rate = se_exists (fsal, pollRate) ? fsal->pollRate : 900; //通常要再poll_resource之前加入这一行，避免有些resource下发的时候没有带上pollRate数据。
      LOG_I("  edev_complete : call poll_resource(),poll_rate=%d\n",s->poll_rate);
      poll_resource (s);  //  这个测试逻辑思维：遇到一个EndDevice，就去poll一下FSAList
    }
    
    if (test & SCHEDULE_TEST) { //在all和primary中存在该测试项目。如果一个EndDevice的子级资源满足了，那么就执行schedule_der。
      LOG_I("  edev_complete : call schedule_der() on edev %s\n",r->base.name);
      schedule_der (r);
      LOG_W("  edev_complete : set EndDevice's completion() to schedule_der\n");
      r->completion = schedule_der;
      /* 原先的edev的completion函数是本函数 edev_complete ，而这里如果符合执行 schedule 的条件的话，则修改成了 schedule_der 函数 。
      后续每刷新一次 FunctionSetAssignmentsList，都会调用一次 schedule_der 。
      每次edev的子级满足一次，则都会执行一遍schedule_der。*/
    }
    LOG_D("edev_complete : exit\n");
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
      if (t = get_list_root (r->conn, edev, LogEventList)){
        LOG_W(" edev_complete (edev completion): set LogEventList's completion() to alarm_test\n");
        t->completion = alarm_test;
      }
      else test_fail ("alarm", "no LogEventList defined for the EndDevice.");
    }
  }
}

/*获取所有的 DERProgram 下面的 DefaultDERControl 和 DERControlList 和 DERCurveList 资源。*/
void der_program (Stub *d) {
  LOG_I("der_program : %s\n",d->base.name);
  if (test & GET_DERC) {  ////在all和primary中都存在该测试项目
    //LOG_I("  der_program : test GET_DERC\n");
    Stub *cl; //ControlList
    SE_DERProgram_t *dp = resource_data (d);
    LOG_I("  der_program : get_dep DefaultDERControl if exists\n");
    get_dep (d, dp, DefaultDERControl); // 获取从属于dp的 DefaultDERControl 资源。这个资源要不存在要不就不存在，只有List才会有all值。

    if( se_exists(dp,DERControlListLink) && dp->DERControlListLink.all > 0 ){
      LOG_I("  der_program : DERControlLinkLink exist,all:%d\n",dp->DERControlListLink.all);
      if (cl = get_list_dep (d, dp, DERControlList)) {  //获取从属于dp的DERControlList资源。
        cl->poll_rate = active_poll_rate;
        poll_resource (cl); //获取到这些ControlList
      }
    }else{
      LOG_D("  der_program : DERControlListLink not existed or all = 0\n");
    }
    
    if( se_exists(dp,DERCurveListLink) && dp->DERCurveListLink.all > 0 ){
      LOG_I("  der_program : get_list_dep DERCurveList,all=%d\n",dp->DERCurveListLink.all);
      get_list_dep (d, dp, DERCurveList);
    }else{
      LOG_D("  der_program : DERCurveListLink not existed or all = 0\n");
    }
  }
}


/*将DErProgramList放到轮询队列中。作为test_dep函数中DERProgramList资源的case 函数 */
void poll_derpl (Stub *r) {
  LOG_I("poll_derpl : %s\n",r->base.name);
  SE_DERProgramList_t *derpl = resource_data (r);
  r->poll_rate = se_exists (derpl, pollRate) ? derpl->pollRate : 900; //如果pollRate属性不不存在，那么就设定该值为900。
  poll_resource (r);
}

//select the highest priority DERProgram from a DERProgramList

/*按照前面的注释来看，意思是第一个的DERProgram的优先级（priority）最高？
这个函数是 DERProgramList 的 completion 函数 */
void der_program_list (Stub *r) {
  LOG_I("der_program_list (DERProgramList completion function) : %s\n",r->base.name);
  if (primary && r->reqs) {
    //这里应该直接改成查询每一个reqs成员的。原代码仅仅对reqs中的第一个元素执行了der_program函数。这里可能存在问题。
    der_program (r->reqs->data);
    LOG_I("  der_program_list : after der_program,set DERProgramList completion to NULL\n");
    r->completion = NULL; //对于core011测试case，只需要执行一次该函数即可。
  }
}


/* 测试中获取到FSA资源 
这个函数是test_dep中的 FunctionSetAssignments 的 case 。*/
int fsa (Stub *r) {
  LOG_I("fsa : resource href:%s\n",r->base.name);
  if (test & GET_DERP) {  //在all和primary中都存在该测试项目  
    SE_FunctionSetAssignments_t *fsa = resource_data (r);
    LOG_I("  fsa : get_list_dep DERProgramList\n");
    Stub *d = get_list_dep (r, fsa, DERProgramList);//获取到 DERProgramList 资源 。d 是 DERProgramList 资源（对象）
    if (d && primary) { //只要测试指令中包含了 primary 命令，则将调用该回调函数。
      LOG_W("  fsa : set DERProgramList's completion() to  der_program_list\n");
      d->completion = der_program_list; //一个fsa中包含了一个program_list。对获取完毕了之后的DERProgramList资源的处理是异步的，这里设定了处理用的回调函数。
      return 1;
    }
  }
  return 0;
}


// select the highest priority FSA that has a DERProgramList

/*这个函数作为 FunctionSetAssignmentsList 资源的 completion 函数，处理List中的每一个FSA资源对象。 */
void fsa_list (Stub *r) {
  LOG_I("fsa_list (FunctionSetAssignmentsList completion ) : %s\n",r->base.name);
  List *l;
  foreach (l, r->reqs){  /*l取得的是fsalist中的子级，即单个fsa*/
    #if 0 //这个是原始代码版本
    if (fsa (l->data)){ 
      LOG_W("fsa_list:call fsa() on %s returns 1 , break\n",((Stub*)l->data)->base.name);
      break;  //看起来问题出在这里：只要fsa()返回1，就跳出。这样执行到第一个fsa。
    }
    #else //这个是修改后的版本。还需要继续改进：判断其中的DERProgramListLink属性all的值是否大于0。只有大于0才去查询。否则可以省掉。
    fsa (l->data);
    #endif
  }
  LOG_I("  fsa_list : set completion to NULL\n");
  r->completion = NULL; //针对core011的测试case，这个completion函数只需要执行一次即可。
}

/*获取EndDevice的子层资源，即EndDevice。参数edevs通常是 EndDeviceList 类型对象。
对每一个edev对象执行：
1）获取DERList对象
2）获取 FunctionSetAssignmentsList 对象并设定 completion 函数为 fsa_list 函数，即在EndDevice数据全部满足后，调用fsa_list函数。*/
void get_edev_subs (Stub *edevs) {  //这个参数对象是 EndDeviceList
  LOG_I("get_edev_subs : %s\n",edevs->base.name);
  List *l;
  if (test & REGISTER_TEST){
    LOG_W("  get_edev_subs : register test not excuted,return\n");
    return; // wait for registration 之前如果还没有验证过了register测试，就先不往下做。
  }
  LOG_I("  get_edev_subs : register test excuted,go on\n");
  foreach (l, edevs->reqs) {  //对每一个 EndDeviceList 的子级对象，就是EndDevice作处理。后面可以试试，作为DER Client的话，可以仅仅取自己pin相同的那个。
    Stub *s = l->data;  //s是子级，就是 EndDevice 对象。
    LOG_I("  get_edev_subs : dealing %s\n",s->base.name);
    SE_EndDevice_t *e = resource_data (s);
    if (test & INVERTER_CLIENT && e->sFDI != device_sfdi) continue; //作为DER Client，仅仅测试于本DER相同的EndDevice对象。通常指令中加入 inverter 指令。
    LOG_W("  get_edev_subs : set EndDevice's completion() to edev_complete\n");
    s->completion = edev_complete;  //将为每一个 EndDevice 赋予该执行函数。
    get_list_dep (s, e, DERList); // 获取到 EndDevice 下面的 DERList。通常 href:/edev/edev0/der
    if (test & GET_FSA) { //在all和primary中都存在该测试项目
    LOG_I("  get_edev_subs : getting FunctionSetAssignmentsList on %s\n",s->base.name);
      if ((s = get_list_dep (s, e, FunctionSetAssignmentsList)) && primary){ //获取到 EndDevice 下面的 FunctionSetAssignmentsList
        LOG_W(" get_edev_subs : set FunctionSetAssignmentsList's completion() to fsa_list\n");
        s->completion = fsa_list;
      }
    }
  }
}

//在获取到了 Registration 数据之后，执行检查动作。
void check_registration (Stub *r) {
  LOG_I("check_registration,%s\n",r->base.name);
  SE_Registration_t *reg = resource_data (r);
  if (reg->pIN == 111115) { //这个111115是这个程序程序所公用的一个值。
    LOG_D("registration succeeded,register test passed,clear REGISTER_TEST bit,call get_edev_subs\n");
    test &= ~REGISTER_TEST; //在primary或者all指令的测试指令下，该bit初始值是置位的。这里一旦执行过了，就清除。
    if (edevs) get_edev_subs (edevs); //edevs 在函数 edev_list 中被初始化填充。所以基本逻辑是，先执行edev_list，然后再执行到这里，然后再执行一次get_edev_subs。
  } else test_fail ("registration", "pIN does not match 111115");
}


//在获取到了EndDeviceList之后执行的操作。

void end_device (Stub *r) {
  LOG_I("end_device : %s\n",r->base.name);
  SE_EndDevice_t *e = resource_data (r);
  
  //如果要执行删除设备操作
  if (test & DELETE_DEVICE && e->sFDI == delete_sfdi) {
    delete_stub (r);
    test &= ~DELETE_DEVICE;
  }
  
  //
  if (e->sFDI == device_sfdi) {
    LOG_I("  end_device : find target device_sfdi,%ld\n",device_sfdi);
    test |= CLIENT_FOUND; //可能表示该操作已经执行过了，后面不要再执行。这个bit表示是否已经找到了这个DER了。
    if (device_sfdi == delete_sfdi) return;
    /*果是测试了primary，那么这个项是测试的。
    在check_registration函数中，如果遇到了PIN 11115之后， REGISTER_TEST 将被清除。
    意思就是前面在遍历每一个EndDevice的时候，如果发现了已经存在该PIN的设备，那么后面这里的注册过程就可以省掉了。
    这个pin的设备是测试流程中规定必须存在的。*/
    if (test & REGISTER_TEST) {
      if (se_exists (e, RegistrationLink)) {
        if (test & PUT_PIN) { //如果用户没有指定 "pin xxxxxx" 指令，则该项操作不会执行。
          LOG_I("  end_device : test & PUT_PIN == true\n");
          SE_Registration_t reg = {0};  
          reg.dateTimeRegistered = se_time ();  //看起来是只要填充下面这两项数据就够了。
          reg.pIN = pin;  //这个pin的意思不是很清楚。
          se_put (r->conn, &reg, SE_Registration, e->RegistrationLink.href);
          test &= ~REGISTER_TEST;
          if (edevs) get_edev_subs (edevs);
        } else get_root (r->conn, e, Registration); //如果测试条件中不需要PUT测试，那么就直接获取一下 Registration 数据。
      } else test_fail ("registration",
                          "EndDevice does not contain RegistrationLink.");
    }
  }
}


/*
作为 EndDeviceList 对象的 completion 函数，
就是 EndDeviceList 的子层（第一层就够了，不需要所有的层次都到齐）EndDevices到齐之后，执行的动作。
在dcap资源获取到之后，主动指定该函数。

注意：EndDeviceList函数的completion函数在执行过一次之后，就设置成了NULL，即后续不会再继续调用该函数。
*/

void edev_list (Stub *r) {
  LOG_I("edev_list : %s\n",r->base.name);
  edevs = r;

  if (test & DELETE_DEVICE) {
    test_fail ("delete device", "client EndDevice instance not found");
  }
  
  if (!(test & CLIENT_FOUND)) { //表示的意思是：如果client还是没有被找到，那么执行下面的指令。但是在我们的测试指令中，通常是包含了被测试的Client的。
    if (test & GET_EDEV) {  //在测试指令all和primary中将测试到该GET_EDEV项目。
      LOG_I("  edev_list : CLIENT_FOUND is false,POST EndDevice\n");
      SE_EndDevice_t edev = {0};
      edev.sFDI = device_sfdi;
      se_post (r->conn, &edev, SE_EndDevice, r->base.name); //POST 如果没有这个设备，那么就放上去一个？？
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
  
  /*由陈立飞添加，test case CORE 003 要求在获取到了该资源后，后续也要定时轮询。原始代码是没有这个的。*/
  SE_EndDeviceList_t *el = resource_data(r);
  r->poll_rate = se_exists (el, pollRate) ? el->pollRate : 300; //添加一个默认的值，该值可以修改。尽量短一点，不能比测试case中的整体时间要长（否则必然测不过）
  LOG_I("  edev_list : add EndDeviceList to poll resource routine,pollRate:%d\n",r->poll_rate);
  poll_resource (r);  //自动轮询该资源。
  /*陈立飞添加完毕*/
  
  LOG_W("  edev_list : set EndDeviceList's completion() to NULL\n");
  r->completion = NULL; //看起来 EndDeviceList数据只需要获取1次，后面就不需要了，所以自己结束掉。
}

/*在收到了完整的dcap数据之后的处理程序。
这个就跟整个test程序的逻辑有关。在获取到了dcap资源后，通常仅仅去获取time和EndDeviceList两种资源。*/
void dcap (Stub *r) {
  LOG_I("dcap\n");
  SE_DeviceCapability_t *dcap = resource_data (r);

  //陈立飞添加开始
  /* test case CORE 003 要求在获取到了该资源后，后续也要定时轮询。原始代码是没有这个的。*/
  r->poll_rate = se_exists (dcap, pollRate) ? dcap->pollRate : 600;//有时候这个参数是不带的，所以必须预先设定一个默认值。
  LOG_I("  dcap : add DeviceCapability to poll resource routine,pollRate:%d\n",r->poll_rate);
  poll_resource(r); //自动轮询该资源
  //陈立飞添加结束

  if (test & GET_TIME) {  //当指令为all或者primary的时候，该测试项目都将被包含。
    if (!get_root (r->conn, dcap, Time))  //获取到dcap中的Time资源。
      test_fail ("time", "no TimeLink in DeviceCapability");
  }
  
  if (test & GET_EDEV) {  //该项目在all和primary指令中都将被包含。执行获取EndDeviceList数据。
    if (r = get_list_root (r->conn, dcap, EndDeviceList)){ // 获取 EndDeviceList 资源
      LOG_W("  dcap : set EndDeviceList's completion() to edev_list\n");
      r->completion = edev_list;
    }
    else test_fail ("edev", "no EndDeviceListLink in DeviceCapability");
  }
  
  if (test & GET_SELF) {  //在SE中，通常没有看到 SelfDevice 
    if (!get_root (r->conn, dcap, SelfDevice))
      test_fail ("self", "no SelfDevice in DeviceCapability");
  }
  
  if (test & METER_TEST) {  //通常也没有 MirrorUsagePointList 
    if (!(dcap_mup = get_list_root (r->conn, dcap, MirrorUsagePointList)))
      test_fail ("meter", "no MirrorUsagePointLink in DeviceCapability");
  }
  }

//请求时间同步
void time_sync (Stub *s) {
  LOG_I("time_sync\n");
  SE_Time_t *tm = resource_data (s);  //获取到时间资源
  s->poll_rate = se_exists (tm, pollRate) ? tm->pollRate : 900; //默认是900秒来获取一次数据。
  poll_resource (s);  //添加一个异步请求。首次获取到数据之后，将进入这种自动轮询模式。这样该时间资源就能够自动定时刷新。
  set_time (tm);  //在获取到服务器时间后，同步一下本地时间，保持于服务器时间一致。
}

void update_mup (Stub *s) {
  LOG_I("update_mup\n");
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
  LOG_I("self_device\n");
  SE_SelfDevice_t *self = resource_data (r);
  r = get_list_dep (r, self, LogEventList);
  r->poll_rate = 30;
  LOG_W("  self_device : set self_device's completion() to log_event\n");
  r->completion = log_event;
  poll_resource (r);
}

extern const char * const se_names[];

/*
这个函数是 process_http中  的   DepFunc 函数，当完整的获取到一个SE对象数据的时候，此时调用该函数对该对象作定制化处理*/
void test_dep (Stub *r) {
  int res_type = resource_type (r);
  //LOG_I("test_dep : resource type:%d(%s),href:%s\n",res_type,se_names[res_type],r->base.name);
  switch (res_type) {
  case SE_Time:
    LOG_I("  test_dep : SE_Time , call time_sync\n");
    time_sync (r);  //时间同步？
    break;
  case SE_DERProgram:
    if (!primary){ 
      LOG_I("  test_dep : SE_DERProgram , call der_program\n");
      der_program (r);  //获取 DERProgram 下面的三个主要资源。在primary指令情况下，不会执行到这里。在primary指令下，通过EndDevice路径来执行。
    }
    break;
  case SE_DERProgramList:
    LOG_I("  test_dep : SE_DERProgramList , call poll_derpl\n");
    poll_derpl (r);
    break;
  case SE_FunctionSetAssignments:
    if (!primary) {
      LOG_I("  test_dep : SE_FunctionSetAssignments , call fsa\n");
      fsa (r);  //获取FSA下面的部分资源。但是只有在primary为0的时候才执行？？为什么
    }
    break;
  case SE_DeviceCapability:
    LOG_I("  test_dep : SE_DeviceCapability , call dcap\n");
    dcap (r);
    break;
  case SE_EndDevice:
    LOG_I("  test_dep : SE_EndDevice , call end_device\n");
    end_device (r);
    break;
  case SE_Registration:
    LOG_I("  test_dep : SE_Registration , call check_registration\n");
    check_registration (r);
    break;
  case SE_MirrorUsagePoint:
    LOG_I("  test_dep : SE_MirrorUsagePoint , call update_mup\n");
    update_mup (r);
    break;
  case SE_SelfDevice:
    LOG_I("  test_dep : SE_SelfDevice , call self_device\n");
    self_device (r);
  }
  //LOG_D("test_dep : exit function\n");
}


//client_test。这个是一个测试程序，用于展示这套代码提供的API在通过SunSpec 测试程序中的顺应性测试。

int main (int argc, char **argv) {
  Service *s;
  void *any;  //这个any是一个void类型的指针，即意思为任意类型的数据。这些数据类型的确定，由当时新增该事件的时候的type所决定。所以此时已知晓type之后，就可以正常处理了。
  version ();
  platform_init ();
  
  LOG_I("client test started\n");

  options (argc, argv);

  while (1) {
    switch (der_poll (&any, -1)) {  //这个函数的执行结果是由内部的 client_poll 提供的
    case SERVICE_FOUND: //主要针对DNSSD的
      LOG_I("in branch SERVICE_FOUND\n");
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
