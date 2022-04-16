// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include "der_client.c"


void dcap (Stub *r) {
  SE_DeviceCapability_t *dcap = resource_data (r);
  if (!se_exists (dcap, EndDeviceListLink)) return;
  get_root (r->conn, dcap, Time); //获取到 dcap 中的 Time 资源
  get_list_root (r->conn, dcap, EndDeviceList); //获取到 dcap 中的 EndDeviceList 资源。
  get_list_root (r->conn, dcap, MirrorUsagePointList);  //获取到 dcap 中的 MirrorUsagePointList
}

void edev (Stub *d) {
  Stub *r;
  SE_EndDevice_t *edev = resource_data (d);
  d->completion = schedule_der; //a user defined completion routine
  get_list_dep (d, edev, DERList);
  if (r = get_list_dep (d, edev, FunctionSetAssignmentsList)) {
    r->poll_rate = 10;
    poll_resource (r);
  }
}

void der_program (Stub *d) {
  SE_DERProgram_t *dp = resource_data (d);
  get_dep (d, dp, DefaultDERControl);
  get_list_dep (d, dp, DERControlList);
  get_list_dep (d, dp, DERCurveList);
}

void fsa (Stub *d) {
  SE_FunctionSetAssignments_t *fsa = resource_data (d);
  get_list_dep (d, fsa, DERProgramList);
}

/*
`csip_dep` takes a pointer to a `Stub` (the local representation of a resource)
and calls an appropriate function based on type of resource. 
*/
void csip_dep (Stub *r) {
  switch (resource_type (r)) {
  case SE_Time:
    set_time (resource_data (r)); //如果是一个表示时间的数据的回复，则处理该数据，即设定本地时间。
    break;
  case SE_DERProgram:
    der_program (r);  //如果是一个DERProgram的数据回复了，则（可能）填充相关的Stub。
    break;
  case SE_DeviceCapability: //
    dcap (r);
    break;
  case SE_EndDevice:
    edev (r); //执行edev相关的操作
    break;
  case SE_FunctionSetAssignments:
    fsa (r);
  }
}

/* 由shell端直接可以运行的函数
这个是这个代码中另外一个可以用于验证的程序。
另外一个是client_test.c中的main
详见der_clinet.md说明内容。
*/
int main (int argc, char **argv) {
  void *any;
  int index;
  Service *s;
  platform_init ();
  if (argc < 2) {
    /*为什么这里仅仅打印IPV6的地址？？*/
    print_interfaces (0);
    exit (0);
  }

  if ((index = interface_index (argv[1])) < 0) {
    printf ("interface %s not found\n", argv[1]);
    exit (0);
  }

  client_init (argv[1], "pti_dev.x509");
  der_init ();
  load_cert_dir ("certs");
  discover_device ();
  while (1) {
    switch (der_poll (&any, -1)) {
    case SERVICE_FOUND:
      s = any;
      print_service (s);
      get_dcap (s, 1);  //通过DNSSD服务发现了一个服务，然后去获取它
      break;
    case TCP_PORT:  //异步处理HTTP的回复。
      if (conn_session (any))
        process_http (any, csip_dep); //跟服务器数据相关的一些操作，主要是在这里进行的。
      break;
    case DEVICE_SCHEDULE:
      print_event_schedule (any);
      break;
    case EVENT_START:
      print_event_start (any);  //打印一个事件现在开始执行了。
      break;
    case EVENT_END:
      print_event_end (any);    //打印一个事件现在已经结束了。
      break;
    }
  }
}
