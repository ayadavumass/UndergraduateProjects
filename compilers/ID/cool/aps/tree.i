/* tree.i */
/* John Boyland, 1993, 1994 */

/* 
 * simple tree and list routines.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define INFO void

extern int lex_current_line_number; /* lexer keeps up to date */

/* Tree nodes are pretty easy */

typedef struct tree_node *TNODE;

struct tree_node {
  int operator;      /* a unique identifier */
  int line_number;   /* stash the line number when node is made */
  INFO *info;
#ifdef __GNUC__
  void *children[0]; /* structure allocated big enough for each node*/
#else
  void *children[1];
#endif
};

extern int info_size;

static TNODE create_tnode(int operator, int num_children) {
  int i;
  TNODE t = (TNODE)malloc(sizeof(struct tree_node)+
			  sizeof(TNODE)*num_children +
			  info_size);
  if (t == NULL) {
    fatal_error("create_tnode could not allocate a node with %d children.",
		num_children);
  }
  t->operator = operator;
  t->line_number = yylineno;
  t->info = ((char *)t)+sizeof(struct tree_node)+sizeof(TNODE)*num_children;
  for (i=0; i < num_children; ++i) {
    t->children[i] = NULL;
  }
  return t;
}

int tnode_line_number(void *t) {
  return ((TNODE)t)->line_number;
}

INFO *tnode_info(void *t) {
  return ((TNODE)t)->info;
}

void set_tnode_info(void *t, INFO *info) {
  ((TNODE)t)->info = info;
}

/* List nodes are a little more difficult.
 * They are arrays that can gow if necessary.
 * In the header we store the maximum length and the actual length
 * along with an id that can be used to type check the list
 */

typedef struct list_node *List;

struct list_node {
  int list_id;
  int current_length;
  int max_length;
#ifdef __GNUC__
  void *elements[0];
#else
  void *elements[1];
#endif
};

static List make_list(int id, int max) {
  int i;
  List l=(List)malloc(sizeof(struct list_node)+max*sizeof(void *));
  if (l == NULL) {
    fatal_error("make_list could not allocate a list of %d elements");
  }
  l->list_id = id;
  l->current_length = 0;
  l->max_length = max;
  for (i=0; i < max; ++i) {
    l->elements[i] = NULL;
  }
  return l;
}

typedef void * (*Copier)(void *);

static List copy_list(List l, Copier c) {
  int i;
  int max = l->current_length;
  List copy = make_list(l->list_id,max);

  copy->current_length = max;
  for (i=0; i < max; ++i) {
    copy->elements[i] = (*c)(l->elements[i]);
  }
  return copy;
}

static List grow(List l, int n) { /* make list at least n long or doubled */
  int i;
  List new;
  if (n < 2*l->max_length) n = 2*l->max_length;
  new = make_list(l->list_id,n+1);
  for (i=0; i < l->current_length; ++i) {
    new->elements[i] = l->elements[i];
  }
  new->current_length = l->current_length;
  /*free(l);   could cause people with buggy programs to core dump */
  return new;
}

static List xcons(List l, void *e) {
  if (l->current_length > l->max_length) fatal_error("List corrupted: 0x%x",l);
  if (l->current_length == l->max_length) l = grow(l,0);
  l->elements[l->current_length++] = e;
  return l;
}

#if 0
/* unused and conflicts */
static List append(List l1, List l2) {
  int len = l1->current_length + l2->current_length;
  int i;
  if (len > l1->max_length) l1 = grow(l1,len);
  for (i=0; i < l2->current_length; ++i) {
    l1->elements[i+l1->current_length] = l2->elements[i];
  }
  l1->current_length += l2->current_length;
  return l1;
}
#endif

static void *nth(List l, int i) {
  if (i >= l->current_length)
    fatal_error("List nth out of bounds! nth(0x%x,%d)",l,i);
  return l->elements[i];
}

#define length(list) ((list)->current_length)

/*VARARGS1*/
void
fatal_error(char *fmt, ...)
{
  va_list args;
  
  va_start(args,fmt);
  
  (void)  fprintf(stderr, "fatal error: ");
  (void) vfprintf(stderr, fmt, args);
  (void)  fprintf(stderr, "\n");
  (void)   fflush(stderr);
  
  va_end(args);
  abort();
}

