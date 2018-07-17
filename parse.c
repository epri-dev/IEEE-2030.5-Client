// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup parse Parse
    
    The parser converts an XML or EXI document into a C object (a typed data
    structure). The parser is schema based, which means that the document
    structure (the order and types of elements and values) is predefined by an
    XML schema. Successful parsing means that the document conforms to the
    schema, failure means that the document did not conform or that the
    document is incomplete. 

    The parser has a state and can stop and resume parsing at any point in the
    document, this means that a large document can be parsed in segments.
    This feature can be used to reduce the space needed to store documents and
    the segment of time used to process documents. For example, using a fixed
    length buffer to store document data means that the space required to store
    the document and the time used to process a document segment is constant.
    This can useful if there is a need share processing time between other
    tasks or if resources are constrained.

    The only requirement is that the buffer that contains the document is large
    enough to contain the largest XML or EXI token, the unit of information
    that advances the parser's state machine.
    @{
*/

typedef struct _Parser Parser;

/** @brief Parse an XML or EXI document.

    Use @ref parse_init or @ref exi_parse_init to initialize the Parser, then
    call parse_doc to parse the document. If the document is larger than the
    containing buffer, call @ref parser_data to get a pointer to the unparsed
    data, and @ref parser_rebuffer to update the parser once more data has been
    read. Repeat calls to @ref parse_doc until the data is completely read or
    the parse fails.
    @param p is a pointer to a Parser
    @param type is a pointer to the returned schema type of object
    @returns an object and type on success, NULL otherwise
*/
void *parse_doc (Parser *p, int *type);

/** @brief Is there an error in parsing the document? 

    Parsing can fail if the document is incomplete or there is an error in the
    document. This function checks for a parse error.
    @param p is a pointer to a Parser.
    @returns 1 if there is an error parsing the document, 0 otherwise.
*/
int parse_error (Parser *p);

/** @brief Print the current parse stack.
    @param p is a pointer to a Parser
*/
void print_parse_stack (Parser *p);

/** @brief Allocate a Parser. */
Parser *parser_new ();

/** @brief Free a Parser.
    @param p is a pointer to a Parser
*/
void parser_free (Parser *p);

/** @brief Return a pointer to a Parser's unparsed data
    @param p is a pointer to a Parser
*/
char *parser_data (Parser *p);

/** @brief Update the Parser's unparsed data pointer.
    @param p is a pointer to the Parser
    @param data is a pointer to the unparsed data
    @param length is the length of the unparsed data
*/
void parser_rebuffer (Parser *p, void *data, int length);

/** @} */

#include "schema.c"

#ifndef HEADER_ONLY

#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "string_table.c"

enum ParserState {PARSE_START, PARSE_ELEMENT, PARSE_NEXT, PARSE_ATTRIBUTE,
		  PARSE_VALUE, PARSE_END, PARSE_SEQUENCE, SEQUENCE_END,
		  PARSE_INVALID};
enum ParserError {ERROR_NONE, UNKNOWN_ELEMENT, STACK_OVERFLOW};

#define MAX_STACK 32 // document tree can be 32 levels deep

typedef struct {
  const SchemaElement *se;
  void *base;
  Queue queue;
  int size, count;
  union { int all; int diff; };
} StackItem;

typedef struct {
  int n;
  StackItem items[MAX_STACK];
} ElementStack;

#define stack_top(s) (&(s)->items[(s)->n-1])

struct _XmlParser;
struct _ParserDriver;

typedef struct _Parser {
  void *obj; int type; // completed object and type
  struct _XmlParser *xml;
  ElementStack stack;
  const Schema *schema;
  const SchemaElement *se;
  const struct _ParserDriver *driver;
  void *base; uint8_t *ptr, *end;
  StringTable *global, *local;
  int state, token, flag, bit;
  unsigned int xml_decl : 1;
  unsigned int need_token : 1;
  unsigned int empty : 1;
  unsigned int truncated : 1;
  //unsigned int incomplete : 1;
} Parser;

typedef struct _ParserDriver {
  int (*parse_start) (Parser *);
  int (*parse_next) (Parser *);
  int (*parse_end) (Parser *, const SchemaElement *);
  int (*parse_sequence) (Parser *, StackItem *);
  int (*parse_attr_value) (Parser *, void *);
  int (*parse_value) (Parser *, void *);
  void (*parse_done) (Parser *);
  void (*rebuffer) (Parser *, char *, int length);
} ParserDriver;

StackItem *push_element (ElementStack *stack, const SchemaElement *se,
			 void *base) { StackItem *t;
  if (stack->n == MAX_STACK) return NULL;
  stack->n++; t = stack_top (stack);
  memset (t, 0, sizeof (StackItem));
  t->se = se; t->base = base; return t;
}

const SchemaElement *pop_element (ElementStack *stack, void **base) {
  StackItem *t = stack_top (stack); stack->n--;
  *base = t->base; return t->se;
}

int parse_error (Parser *p) { return p->state != PARSE_START; }

void print_stack (ElementStack *stack, const Schema *schema) {
  StackItem *t = stack->items; int i;
  printf ("parse_stack:\n");
  for (i = 0; i < stack->n; i++, t++)
    printf ("  %d %s\n", i, se_name (t->se, schema));
  fflush (stdout);
}

void print_parse_stack (Parser *p) {
  print_stack (&p->stack, p->schema);
}

char *parser_data (Parser *p) { return p->ptr; }

void parser_rebuffer (Parser *p, void *data, int length) {
  p->driver->rebuffer (p, data, length);
}

#define set_count(flags, count, bit) \
  *(uint32_t *)(flags) |= (count) << (bit)

void *add_element (StackItem *t) {
  List *l = list_insert (NULL, calloc (1, t->size)); 
  queue_add (&t->queue, l); return l;
}

// return parsed object on success NULL otherwise
void *parse_doc (Parser *p, int *type) {
  const SchemaElement *se;
  const ParserDriver *d = p->driver;
  ElementStack *stack = &p->stack;
  StackItem *t; int size;
  while (1) {
    switch (p->state) {
    case PARSE_START:
      ok (d->parse_start (p));
      stack->n = 0; p->state++;
      size = object_element_size (p->se, p->schema);
      p->obj = p->base = calloc (1, size); break;
    case PARSE_ELEMENT:
      se = p->se; p->flag = se->bit;
      if (se->attribute) {
	if (!se->min && !is_pointer (se->xs_type)) {
	  set_count (p->base, 1, p->flag); p->flag++;
	} p->state = PARSE_ATTRIBUTE; continue;
      } else if (t = push_element (stack, se, p->base)) {
	p->base += se->offset;
	t->size = object_element_size (se, p->schema);
	if (se->unbounded) {
	  List *l = *(List **)p->base = add_element (t);
	  p->base = l->data;
	} else {
	  t->diff = se->max - se->min;
	  if (t->diff && !(se->simple && is_pointer (se->xs_type)))
	    p->flag += bit_count (t->diff);
	}
      } else goto parse_error;
    parse_element:
      if (se->simple) goto parse_value;
      p->se = &p->schema->elements[se->index+1];
      p->state = PARSE_NEXT;
    case PARSE_NEXT:
      ok (d->parse_next (p)); break;
    case PARSE_ATTRIBUTE:
      ok (d->parse_attr_value (p, p->base+p->se->offset));
      p->state--; p->se++; break;
    parse_value:
      p->state = PARSE_VALUE;
    case PARSE_VALUE:
      ok (d->parse_value (p, p->base)); p->state++; break;
    case PARSE_END:
      if(stack->n) {
	t = stack_top (stack); se = t->se;
	ok (d->parse_end (p, se)); t->count++;
	if (se->unbounded || t->count < se->max)
	  p->state = PARSE_SEQUENCE;
	else p->state = SEQUENCE_END;
      } else {
	d->parse_done (p);
	p->state = PARSE_START;
	*type = p->type; return p->obj;
      } break;
    case PARSE_SEQUENCE:
      t = stack_top (stack); se = t->se;
      if (d->parse_sequence (p, t)) {
	if (se->unbounded)
	  p->base = list_data (add_element (t));
	else if (t->count < se->max)
	  p->base += t->size;
	else goto parse_error;
	p->flag++; goto parse_element;
      } else if (p->state == PARSE_SEQUENCE)
	return NULL; break;
    case SEQUENCE_END:
      if (t->diff)
	set_count (t->base, t->count-se->min, se->bit);
      p->se = pop_element (stack, &p->base)+1;
      p->state = PARSE_NEXT; break;      
    case PARSE_INVALID: return NULL;
    }
  }
 parse_error:
  p->state = PARSE_INVALID; return NULL;
}

Parser *parser_new () { return calloc (1, sizeof (Parser)); }

void parser_free (Parser *p) { if (p->xml) free (p->xml); free (p); }

#endif
