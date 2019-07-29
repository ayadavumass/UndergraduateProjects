#include "string.h"
#include "symbol.h"

#include "aps-tree.gen.xcons.h"

typedef STRING String;
typedef SYMBOL Symbol;
typedef int Boolean;

#define TRUE 1
#define FALSE 0

extern Symbol copy_Symbol(Symbol);
extern String copy_String(String);
extern Boolean copy_Boolean(Boolean);

extern void assert_Symbol(Symbol);
extern void assert_String(String);
extern void assert_Boolean(Boolean);

/* strange name mixups */
#define a_var_type(x) a_var_type_(x)
