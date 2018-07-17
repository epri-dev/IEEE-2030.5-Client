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

void print_event_start (EventBlock *eb) { Resource *r = eb->event;
  DerDevice *device = eb->context;
  SE_Event_t *ev = r->data; SE_DERControl_t *derc;
  printf ("Event Start \"%s\" -- %s", ev->description, timestamp ());
  printf ("EndDevice: %ld\n", device->sfdi);
  switch (r->type) {
  case SE_DERControl: derc = r->data;
    print_se_object (&derc->DERControlBase, SE_DERControlBase);
  } printf ("\n");
}

void print_event_end (EventBlock *eb) { Resource *r = eb->event;
  DerDevice *device = eb->context; SE_Event_t *ev = r->data;
  printf ("Event End \"%s\" -- %s", ev->description, timestamp ());
  printf ("EndDevice: %ld\n\n", device->sfdi);
}

void print_default_control (DerDevice *device) {
  SE_DefaultDERControl_t *dc = device->dderc;
  printf ("Default Control \"%s\" -- %s", dc->description, timestamp ());
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
  Schedule *s = &d->schedule; EventBlock *eb = s->scheduled;
  printf ("Event Schedule for device %ld -- %s", d->sfdi, timestamp ());
  printf ("  Start       End         Description\n");
  print_blocks (s->scheduled);
  printf ("Active Blocks:\n");
  print_blocks (s->active); printf ("\n");
}

int der_poll (void **any, int timeout) {
  Schedule *s; int event;
  while (event = next_event (any)) {
    switch (event) {
    case SCHEDULE_UPDATE: s = *any; update_schedule (s);
      if (!s->active) { DerDevice *d = s->context;
	if (d->dderc) insert_event (d, DEFAULT_CONTROL, 0);
      } break;
    case RESOURCE_POLL: poll_resource (*any);
    case RESOURCE_UPDATE: update_resource (*any); break;
    case RESOURCE_REMOVE:
      if (se_event (resource_type (*any)))
	delete_blocks (*any);
    case RETRIEVE_FAIL:
      remove_stub (*any); break;
    default: return event;
    }
  }
  return client_poll (any, timeout);
}

void der_init () {
  device_init (); resource_init (); event_init ();
}
