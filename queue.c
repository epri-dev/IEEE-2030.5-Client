// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup queue Queue
    @{
*/

/** @brief Queue structure */
typedef struct {
  List *first; ///< is the first item in the queue
  List *last; ///< ist the last item in the queue
} Queue;

/** @brief Return the head of the queue without removing it.
    @param queue is a pointer to a Queue
    @return the head of the queue
 */
#define queue_peek(queue) (void *)(queue)->first

/** @brief Test if the queue is empty.
    @param queue is a pointer to a Queue
    @return true if the queue is not empty
 */
#define queue_empty(queue) ((queue)->first == NULL)

/** @brief Return the tail of the queue
    @param queue is a pointer to a Queue
    @return the tail of the queue
 */
#define queue_tail(queue) (void *)(queue)->last

/** @brief Clear the queue
    @param queue is a pointer to a Queue
 */
#define queue_clear(queue) (queue)->first = (queue)->last = NULL

/** @brief Insert linked item at the tail of the queue.
    @param queue is a pointer to a Queue
    @param item is a pointer to a linked item
 */
void queue_add (Queue *queue, void *item);

/** @brief Remove linked item from the head of the queue.
    @param queue is a pointer to a Queue
    @return pointer to a linked item
 */
void *queue_remove (Queue *queue);

/** @brief Free queued items.
    @param queue is a pointer to a Queue
 */
void queue_free (Queue *queue);

/** @} */

#ifndef HEADER_ONLY

void queue_add (Queue *q, void *item) {
  if (q->last) q->last = q->last->next = item;
  else q->first = q->last = item;
}

void _queue_insert (Queue *q, void *item, void *prev,
		    int (*compare) (void *a, void *b)) {
  q->first = _insert_sorted (q->first, item, prev, compare);
  if (q->last) {
    if (q->last->next) q->last = q->last->next;
  } else q->last = q->first;
}

void queue_insert (Queue *q, void *item,
		    int (*compare) (void *a, void *b)) {
  List *prev = NULL;
  _queue_insert (q, item, &prev, compare);
}

void *queue_remove (Queue *q) {
  void *item = q->first; 
  if (item) {
    if (q->first == q->last) queue_clear (q);
    else q->first = q->first->next;
  } return item;
}

void queue_free (Queue *q) {
  free_list (q->first); queue_clear (q);
}

#endif
    
