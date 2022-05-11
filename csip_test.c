// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include "der_client.c"
//get dcap resource. url based on discovered server I assume
void dcap (Stub *r) {
  SE_DeviceCapability_t *dcap = resource_data (r);
  if (!se_exists (dcap, EndDeviceListLink)) return;
  get_root (r->conn, dcap, Time);
  get_list_root (r->conn, dcap, EndDeviceList);
  get_list_root (r->conn, dcap, MirrorUsagePointList);
}
/*
edev assign completion routine and setting up a polling interval 
for the FunctionSetAssignmentsList subroutine resource
*/
void edev (Stub *d) { Stub *r;
  SE_EndDevice_t *edev = resource_data (d);
  d->completion = schedule_der;
  get_list_dep (d, edev, DERList);
  /*retrieve FSAList subordinate resource, and set up polling for the resource*/
  if (r = get_list_dep (d, edev, FunctionSetAssignmentsList)) {
    r->poll_rate = 10; poll_resource (r);
  }
}
//retrieves DefaultDERControl, DERControlList, and DERCurveList if the DERProgram contains links to them
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
//stub is the local representation of a resource. csip_dep takes a pointer to a stub
void csip_dep (Stub *r) {
  switch (resource_type (r)) {
  case SE_Time: set_time (resource_data (r)); break;
  case SE_DERProgram: der_program (r); break;
  case SE_DeviceCapability: dcap (r); break;
  case SE_EndDevice: edev (r); break;
  case SE_FunctionSetAssignments: fsa (r);
  }
}

int main (int argc, char **argv) {
  void *any; int index; Service *s;
  // platform intialization
  platform_init ();
  // process command line arguments
  if (argc < 2) {
    print_interfaces (0); exit (0);
  }
  if ((index = interface_index (argv[1])) < 0) {
    printf ("interface %s not found\n", argv[1]); exit (0); 
  }
  // load certificates and initialize client
  client_init (argv[1], "pti_dev.x509"); der_init ();
  load_cert_dir ("certs");
  // perform service discovery aka server discovery via DNS-SD method
  discover_device ();
  while (1) {
    // process events
    switch (der_poll (&any, -1)) {
    case SERVICE_FOUND: s = any;
      print_service (s); get_dcap (s, 1); break;
    case TCP_PORT:
      if (conn_session (any))
	process_http (any, csip_dep); break;
    case DEVICE_SCHEDULE:
      print_event_schedule (any); break;
    case EVENT_START:
      //print when DERControl start
      print_event_start (any); break;
    case EVENT_END:
      //print when DERControl ended
      print_event_end (any); break;
    }
  }
}
