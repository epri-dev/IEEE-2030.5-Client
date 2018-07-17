// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup output Output

    The output formater takes a C object that corresponds with a schema
    element declaration and converts it into an XML or EXI document. Just like
    the parser the formater can operate in segments, outputting only the
    portion of the document that will fit within the buffer.

    Most documents within a schema can be comfortably output to a fixed length
    buffer, however documents with unbounded elements (elements that can occur
    any number of times) can easily exceed these confines. The %List derived
    documents in IEEE 2030.5 are an example of documents that contain unbounded
    elements. According to the protocol these documents support a "paged"
    access method where a document is returned with only a portion of the
    entire set of elements.

    To support this paged access method with a fixed length buffer, it is
    useful to know how many elements fit in the buffer before the formater
    ran out of space. A call to @ref output_item_count will return the number
    of elements successfully written to a buffer at the specified element level
    before @ref output_doc returned. This count can be used to determine a
    limit of items that can be reliably output to a buffer.
    @{
*/

typedef struct _Output Output;

/** @brief Return the item count at the specified element level.

    This function is useful for determining the limit when outputting unbounded
    lists of objects to a fixed length buffer. The level is the level of
    the document at which the unbounded element occurs with 0 as the level of
    the root element, 1 as the level of the child of the root and so forth.
    @param o is a pointer to the Output object
    @param level is the stack level
    @returns the count of items at the specified level
*/
int output_item_count (Output *o, int level);

/** @brief Is the output complete?
    @returns 1 if the output is complete, 0 otherwise
*/
int output_complete (Output *o);

/** @brief Output an XML or EXI document to a buffer.

    The buffer is fixed in length so this function may need to be called more
    than once to output the complete document. Use @ref output_complete to
    determine if the output is complete.
    @param o is a pointer to an Output object
    @param base is a pointer to the object to output
    @param type is the type of object to output
    @returns the length of the output (complete or partial)
    
*/
int output_doc (Output *o, void *obj, int type);

/** @} */

#ifdef HEADER_ONLY

extern Output output_global;

#else

#include <stdio.h>
#include <stdarg.h>

enum OutputState {OUTPUT_START, OUTPUT_ELEMENT, OUTPUT_ATTRIBUTE,
		  OUTPUT_ATTR_VALUE, OUTPUT_SIMPLE, OUTPUT_VALUE, OUTPUT_END,
		  OUTPUT_COMPLEX, OUTPUT_COMPLETE, OUTPUT_ERROR};

struct _OutputDriver;

typedef struct _Output {
  const Schema *schema; const SchemaElement *se;
  ElementStack stack;
  char *buffer, *ptr, *end;
  int state, indent; void *base;
  /* For the EXI driver keep track of the number of possible event codes and
     the current event code for a given context. The number of possible event
     codes determines the number of bits needed to record an event. With
     exception of the root element, the current event code starts at 0 and is
     advanced by skiping optional attributes and elements. */
  int n; // the number of possible event codes
  int code; // the current EXI event code
  int bit, flag;
  StringTable *global, *local;
  const struct _OutputDriver *driver;
  unsigned int open : 1;
  unsigned int first : 1;
} Output;

Output output_global;

enum OutputEvent {EE_EVENT, AT_EVENT, SE_SIMPLE, SE_COMPLEX};

typedef struct _OutputDriver {
  int (*output_event) (Output *, const SchemaElement *, int);
  int (*output_attr_value) (Output *, void *);
  int (*output_value) (Output *, void *);
  void (*output_done) (Output *);
} OutputDriver;

int output_string (Output *o, char *format, ...) {
  va_list args; int n, size = o->end-o->ptr;
  va_start (args, format);
  n = vsnprintf (o->ptr, size, format, args);
  va_end (args);
  if (n >= size || n < 0) { *o->ptr = '\0'; return 0; }
  o->ptr += n; return n; 
}

int flag_count (uint32_t *flags, int bit, int min, int max) {
  int diff = max - min, n = 1;
  while (diff >>= 1) n++;
  diff = (*flags >> bit) & ~(-1 << n);
  return min + diff;
}
#define flag_set(flags, bit) (*(uint32_t *)(flags) >> (bit)) & 1

// return the attribute or element count for a schema element
int output_count (void *base, const SchemaElement *se) {
  if ((se->attribute || se->simple) && is_pointer (se->xs_type)) {
    void **value = base + se->offset; int n;
    for (n = 0; n < se->max && *value; n++, value++);
    return n >= se->min? n : 0;
  } if (se->min < se->max) { int count;
    if (se->max == 1) return flag_set (base, se->bit);
    count = flag_count (base, se->bit, se->min, se->max);
    return count > se->max? se->max : count;
  } return se->min;
}

int output_item_count (Output *o, int level) {
  return o->stack.items[level].count;
}

int output_complete (Output *o) {
  return o->state == OUTPUT_COMPLETE;
}

int output_doc (Output *o, void *base, int type) {
  ElementStack *stack = &o->stack; StackItem *t;
  const SchemaElement *se; int length;
  const OutputDriver *d = o->driver; List *q;
  while (1) {
    switch (o->state) {
    case OUTPUT_START:
      if (type >= o->schema->length) return 0;
      o->se = &o->schema->elements[type];
      o->code = type; o->n = o->schema->length;
      o->base = base; o->state++; o->first = 1;
    case OUTPUT_ELEMENT:
      se = o->se; base = o->base; o->flag = se->bit;
      if (se->attribute) {
	if (!o->n) o->n = o->se->n;
	if (output_count (base, se))
	  o->state = OUTPUT_ATTRIBUTE;
	else if (se->min) goto error;
	else goto output_skip;
      } else if (t = push_element (stack, se, base)) {
	t->size = object_element_size (se, o->schema);
	base += se->offset;
	if (se->unbounded) {
	  if (q = *(List **)base)
 	    t->queue.first = q, o->base = q->data;
	  else goto element_skip;
	} else if (t->all = output_count (o->base, se))
	  o->base = base;
	else goto element_skip;
      output_element:
	if (!o->n) o->n = t->count >= se->min? se->n : 1;
	o->state = se->simple? OUTPUT_SIMPLE : OUTPUT_COMPLEX;
      } else return 0; break;
    element_skip: if (t->count < se->min) goto error;
      if (!o->n) o->n = se->n;
    element_next: o->se = pop_element (stack, &o->base);
    output_skip: o->code++;
    output_next: o->se++;
      o->state = o->se->n? OUTPUT_ELEMENT : OUTPUT_END; break;
    case OUTPUT_ATTRIBUTE:
      if (d->output_event (o, o->se, AT_EVENT)) o->state++;
      else goto full;
    case OUTPUT_ATTR_VALUE:
      if (d->output_attr_value (o, o->base+o->se->offset))
	goto output_next;
      else goto full; break;
    case OUTPUT_SIMPLE:
      if (d->output_event (o, o->se, SE_SIMPLE)) o->state++;
      else goto full;
    case OUTPUT_VALUE:
      if (d->output_value (o, o->base)) o->state++;
      else goto full;
    case OUTPUT_END:
      if (stack->n) {
	t = stack_top (stack); se = t->se;
	if (!d->output_event (o, se, EE_EVENT)) goto full;
	t->count++; // handle element sequences
	if (se->unbounded) {
	  queue_remove (&t->queue);
	  if (q = queue_peek (&t->queue))
	    o->base = q->data;
	  else goto element_skip;
	} else if (t->count < t->all) {
	  o->base += t->size;
	} else {
	  if (t->count == se->max) {
	    o->code--; // o->code+1 == 0
	    goto element_next;
	  } goto element_skip;
	}
	o->flag++; goto output_element;
      } else {
	if (o->bit) o->ptr++;
	d->output_done (o); o->state = OUTPUT_COMPLETE;
	return o->ptr - o->buffer;
      } break;
    case OUTPUT_COMPLEX:
      t = stack_top (stack); se = t->se;
      if (d->output_event (o, se, SE_COMPLEX)) {
	// jump to the element definition
        o->se = &o->schema->elements[se->index]+1;
	o->state = OUTPUT_ELEMENT;
      } else goto full; break;
    case OUTPUT_COMPLETE:
      o->ptr = o->buffer;
      o->state = OUTPUT_START;
      return 0;
    error:
      o->state = OUTPUT_ERROR;
    case OUTPUT_ERROR:
      return 0;
    }
  }
 full:
  length = o->ptr - o->buffer;
  o->ptr = o->buffer;
  return length;
}

#endif
