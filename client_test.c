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
  print_interfaces (0); exit (0);
}

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

int server = 0, test = 0, secure = 0, interval = 5*60, primary = 0, pin = 0;
Stub *edevs; char *path = NULL; uint64_t delete_sfdi;

int subtype_query (char *arg, char *name) {
  char subtype[12] = {0}; int qu = 0, n;
  char *p = strchr (arg, ':'), *q = strchr (arg, '/');
  if (p) {
    if (*(p+1) == '1') qu = 1;
    else return 0; n = p-arg;
  } else if (q) n = q-arg;
  else n = strlen (arg);
  if (n > 11) return 0;
  strncpy (subtype, arg, n);
  if (server = se_subquery (subtype)) {
    if (!secure) client_init (name, NULL);
    se_discover (server, qu);
    path = q; return 1;
  } return 0;
}

int uri_retrieval (char *arg) {
  Uri128 buf = {0}; Uri *uri = &buf.uri;
  void *conn; Query q = {0};
  if (!uri_parse (&buf, arg, 127) || !uri->host) {
    if (uri->scheme) {
      printf ("error parsing URI %s\n", arg); exit (0);
    } return 0;
  }
  conn = se_connect_uri (uri);
  if (uri->query && !parse_query (&q, uri->query)) {
    printf ("error parsing URI query \"%s\"\n", uri->query); exit (0);
  }
  get_resource (conn, -1, uri->path, q.limit);
  return 1;
}

int cert_name (char *arg) {
  switch (file_type (arg)) {
  case FILE_DIR: load_cert_dir (arg); break;
  case FILE_REGULAR: load_cert (arg); break;
  default: return 0;
  } return 1;
}

void options (int argc, char **argv) {
  int i = 2, index; char *name = argv[1];
  if (argc < 3) usage ();
  if ((index = interface_index (name)) < 0) {
    printf ("options: interface %s not found\n", name); exit (0);
  }
  net_select (index); der_init ();
  // process certificates and base query
  while (i < argc) { DerDevice *d;
    if (subtype_query (argv[i], name)
	|| uri_retrieval (argv[i])) {
      i++; break;
    } else if (!secure) {
      client_init (name, argv[i]); secure = 1;
      d = get_device (device_sfdi);
      memcpy (d->lfdi, device_lfdi, 20);
    } else if (!cert_name (argv[i])) {
      printf ("options: certificate file or directory %s does not exist\n",
	      argv[i]); exit (0);
    } i++;
  }
  if (!secure) {
    printf ("options: warning, no device certificate specified, "
	    "TLS library will be uninitialized\n");
  } else if (i < 5) {
    printf ("options: no CA certificates specified, using ./certs\n");
    load_cert_dir ("certs");
  }
  // process tests
  while (i < argc) { uint64_t sfdi;
    const char * const commands[] =
      {"sfdi", "edev", "fsa", "register", "pin", "primary", "all", "time",
       "self", "subscribe", "metering", "meter", "alarm", "poll", "load",
       "device", "delete", "inverter"};
    switch (string_index (argv[i], commands, 18)) {
    case 0: // sfdi
      if (++i == argc || !number64 (&device_sfdi, argv[i])) {
	printf ("sfdi command expects number argument\n"); exit (0);
      } break;
    case 1: // edev
      test |= GET_EDEV | GET_FSA; primary = 1; break;
    case 2: // fsa
      test |= (GET_ALL^GET_DERC) | REGISTER_TEST; break;
    case 3: // register
      test |= GET_EDEV | REGISTER_TEST | PUT_SETTINGS;
      device_settings (device_sfdi, "settings"); break;
    case 4: // pin
      test |= PUT_PIN;
      if (++i == argc || !number (&pin, argv[i])) {
	printf ("pin command expects number argument\n"); exit (0);
      } break;
    case 5: // primary
      primary = 1;
    case 6: // all
      test |= GET_ALL | REGISTER_TEST | SCHEDULE_TEST | PUT_SETTINGS;
      device_settings (device_sfdi, "settings"); break;
    case 7: // time
      test |= GET_TIME; break;
    case 8: // self
      test |= GET_SELF; break;
    case 9: // subscribe
      test |= GET_EDEV | GET_FSA | FSA_SUBSCRIBE; break;
    case 10: // metering
      test |= GET_ALL | REGISTER_TEST | INCLUDE_READINGS | PUT_SETTINGS;
      device_settings (device_sfdi, "settings");
    case 11: // meter
      test |= GET_EDEV | METER_TEST; break;
    case 12: // alarm
      test |= GET_ALL | REGISTER_TEST | ALARM_TEST; break;
    case 13: // poll
      if (++i == argc || !number (&active_poll_rate, argv[i])) {
	printf ("poll command expects an interval in seconds\n"); exit (0);
      } break;
    case 14: // load
      if (++i == argc || !number64 (&sfdi, argv[i]) || ++i == argc) {
	printf ("load command expects SFDI and directory name\n"); exit (0);
      } device_settings (sfdi, argv[i]); break;
    case 15: // device
      if (++i == argc || file_type (argv[i]) != FILE_DIR) {
	printf ("device command expects a directory name\n"); exit (0);
      } device_certs (argv[i]);
      test |= GET_EDEV | GET_FSA | DEVICE_TEST; break;
    case 16: // delete
      if (++i == argc || !number64 (&delete_sfdi, argv[i])) {
	printf ("delete command expects number argument\n"); exit (0);
      } test |= GET_EDEV | DELETE_DEVICE; break;
    case 17: // inverter
      test |= INVERTER_CLIENT; break;
    default:
      printf ("unknown command \"%s\"\n", argv[i]); exit (0);
    }
    i++;
  }
}

void test_fail (char *test, char *description) {
  printf ("%s failed: %s\n", test, description); exit (0);
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

void put_der_settings (void *conn, Settings *ds, SE_DER_t *der) {
  link_put (conn, der, ds->dera, DERAvailability);
  link_put (conn, der, ds->dercap, DERCapability);
  link_put (conn, der, ds->derg, DERSettings);
  link_put (conn, der, ds->ders, DERStatus);
}

void edev_complete (Stub *r) { Stub *s, *t;
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
      s->poll_rate = se_exists (fsal, pollRate)? fsal->pollRate : 900;
      poll_resource (s);
    }
    if (test & SCHEDULE_TEST) {
      schedule_der (r);
      r->completion = schedule_der;
    }
  }
  if (edev->sFDI == device_sfdi) {
    if (test & METER_TEST) { List *rds = NULL;
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

void der_program (Stub *d) {
  if (test & GET_DERC) { Stub *cl;
    SE_DERProgram_t *dp = resource_data (d);
    get_dep (d, dp, DefaultDERControl);
    if (cl = get_list_dep (d, dp, DERControlList)) {
      cl->poll_rate = active_poll_rate;
      poll_resource (cl);
    }
    get_list_dep (d, dp, DERCurveList);
  }
}

void poll_derpl (Stub *r) {
  SE_DERProgramList_t *derpl = resource_data (r);
  r->poll_rate = se_exists (derpl, pollRate)? derpl->pollRate : 900;
  poll_resource (r);
}

// select the highest priority DERProgram from a DERProgramList
void der_program_list (Stub *r) {
  if (primary && r->reqs) der_program (r->reqs->data);
}

int fsa (Stub *r) {
  if (test & GET_DERP) {
    SE_FunctionSetAssignments_t *fsa = resource_data (r);
    Stub *d = get_list_dep (r, fsa, DERProgramList);
    if (d && primary) { d->completion = der_program_list; return 1; }
  } return 0;
}

// select the highest priority FSA that has a DERProgramList
void fsa_list (Stub *r) { List *l; 
  foreach (l, r->reqs) if (fsa (l->data)) break;
}

void get_edev_subs (Stub *edevs) { List *l;
  if (test & REGISTER_TEST) return; // wait for registration
  foreach (l, edevs->reqs) {
    Stub *s = l->data;
    SE_EndDevice_t *e = resource_data (s);
    if (test & INVERTER_CLIENT && e->sFDI != device_sfdi) continue;
    s->completion = edev_complete;
    get_list_dep (s, e, DERList);
    if (test & GET_FSA) {
      if ((s = get_list_dep (s, e, FunctionSetAssignmentsList)) && primary)
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
    delete_stub (r); test &= ~DELETE_DEVICE;
  }
  if (e->sFDI == device_sfdi) {
    test |= CLIENT_FOUND;
    if (device_sfdi == delete_sfdi) return;
    if (test & REGISTER_TEST) {
      if (se_exists (e, RegistrationLink)) {
	if (test & PUT_PIN) { SE_Registration_t reg = {0};
	  reg.dateTimeRegistered = se_time ();
	  reg.pIN = pin;
	  se_put (r->conn, &reg, SE_Registration, e->RegistrationLink.href);
	  test &= ~REGISTER_TEST;
	  if (edevs) get_edev_subs (edevs);
	} else get_root (r->conn, e, Registration);
      } else test_fail ("registration",
			"EndDevice does not contain RegistrationLink.");
    }
  }
}

void edev_list (Stub *r) { edevs = r;
  if (test & DELETE_DEVICE) {
    test_fail ("delete device", "client EndDevice instance not found");
  }
  if (!(test & CLIENT_FOUND)) {
    if (test & GET_EDEV) {
      SE_EndDevice_t edev = {0};
      edev.sFDI = device_sfdi;
      se_post (r->conn, &edev, SE_EndDevice, r->base.name);
    }
    if (test & DEVICE_TEST)
      test_fail ("device test", "client EndDevice instance not found");
  } else if (device_sfdi != delete_sfdi) {
    if (test & DEVICE_TEST) { List *l;
      foreach (l, r->reqs) { DerDevice *d;
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
  } r->completion = NULL;
}

void dcap (Stub *r) {
  SE_DeviceCapability_t *dcap = resource_data (r);
  if (test & GET_TIME) {
    if (!get_root (r->conn, dcap, Time)) 
      test_fail ("time", "no TimeLink in DeviceCapability");
  }
  if (test & GET_EDEV) {
    if (r = get_list_root (r->conn, dcap, EndDeviceList))
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

void time_sync (Stub *s) {
  SE_Time_t *tm = resource_data (s);
  s->poll_rate = se_exists (tm, pollRate)? tm->pollRate : 900;
  poll_resource (s); set_time (tm);
}

void update_mup (Stub *s) {
  SE_MirrorUsagePoint_t *mup = resource_data (s);
  uint64_t sfdi = sfdi_gen (mup->deviceLFDI);
  DerDevice *d = find_device (&sfdi);
  if (d && d->readings) { d->mup = s;
    d->metering_rate = se_exists (mup, postRate)? mup->postRate : 5 * 60;
    if (!(test & INCLUDE_READINGS)) { List *r;
      foreach (r, d->readings)
	se_post (d->mup->conn, r->data, SE_MirrorMeterReading,
		 resource_name (d->mup));
    }
    insert_event (d, DEVICE_METERING, 0);
  }
}

void post_readings (DerDevice *d) { List *r;
  int64_t now = se_time (), next = now + d->metering_rate;
  printf ("post_readings\n");
  foreach (r, d->readings) {
    SE_MirrorMeterReading_t *mmr = r->data;
    SE_Reading_t *rd = &mmr->Reading;
    se_clear (mmr, ReadingType); se_set (mmr, Reading);
    if (mmr->nextUpdateTime) {
      se_set (mmr, lastUpdateTime);
      mmr->lastUpdateTime = mmr->nextUpdateTime;
    } else se_set (mmr, nextUpdateTime);
    mmr->nextUpdateTime = next;
    rd->_flags = SE_value_exists | SE_timePeriod_exists;
    rd->timePeriod.start = now;
    switch (mmr->ReadingType.uom) {
    case 5: rd->value = 20; break; // Amps
    case 29: rd->value = 230; break; // Volts
    case 33: rd->value = 60; break; // Hz
    case 38: case 61: case 63:
      rd->value = 5000;
    }
    se_post (d->mup->conn, mmr, SE_MirrorMeterReading,
	     resource_name (d->mup));
  }
  insert_event (d, DEVICE_METERING, next);
}

void log_event (Stub *log) { List *l;
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

void test_dep (Stub *r) {
  switch (resource_type (r)) {
  case SE_Time: time_sync (r); break;
  case SE_DERProgram: if (!primary) der_program (r); break;
  case SE_DERProgramList: poll_derpl (r); break;
  case SE_FunctionSetAssignments: if (!primary) fsa (r); break;
  case SE_DeviceCapability: dcap (r); break;
  case SE_EndDevice: end_device (r); break;
  case SE_Registration: check_registration (r); break;
  case SE_MirrorUsagePoint: update_mup (r); break;
  case SE_SelfDevice: self_device (r);
  }
}

int main (int argc, char **argv) {
  Service *s; void *any;
  version ();
  platform_init ();
  options (argc, argv);
  while (1) {
    switch (der_poll (&any, -1)) {
    case SERVICE_FOUND: s = any;
      print_service (s);
      if (test) get_dcap (s, secure);
      else if (path)
	get_resource (service_connect (s, secure), -1, path, 0);
      else get_path (s, secure); break;
    case TCP_ACCEPT:
      // accept_notifier (any);
    case TCP_PORT:
      if (conn_session (any)) {
	http_debug (any, 1);
	// if (http_client (any))
	process_http (any, test_dep);
	// else process_notifications (any, test_dep);
      }
      break;
    case TCP_TIMEOUT:
      cleanup_http (any);
      printf ("Connection timed out\n"); return 0;
    case TCP_CLOSED:
      cleanup_http (any);
      printf ("Connection closed\n"); return 0;
    case DEVICE_SCHEDULE:
      print_event_schedule (any); break;
    case EVENT_START:
      print_event_start (any); break;
    case EVENT_END:
      print_event_end (any); break;
    case DEFAULT_CONTROL:
      print_default_control (any); break;
    case DEVICE_METERING:
      post_readings (any); break;
    } 
  }
}
