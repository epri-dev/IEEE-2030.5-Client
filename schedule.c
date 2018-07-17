// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup schedule Schedule

    The schedule module provides methods for IEEE 2030.5 event scheduling.
    Scheduling is done per function set such as Pricing, Distributed Energy
    Resources (DER), or Demand Response / Load Control (DRLC).

    The events of each function set are independent and each EndDevice may
    utilitize one or more function sets, therefore an EndDevice could have one
    or more independent event schedules. 
    @{
*/

#define SCHEDULE_UPDATE (EVENT_NEW+10)

enum EventStatus {Scheduled, Active, Canceled, CanceledRandom,
		  Superseded, Aborted, Completed, ActiveWait,
		  ScheduleSuperseded};

/** @brief An EventBlock is an Event instance scheduled for a particular
    EndDevice.

    The start and end times are effective times with the event
    randomization already applied, primacy is derived the context that the event
    occured.
*/
typedef struct _EventBlock {
  struct _EventBlock *next; ///< is a pointer to the next EventBlock
  void *event; ///< is a pointer to the Event Stub
  void *program; ///< is a pointer to the DERProgram for DERControls
  void *context; //< is a pointer to device specific information
  uint8_t status; ///< the schedule event status
  uint8_t primacy;  ///< associated event primacy
  uint32_t der; ///< bitmask of active DER controls
  int64_t start; ///< the effective start time
  int64_t end; ///< the effective end time
} EventBlock;

/** @brief A Schedule organizes the events of a particular function set for an
    EndDevice in terms of the rules in section 12.1.3 Event Rules and Guidlines.
    
    Events are assigned to an EventBlock which can belong to one of three
    queues, `scheduled`, `active`, and `superseded`. EventBlocks that
    correspond with server events that have been marked Canceled or Superseded
    belong to none of these queues and are eventually removed from the schedule
    when the events expire.

    Initially all EventBlocks are inserted into the `scheduled` queue using the
    rules for overlapping and nested events. Future events that are superseded
    by scheduling are marked `ScheduleSuperseded` and are placed in the
    superseded queue. If the set of events changes for some reason, these
    events may eventually rejoin the `scheduled` queue.

    When EventBlocks in the scheduled queue become active (i.e when the current
    time matches the effective start time) they are transfered to the `active`
    queue, in the process they may be superseded by existing active EventBlocks
    or they may supersede other active EventBlocks. In these cases the
    superseded EventBlocks are removed from the schedule and a Response is sent
    to the server with the response status EventSuperseded.

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
  int64_t next;
  Stub *device; ///< EndDevice that is the subject of scheduling
  void *context; //< is a pointer to device specific information
  HashTable *blocks; /**< HashTable of EventBlocks that belong to the schedule
			using the Event mRID as the hash key. */
  EventBlock *scheduled; ///< EventBlock queue sorted by effective start time
  EventBlock *active; ///< EventBlock queue sorted by effective end time
  EventBlock *superseded; ///< EventBlock queue sorted by effective start time
} Schedule;

/** @brief Send an event response to the server on the behalf of a device.
    @param device is a pointer to an EndDevice Stub
    @param event is a pointer to an Event Stub
    @param status is the status of the event response (@ref ResponseType)
*/
void device_response (Stub *device, Stub *event, int status);

/** @brief Add an event to a schedule with an associated primacy.
    @param s is a pointer to a Schedule
    @param event is a pointer to an Event
    @param primacy is the associated primacy for the event
    @returns a pointer to an EventBlock belonging to the schedule
*/
EventBlock *schedule_event (Schedule *s, Stub *event, int primacy);

/** @brief Update the schedules associated with an event.
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

#define randomizable(type)				  \
  (type == SE_DERControl || type == SE_TimeTariffInterval \
   || type == SE_EndDeviceControl)

int rand_bound (int bound) {
  if (bound) {
    int sign = bound < 0? -1 : 1, upper, result;
    bound *= sign; upper = RAND_MAX - RAND_MAX % bound;
    do result = rand (); while (result > upper);
    return sign * (result % (bound + 1));
  } return 0;
}

int randomize_start (void *event) {
  SE_RandomizableEvent_t *ev = resource_data (event);
  return rand_bound (ev->randomizeStart);
}

int randomize_duration (void *event) {
  SE_RandomizableEvent_t *ev = resource_data (event);
  return rand_bound (ev->randomizeDuration);
}

void device_response (Stub *device, Stub *event, int status) {
  SE_EndDevice_t *edev = resource_data (device);
  SE_Event_t *ev = resource_data (event);
  SE_DERControlResponse_t resp;
  if ((status == EventReceived && ev->responseRequired & 1)
      || (ev->responseRequired & 2)) {
    se_response (&resp, ev, edev->lFDI, status);
    se_post (event->conn, &resp, SE_DERControlResponse, ev->replyTo);
  }
}

uint32_t der_base (void *event) {
  SE_DERControl_t *derc = resource_data (event);
  return se_flags (&derc->DERControlBase);
}

int block_supersede (EventBlock *a, EventBlock *b) {
  SE_Event_t *x = resource_data (a->event),
    *y = resource_data (b->event);
  if ((a->primacy == b->primacy
       && x->creationTime > y->creationTime)
      || a->primacy < b->primacy)
    return a->der? (b->der &= ~a->der) == 0 : 1;
  a->der &= ~b->der; return 0;
}

EventBlock *new_block (Stub *event, int primacy) {
  EventBlock *eb = type_alloc (EventBlock);
  SE_Event_t *ev = resource_data (event);
  int rand = randomizable (resource_type (event));
  eb->next = NULL;
  eb->start = ev->interval.start
    + (rand? randomize_start (event) : 0);
  eb->end = eb->start + ev->interval.duration
    + (rand? randomize_duration (event) : 0);
  eb->primacy = primacy;
  eb->status = 0;
  eb->event = event;
  return eb;
}

int event_status (void *event) {
  SE_Event_t *ev = resource_data (event);
  return ev->EventStatus.currentStatus;
}

int compare_start (void *a, void *b) {
  EventBlock *x = a, *y = b;
  return x->start - y->start;
}

int compare_end (void *a, void *b) {
  EventBlock *x = a, *y = b;
  return x->end - y->end;
}

typedef struct {
  uint64_t start, end;
} Interval;

void event_interval (Interval *i, Stub *event) {
  SE_Event_t *ev = resource_data (event);
  i->start = ev->interval.start;
  i->end = i->start + ev->interval.duration;
}

void insert_block (Schedule *s, EventBlock *eb) {
  EventBlock *prev = (EventBlock *)&s->scheduled,
    *e = s->scheduled, *next; Interval x, y;
  eb->status = Scheduled;
  if (resource_type (eb->event) == SE_DERControl)
    eb->der = der_base (eb->event);
  event_interval (&x, eb->event);
  while (e) { next = e->next;
    event_interval (&y, e->event);
    if (x.start < y.end && x.end > y.start) {
      if (block_supersede (eb, e)) {
	e->status = ScheduleSuperseded;
	link_insert (s->superseded, e);
	prev->next = e = next; continue;
      } else if (!eb->der) {
	eb->status = ScheduleSuperseded;
	link_insert (s->superseded, eb); return;
      }
    } else if (x.end == y.start)
      e->start = eb->end;
    else if (x.end == y.end)
      e->end = eb->end;
    prev = e; e = next;
  }
  s->scheduled = insert_sorted (s->scheduled, eb, compare_start);
}

int active_poll_rate = 300;

void activate_block (Schedule *s, EventBlock *eb) {
  Stub *event = eb->event;
  // printf ("activate_block %s %d\n", event->base.name, eb->status);
  if (eb->status != Active) {
    eb->status = Active;
    insert_event (eb, EVENT_START, 0);
    device_response (s->device, event, EventStarted);
  }
  event->poll_rate = active_poll_rate;
  poll_resource (event);  
}

void insert_active (Schedule *s, EventBlock *eb) {
  EventBlock *prev = (EventBlock *)&s->active, *a = s->active, *next;
  printf ("insert_active\n");
  if (resource_type (eb->event) == SE_DERControl)
    eb->der = der_base (eb->event);
  while (a) { next = a->next;
    if (block_supersede (eb, a)) {
      prev->next = next;
      a->end = eb->start;
      if (a->status == Active)
	insert_event (a, EVENT_END, 0);
      a->status = Superseded;
      device_response (s->device, a->event, EventSuperseded);
    } else if (!eb->der) {
      eb->status = Superseded;
      device_response (s->device, eb->event, EventSuperseded); return;
    } a = next;
  }
  switch (event_status (eb->event)) {
  case Scheduled: update_resource (eb->event);
    eb->status = ActiveWait; break;
  case Active: activate_block (s, eb); break;
  }
  s->active = insert_sorted (s->active, eb, compare_end); 
}

EventBlock *get_block (Schedule *s, void *event) {
  SE_Event_t *ev = resource_data (event);
  return hash_get (s->blocks, ev->mRID);
}

void activate_blocks (Stub *event) { List *l;
  // printf ("activate_blocks\n");
  foreach (l, event->schedules) {
    EventBlock *eb = get_block (l->data, event);
    if (eb->status == ActiveWait)
      activate_block (l->data, eb);
  }
}

void remove_block (Schedule *s, EventBlock *eb) {
  switch (eb->status) {
  case Scheduled:
    s->scheduled = list_remove (s->scheduled, eb); break;
  case Active: eb->end = se_time ();
    remove_event (eb);
    insert_event (eb, EVENT_END, 0);
  case ActiveWait:
    s->active = list_remove (s->active, eb); break;
  case ScheduleSuperseded:
    s->superseded = list_remove (s->superseded, eb); break;
  }  
}

void remove_blocks (Stub *event, int response) { List *l;
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

void delete_blocks (Stub *event) { List *l;
  SE_Event_t *ev = resource_data (event);
  foreach (l, event->schedules) {
    Schedule *s = l->data;
    free (hash_delete (s->blocks, ev->mRID));
  } free_list (event->schedules);
  event->schedules = NULL;
}

void event_update (Stub *event) {
  int64_t now; static int64_t sync_time = 0;
  printf ("event_update\n");
  switch (event_status (event)) {
  case Scheduled:
    /* The client's clock is ahead of the server's clock, attempt to
       synchronize by setting the clock (time offset) backward, also retry
       the event retrieval in another second. */
    if (sync_time != (now = se_time ())) {
      se_time_offset--; sync_time = --now;
    } insert_event (event, RESOURCE_UPDATE, now+1);
    break;
  case Active: activate_blocks (event); break;
  case Canceled: case CanceledRandom:
    remove_blocks (event, EventCanceled); break;
  case Superseded:
    remove_blocks (event, EventSuperseded); break;
  }
}

void block_update (EventBlock *eb, int status) {
  if (eb->status == Active) {
    eb->end = se_time ();
    insert_event (eb, EVENT_END, 0);
  } eb->status = status;
}

EventBlock *schedule_event (Schedule *s, Stub *event, int primacy) {
  EventBlock *eb; SE_Event_t *ev = resource_data (event);
  int status = event_status (event);
  if (eb = hash_get (s->blocks, ev->mRID)) {
    eb->primacy = primacy;
  } else {
    event->schedules = insert_unique (event->schedules, s);
    event->completion = event_update;
    eb = new_block (event, primacy);
    hash_put (s->blocks, eb);
    device_response (s->device, event, EventReceived);
    if (eb->end <= se_time ()) {
      // specified end time is in the past
      eb->status = Completed;
      device_response (s->device, event, EventExpired);
      return eb;
    }
  }
  if (in_range (eb->status, Canceled, Completed)) return eb;
  // eb->status: Scheduled, Active, ActiveWait, ScheduleSuperseded
  switch (status) {
  case Scheduled:
    if (eb->status != ActiveWait) {
      insert_block (s, eb); break;
    }
  case Active: insert_active (s, eb); break;
  case Canceled: case CanceledRandom:
    device_response (s->device, event, EventCanceled);
    block_update (eb, status); break;
  case Superseded:
    device_response (s->device, event, EventSuperseded);
    block_update (eb, status);
  } return eb;
}

void *mrid_key (void *data) {
  EventBlock *eb = data; Stub *s = eb->event;
  SE_Event_t *e = resource_data (s);
  return e->mRID;
}

void schedule_init (Schedule *s) {
  s->blocks = new_int128_hash (64, mrid_key);
}

void update_schedule (Schedule *s) {
  EventBlock *eb = s->active, *next;
  int64_t now = se_time (), last = 0;
  while (eb) {
    if (eb->end <= now) {
      if (eb->status == Active) {
	device_response (s->device, eb->event, EventCompleted);
	insert_event (eb, EVENT_END, 0);
      } eb->status = Completed;
    } else { last = eb->end; break; }
    eb = eb->next;
  } s->active = eb;
  eb = s->scheduled;
  while (eb) { next = eb->next;
    if (eb->start <= now) {
      insert_active (s, eb);
      last = last? min (last, eb->end) : eb->end;
    } else { last = last? min (last, eb->start) : eb->start; break; }
    eb = next;
  } s->scheduled = eb;
  eb = s->superseded;
  while (eb) {
    if (eb->start <= now) {
      eb->status = Superseded;
      device_response (s->device, eb->event, EventSuperseded);
    } else { last = last? min (last, eb->start) : eb->start; break; }
    eb = eb->next;
  } s->superseded = eb;
  printf ("update_schedule %" PRId64 " %" PRId64 "\n", now, last);
  if (last) {
    if (last != s->next) {
      remove_event (s);
      insert_event (s, SCHEDULE_UPDATE, last);
    } s->next = last;
  } else s->next = 0;
}

