// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup list List

    Provides a linked List structure with associated functions.
    @{
*/

/** @brief Linked List structure */
typedef struct _List {
  struct _List *next; ///< is a pointer to the next List item
  void *data;         ///< is a pointer to a data element
} List;

#define list_next(l) (((List *)l)->next)
#define list_data(l) (((List *)l)->data)

#define link_insert(head, l) \
  ((list_next (l) = (void *)head), (head = (void *)l))
#define foreach(l, head) \
  for (l = (void *)head; l != NULL; l = (void *)list_next (l))

/** @brief Free a linked List.

    Frees the linked structures only, not the data elements.
    @param list is a pointer to a List.
*/
void free_list (void *list);

/** @brief Insert data element at the head of a list.

    Allocates a List item and assigns the data element to the data field. Makes
    the allocated item the new head of the linked list.
    @param list is a pointer to a List
    @param data is a pointer to a data element
    @returns the new head of the list
*/
List *list_insert (List *list, void *data);

/** @brief Return the length of a list.

    Returns the length of any linked structure (e.g List).
    @param list is a pointer to a linked list
    @returns the length of the list
*/
int list_length (void *list);

/** @brief Return a list item with the matching data element.
    @param list is a pointer to a List
    @param data is a pointer to a data element
    @returns a pointer to the List item
*/
void *find_by_data (List *list, void *data);

/** @brief Insert unique data element into a list.

    Insert a data element such that only a single List item in the list
    points to the data element.
    @param list is a pointer to a List
    @param data is the data element
    @returns the modified or unmodified list
*/
List *insert_unique (List *list, void *data);

/** @brief Delete the List object that contains the data element.
    @param list is a pointer to a List
    @param data is a pointer to a data element
    @returns the head of the list with the object removed
*/
List *list_delete (List *list, void *data);

/** @brief Subtract one list from another.

    Removes the items of a list (la) that contain data elements present in
    another list (lb).
    @param la is a pointer to a linked List.
    @param lb is a pointer to a linked List.
    @returns the difference beteween lists, la - lb
*/
List *list_subtract (List *la, List *lb);

/** @brief Remove a List item from a List.

    @param list is a pointer to a linked List.
    @param link points to a item within the linked List
    @returns the list with node removed
*/
void *list_remove (void *list, void *link);

/** @brief Insert a linked item into a sorted list.
    @param list is a pointer to a linked list
    @param item is a pointer to a linked item
    @param compare is a pointer to a comparison function that compares two
    linked items
    @returns a sorted list with the item inserted.
*/
void *insert_sorted (void *list, void *item,
		     int (*compare) (void *a, void *b));

/** @} */

#ifndef HEADER_ONLY

#include <stdlib.h>
#include <string.h>

void free_list (void *list) { List *l = list, *t;
  while (l) t = l, l = l->next, free (t);
}

List *list_insert (List *l, void *data) {
  List *n = malloc (sizeof (List));
  n->next = l; n->data = data;
  return n;
}

int list_length (void *l) {
  List *list = l; int length = 0;
  while (list) length++, list = list->next;
  return length;
}

void *_find_by_data (List **prev, List *l, void *data) {
  for (*prev = NULL; l; *prev = l, l = l->next) 
    if (l->data == data) return l;
  return NULL;
}

void *find_by_data (List *list, void *data) { List *prev;
  return _find_by_data (&prev, list, data);
}

List *insert_unique (List *l, void *data) { List *prev;
  if (_find_by_data (&prev, l, data)) return l;
  return list_insert (l, data);
}

List *list_delete (List *l, void *data) { List *d, *prev;
  if (d = _find_by_data (&prev, l, data)) {
    if (prev) prev->next = d->next;
    else l = d->next; free (d);
  } return l;
}

List *list_subtract (List *la, List *lb) { List *b;
  foreach (b, lb) la = list_delete (la, b->data);
  return la;
}

List *list_intersect (List *la, List *lb) {
  List *a = la, *prev = (List *)&la, *next;
  while (a) { next = a->next;
    if (!find_by_data (lb, a->data)) {
      prev->next = next; free (a);
    } a = next;
  } return la;
}

List *list_union (List *la, List *lb) { List *b;
  foreach (b, lb)
    la = insert_unique (la, b->data);
  return la;
}

void *list_remove (void *list, void *link) { 
  List *l, *prev = (List *)&list;
  foreach (l, list) {
    if (l == link) {
      prev->next = l->next; break;
    } prev = l;
  } return list;
}

void *_insert_sorted (void *list, void *item, List **prev,
		      int (*compare) (void *a, void *b)) {
  List *l = list, *n = item;
  while (l && (compare (n, l) > 0))
    *prev = l, l = l->next;
  if (*prev) (*prev)->next = n; else list = n;
  n->next = l; return list;
}

void *insert_sorted (void *list, void *item,
		    int (*compare) (void *a, void *b)) {
  List *prev = NULL;
  return _insert_sorted (list, item, &prev, compare);
}

#endif
