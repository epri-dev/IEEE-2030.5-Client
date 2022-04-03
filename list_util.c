// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup list_util List Utility

    Provides utility functions that deal with lists.
    @ingroup list
    @{
*/

/** @brief Duplicate a List (shallow copy).
    @param list is a pointer to a linked List
    @returns a copy of the list argument
*/
List *list_dup (List *list);

/** @brief Concatenate two lists. 
    @param la is a pointer to a linked list
    @param lb is a pointer to a linked list
    @returns the list la + lb
*/
void *list_cat (void *la, void *lb);

/** @brief Reverse a list.
    @param list is a linked list
    @returns the reveresed list
*/
void *list_reverse (void *list);

/** @brief Partition a list into two seperate lists based upon a pivot item.
    @param lb points to a value that receives a pointer to a list
    @param list is a pointer to a list to be partitioned
    @param x is pivot item used to partition the list
    @param compare is a pointer to a function that compares list items
    @returns pointers to two seperate lists (return value, *lb)
*/
void *partition (void *b, void *list, void *x,
		 int (*compare) (void *a, void *b));

/** @brief Sort a list using quicksort.
    @param list is a pointer to a linked list
    @param compare is a pointer to a function that compares list items
    @returns a pointer to the sorted list
*/
void *quick_sort (void *list, int (*compare) (void *a, void *b));

#ifndef HEADER_ONLY

List *list_dup (List *list) {
  List *l, *prev = NULL, *head = NULL;
  foreach (l, list) {
    List *n = type_alloc (List);
    n->data = l->data;
    if (prev) prev->next = n;
    else head = prev = n;
  } return head;
}

void *list_cat (void *la, void *lb) {
  if (la) { List *prev, *n;
    foreach (n, la) prev = n;
    prev->next = lb; return la;
  } return lb;
}

//看起来像是将一个list中的各个元素的next方向互换，具体还不是很清楚。
void *list_reverse (void *list) {
  List *l = list, *rev = NULL;
  while (l != NULL) {
    List *n = l->next;  //将l->next的值暂时存到List *n中
    l->next = rev;  //修改l->next的值，指向rev，第一次是NULL。
    rev = l; l = n; //rev被赋予了l的值。l被赋予了n的值，也就是之前l->next的值。n是一个中间变量，此时l本身的值与l->next的值互换了。
  } return rev;
}

void *partition (void *lb, void *list, void *x,
		 int (*compare) (void *a, void *b)) {
  List *a = NULL, **b = lb, *y = list, *next;
  while (y) { next = y->next;
    if (compare (x, y) < 0) {
      y->next = *b; *b = y;
    } else {
      y->next = a; a = y;
    } y = next;
  } return a;
}

void *quick_sort (void *list, int (*compare) (void *a, void *b)) {
  List *x = list, *a = NULL, *b = NULL;
  if (list_length (list) <= 1) return list;
  a = partition (&b, x->next, x, compare);
  a = quick_sort (a, compare);
  b = quick_sort (b, compare);
  x->next = b; return list_cat (a, x);
}

#endif
