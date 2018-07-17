// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup event_queue Event Queue

    Provides a timed event queue.
    @{
*/

#define EVENT_TIMER (EVENT_NEW+2)
#define EVENT_START (EVENT_NEW+3)
#define EVENT_END (EVENT_NEW+4)
// #define EVENT_UPDATE (EVENT_NEW+5)

/** @brief Insert an event into the queue.
    @param data is a pointer to the event data
    @param type is the type of the event
    @param time is the time at which the event becomes active
*/
void insert_event (void *data, int type, int64_t time);

/** @brief Remove all events from the queue that match the event data.
    @brief data is a pointer to the event data
*/
void remove_event (void *data);

/** @brief Returns (and removes) the next active event from the queue if any.
    @param any is a pointer to value that receives the event data
    @returns the type of event or EVENT_NONE if there are no active events in
    the queue.
*/
int next_event (void **any);

/** @brief Create an event queue timer to wake up @ref event_poll. */
void event_init ();

/** @} */

typedef struct _Event {
  struct _Event *next;
  void *data; int type;
  int64_t time;
} Event;

Event *cur_event = NULL;
Queue im_event = {0};

int event_compare (void *a, void *b) {
  Event *x = a, *y = b; return x->time - y->time;
}

void print_events () {
  Event *e = cur_event;
  printf ("print_events %d: ", list_length (e));
  while (e) { printf ("%p %d %d, ", e->data, e->type, e->time); e = e->next; }
  printf ("\n");
}

void insert_event (void *data, int type, int64_t time) {
  Event *e = type_alloc (Event);
  e->data = data; e->type = type; e->time = time;
  if (time == 0) queue_add (&im_event, e);
  else cur_event = insert_sorted (cur_event, e, event_compare);
}

void *remove_by_data (void *list, void *data) {
  List **first = list, *l = *first, *prev = NULL, *next;
  while (l) { next = l->next;
    if (l->data == data) {
      if (prev) prev->next = next;
      else *first = next;
      free (l);
    } else prev = l;
    l = next;
  } return prev;
}

void remove_event (void *data) {
  im_event.last = remove_by_data (&im_event.first, data);
  remove_by_data (&cur_event, data);
}

Timer *ev_timer;

int next_event (void **any) {
  Event *e; int event;
  if (queue_peek (&im_event)) {
    e = queue_remove (&im_event);
    event = e->type; *any = e->data;
    free (e); return event;
  }
  if (e = cur_event) {
    int64_t now = se_time ();
    //printf ("next_event %d %d\n", e->time, now);
    if (e->time <= now) {
      event = e->type;
      cur_event = e->next;
      *any = e->data; free (e);
      return event;
    } set_timer (ev_timer, e->time - now);
  } else set_timer (ev_timer, 0);
  return EVENT_NONE;
}

void event_init () {
  ev_timer = add_timer (EVENT_TIMER);
}
