%{
#include <stdio.h>
#include "aps-tree.h"
#include "aps-lex.h"
#include "symbol.h"

/* hacks */
#define a_var(name,type,constant,export) \
  a_value(name,nil_TypeFormals(),type,constant,export)
#define a_function(name,tf,f,rt,constant,e) \
  a_value(name,tf,function_type(f,value_decl(a_value(underscore_symbol, \
						     nil_TypeFormals(), \
						     rt,FALSE,FALSE), \
					     no_direction(FALSE), \
					     no_default())),\
	  constant,e)

extern Program the_tree;

static Symbol underscore_symbol, eq_symbol, less_symbol;

void init_parser() {
  underscore_symbol = intern_symbol("_");
  eq_symbol = intern_symbol("=");
  less_symbol = intern_symbol("<");
}


int public = FALSE; /* toggled by "private" and "public" */
int constant = TRUE; /* cleared by "var" */

static Expression sbinop(Symbol sym, Expression e1, Expression e2) {
  return funcall(value_name(sym),
		 xcons_Actuals(list_Actuals(e1),e2));
}

static Expression binop(char *name, Expression e1, Expression e2) {
  return funcall(value_name(intern_symbol(name)),
		 xcons_Actuals(list_Actuals(e1),e2));
}
			        
static Expression unop(char *name, Expression e) {
  return funcall(value_name(intern_symbol(name)),
		 list_Actuals(e));
}

#define list2_Formals(x,y) append_Formals(list_Formals(x),list_Formals(y))

/* we still have to re make the dump stuff */
static Pattern match_expression(Expression e) {
  Symbol s = gensym();
  ValueDecl v = a_var(s,no_type(),TRUE,FALSE);
  return and_pattern(pattern_var(v),
		     condition(binop("==",value_name(s),e)));
}

static ValueDecl first_Formal(Formals l) {
  switch (Formals_KEY(l)) {
  case KEYlist_Formals:
    return list_Formals_elem(l);
  case KEYappend_Formals:
    return first_Formal(append_Formals_l1(l));
  default:
    fatal_error("first_Formal got empty list");
    return a_var(underscore_symbol,no_type(),TRUE,FALSE);
  }
}

static TypeDecl first_TypeFormal(TypeFormals l) {
  switch (TypeFormals_KEY(l)) {
  case KEYlist_TypeFormals:
    return list_TypeFormals_elem(l);
  case KEYappend_TypeFormals:
    return first_TypeFormal(append_TypeFormals_l1(l));
  default:
    fatal_error("first_TypeFormal got empty list");
    return a_type(underscore_symbol,nil_TypeFormals(),nil_Signatures(),
		  TRUE,FALSE);
  }
}

static Type first_Types(Types l) {
  switch (Types_KEY(l)) {
  case KEYlist_Types:
    return list_Types_elem(l);
  case KEYappend_Types:
    return first_Types(append_Types_l1(l));
  default:
    fatal_error("first_Types got empty list");
    return no_type();
  }
}

#define cons_Actuals(x,l) \
	append_Actuals(list_Actuals(x),l)
#define cons_Formal(x,l) \
	append_Formal(list_Formal(x),l)
#define cons_PatternActuals(x,l) \
	append_PatternActuals(list_PatternActuals(x),l)

TypeActuals type_decls_to_actuals(TypeFormals tfs) {
  switch (TypeFormals_KEY(tfs)) {
  case KEYnil_TypeFormals:
    return nil_TypeActuals();
  case KEYlist_TypeFormals:
    return
      list_TypeActuals(type_name(a_type_name(list_TypeFormals_elem(tfs))));
  case KEYappend_TypeFormals:
    return
      append_TypeActuals(type_decls_to_actuals(append_TypeFormals_l1(tfs)),
			 type_decls_to_actuals(append_TypeFormals_l2(tfs)));
  }
  fatal_error("type_decls_to_actuals fell through");
}

Signature type_to_sig(Type ty) {
  switch (Type_KEY(ty)) {
  case KEYtype_name:
    return sig_name(type_name_name(ty));
  case KEYtype_inst:
    return sig_inst(type_to_sig(type_inst_base(ty)),
		    copy_TypeActuals(type_inst_type_actuals(ty)));
  default:
    yyerror("illegal module extension");
    return no_sig();
  }
}

static int infix_table[10][2] = {
  {OPL0, OPR0},
  {OPL1, OPR1},
  {OPL2, OPR2},
  {OPL3, OPR3},
  {OPL4, OPR4},
  {OPL5, OPR5},
  {OPL6, OPR6},
  {OPL7, OPR7},
  {OPL8, OPR8},
  {OPL9, OPR9}
};

%}

%union { /* two elements for each abstract non-terminal, exceptions noted*/
  Signature signature;
  Type type;
  Expression expression;
  SignatureDecl signatureDecl;
  TypeDecl typeDecl;
  ValueDecl valueDecl;
  PatternDecl patternDecl;
  Program program;
  Unit unit;
  Declaration declaration;
  Renaming renaming;
  Statement statement;
  Pattern pattern;
  Match match;
  Direction direction;
  Default default_tag;
  Units units;
  Declarations declarations;
  Statements statements;
  Actuals actuals;
  PatternActuals patternActuals;
  Types types;
  Expressions expressions;
  Patterns patterns;
  Matches matches;
  TypeActuals typeActuals;
  Renamings renamings;
  Formals formals;
  TypeFormals typeFormals;
  Signatures signatures;

  String string;
  Symbol symbol;
  int integer;
}

%token <symbol> keyAND
%token <symbol> keyATTRIBUTE
%token <symbol> keyATTRIBUTION
%token <symbol> keyBEGIN
%token <symbol> keyCASE
%token <symbol> keyCIRCULAR
%token <symbol> keyCOLLECTION
%token <symbol> keyCONSTANT
%token <symbol> keyCONSTRUCTOR
%token <symbol> keyDECLARE
%token <symbol> keyELSE
%token <symbol> keyELSIF
%token <symbol> keyEND
%token <symbol> keyENDIF
%token <symbol> keyEXTENDS
%token <symbol> keyFOR
/* %token <symbol> keyFOREIGN */
%token <symbol> keyFUNCTION
/* %token <symbol> keyGRAMMAR */
%token <symbol> keyGROUP
%token <symbol> keyIF
%token <symbol> keyIN
%token <symbol> keyINFIX
%token <symbol> keyINFIXL
%token <symbol> keyINFIXR
%token <symbol> keyINHERIT
/* %token <symbol> keyINHERITED */
%token <symbol> keyINPUT
/* %token <symbol> keyIS */
%token <symbol> keyLANGUAGE
/* %token <symbol> keyLOCAL */
%token <symbol> keyMATCH
%token <symbol> keyMODULE
%token <symbol> keyNOT
%token <symbol> keyON
%token <symbol> keyOR
%token <symbol> keyPATTERN
%token <symbol> keyPHYLUM
%token <symbol> keyPRAGMA
%token <symbol> keyPRIVATE
%token <symbol> keyPROCEDURE
%token <symbol> keyPUBLIC
%token <symbol> keyREMOTE
%token <symbol> keyRETURN
/* %token <symbol> keySET */
%token <symbol> keySIGNATURE
/* %token <symbol> keySYNTHESIZED */
%token <symbol> keyTHEN
%token <symbol> keyTYPE
%token <symbol> keyUSE
%token <symbol> keyVAR
%token <symbol> keyWITH

%token DOTDOT
%token DOUBLECOLON
%token DOTDOTDOT
%token TRIPLECOLON
%token ARROW

%token IDENTIFIER
%token OPERATOR
%token INTEGER
%token STRING_CONSTANT
%token CHARACTER_CONSTANT

%nonassoc COLONEQ
%left ','
%left keyAND
%left keyOR
%nonassoc keyNOT keyREMOTE /* keyFOREIGN */ keyIN

%left OPL0 DOTDOT
%right OPR0
%left OPL1
%right OPR1
%left OPL2
%right OPR2
%left OPL3
%right OPR3
%left OPL4 '='
%right OPR4
%left OPL5
%right OPR5
%left OPL6
%right OPR6 UNARY '-'
%left OPL7
%right OPR7
%left OPL8
%right OPR8
%left OPL9 OPERATOR
%right OPR9
%left DOTDOTDOT

%type <symbol> OPL0 OPR0 OPL1 OPR1 OPL2 OPR2 OPL3 OPR3 OPL4 OPR4
%type <symbol> OPL5 OPR5 OPL6 OPR6 OPL7 OPR7 OPL8 OPR8 OPL9 OPR9

%type <units> unit_list
%type <unit> unit with
%type <declarations> decl_list module_body
%type <declaration> decl inherit_decl var_decl function_decl type_decl 
%type <declaration> phy_decl con_decl pat_decl signature_decl
%type <declaration> attribute_decl match_decl pragma_decl module_decl
%type <declaration> result_type
%type <renamings> renamings
%type <renaming> renaming base_renaming
%type <direction> opt_direction direction
%type <default_tag> default lattice
%type <typeFormals> opt_polymorphism polymorphism type_formals type_formal
%type <formals> formals formal formal_list
%type <valueDecl> opt_formal
%type <type> type opt_typing simple_type atomic_type type_prefix
%type <type> opt_extension
%type <pattern> pattern pattern_body simple_pattern Pattern
%type <patternActuals> simple_pattern_args non_empty_pattern_args
%type <pattern> sequence_pattern
%type <patterns> choices sequence_pattern_args
%type <statement> stmts stmt stmt_elsif_part stmt_match_default opt_body
%type <statement> result_value
%type <statements> stmt_list
%type <matches> stmt_match_list
%type <match> match
%type <expression> atomic_expr simple_expr expr
%type <expression> lhs actual sequence_actual
%type <expressions> sequence_actuals
%type <actuals> actuals non_empty_actuals
%type <pattern> constructor_name
%type <symbol> IDENTIFIER OPERATOR id opt_id
%type <string> INTEGER STRING_CONSTANT CHARACTER_CONSTANT
%type <integer> optional_integer
%type <typeActuals> type_actuals type_parameters
%type <types> types non_empty_types
%type <signatures> signatures opt_signatures
%type <signature> signature

%%

program : { public=TRUE; } unit_list
  		{ the_tree = program($2); }
	;

unit_list
 	: /* nothing */
		{ $$ = nil_Units(); }
	| unit_list unit
		{ $$ = xcons_Units($1,$2); }
	| unit_list fixity ';'
		{ $$ = $1; }
	| unit_list keyPRIVATE ';'
		{ $$ = $1; public = FALSE; }
	| unit_list keyPUBLIC ';'
		{ $$ = $1; public = TRUE; }
	| unit_list ';'
		{ $$ = $1; }
	;

unit 	: with
		{ $$ = $1; }
	| decl
		{ $$ = decl_unit($1); }
	;

fixity	: keyINFIX optional_integer OPERATOR
		{ /* printf("infix[%s] := %d ?\n",symbol_name($3),$2); */
		  set_infix($3,infix_table[$2][0]); }
	| keyINFIXL optional_integer OPERATOR
		{ set_infix($3,infix_table[$2][0]); }
	| keyINFIXR optional_integer OPERATOR
		{ set_infix($3,infix_table[$2][1]); }
	;

optional_integer
	: /* EMPTY */
		{ $$ = 9; }
	| INTEGER
		{ char *s = (char *)$1;
		  if (s[1] != '\0') {
		    yyerror("fixity levels must be single digits:");
		  }
		  $$ = s[0]-'0';
		}
	;

with
	: keyWITH STRING_CONSTANT ';'
		{ $$ = with_unit($2); }
	;

decl_list
	:	{ $$ = nil_Declarations(); }
	| decl_list ';'
		{ $$ = $1; }
	| decl_list keyPRIVATE ';'
		{ $$ = $1; public = FALSE; }
	| decl_list keyPUBLIC ';'
		{ $$ = $1; public = TRUE; }
	| decl_list decl
		{ $$ = xcons_Declarations($1,$2); }
	;


decl	: signature_decl
	| module_decl
	| pragma_decl
	| type_decl
	| function_decl
	| var_decl
	| inherit_decl
	| phy_decl
	| con_decl
	| pat_decl
	| attribute_decl
	| match_decl
	| keyPRIVATE { $<integer>$ = public; public = FALSE; } decl
		{ $$ = $3; public = $<integer>2; }
	| keyPUBLIC { $<integer>$ = public; public = TRUE; } decl
		{ $$ = $3; public = $<integer>2; }
	| keyCONSTANT decl 
		{ $$ = $2; }
	| keyVAR { constant = FALSE; } decl
		{ $$ = $3; constant = TRUE; }
	| error
		{ $$ = no_decl(); }
	;

signature_decl
	: keySIGNATURE id polymorphism result_type module_body ';'
		{ $$ = signature_decl(a_signature($2,$3,constant,public),
				      $4,$5); }
	;

module_decl
	: keyMODULE id polymorphism formals result_type module_body ';'
		{ $$ = module_decl(a_signature($2,$3,constant,public),
				   a_type($2,nil_TypeFormals(),
					  list_Signatures
					  (module_sig(copy_TypeFormals($3),
						      $4,$5)),
					  constant,public),
				   $6); }
	| keyMODULE id polymorphism result_type module_body ';'
		{ $$ = module_decl(a_signature($2,$3,constant,public),
				   a_type($2,nil_TypeFormals(),
					  list_Signatures
					  (module_sig(copy_TypeFormals($3),
						      nil_Formals(),$4)),
					  constant,public),
				   $5); }
	;

result_type
	: opt_id opt_signatures opt_extension
		{ if ($1 == NULL) $1 = intern_symbol("Result");
		  $$ = type_decl(a_type($1,nil_TypeFormals(),$2,
					constant,public),$3); }
	;

opt_id	: /* empty */
		{ $$ = NULL; }
	| id
		{ $$ = $1; }
	;

opt_extension
	: /* NULL */
		{ $$ = no_type(); }
	| '=' type
		{ $$ = $2; }
	| keyEXTENDS type
		{ $$ = $2; }
	;

module_body
	: /* NULL */
		{ $$ = nil_Declarations(); }
	| keyBEGIN { $<integer>$ = public; public = TRUE; }
	    decl_list 
	  iskeyEND
		{ public = $<integer>2;
                  /* force the list to contain something if there is a block */
                  $$ = xcons_Declarations($3,no_decl()); }
	;

pragma_decl
	: keyPRAGMA id ';'
		{ $$ = pragma_decl($2,nil_Expressions()); }
	| keyPRAGMA id '(' ')' ';'
		{ $$ = pragma_decl($2,nil_Expressions()); }
	| keyPRAGMA id '(' sequence_actuals ')' ';'
		{ $$ = pragma_decl($2,$4); }
	;

inherit_decl
	: keyINHERIT type keyBEGIN
	    renamings
	  iskeyEND ';'
		{ $$ = inheritance($2,$4); }
	| keyINHERIT type ';'
		{ $$ = inheritance($2,nil_Renamings()); }
	;

renamings
	: /* NULL */
		{ $$ = nil_Renamings(); }
	| renamings renaming ';'
		{ $$ = xcons_Renamings($1,$2); }
	;

renaming
	: keyPUBLIC { $<integer>$ = public; public=TRUE; } base_renaming
		{ $$ = $3; public = $<integer>2; }
	| keyPRIVATE { $<integer>$ = public; public=FALSE; } base_renaming
		{ $$ = $3; public = $<integer>2; }
	| base_renaming
		{ $$ = $1; }
	| value_keyword id ARROW expr
		{ $$ = value_replacement(value_name($2),$4); }
	| type_keyword id ARROW type
		{ $$ = type_replacement(type_name($2),$4); }
	;

base_renaming
	: error
		{ $$ = no_renaming(); }
	| pattern_keyword id '=' id
		{ $$ = pattern_renaming(a_pattern($2,nil_TypeFormals(),
						  no_type(),constant,public),
					pattern_name($4)); }
	| value_keyword id '=' id
		{ $$ = value_renaming(a_value($2,nil_TypeFormals(),
					      no_type(),constant,public),
				      value_name($4)); }
	| type_keyword id '=' id
		{ $$ = type_renaming(a_type($2,nil_TypeFormals(),
					    nil_Signatures(),constant,public),
				     type_name($4)); }
	| signature_keyword id '=' id
		{ $$ = signature_renaming(a_signature($2,nil_TypeFormals(),
						      constant,public),
					  sig_name($4),
					  nil_Renamings()); }
	| signature_keyword id '=' id {$<integer>$ = public;}
          keyBEGIN
            renamings
          iskeyEND
		{ public = $<integer>5;
		  $$ = signature_renaming(a_signature($2,nil_TypeFormals(),
						      constant,public),
					  sig_name($4),
					  $7); }
	;
 
value_keyword
	: /* EMPTY */ {}
	| keyVAR {}
	| keyFUNCTION {}
	| keyPROCEDURE {}
	| keyATTRIBUTE {}
	| keyCONSTRUCTOR {} /* need to fix this */
	;

pattern_keyword
	: keyPATTERN {}
	;

type_keyword
	: keyTYPE {} 
	| keyPHYLUM {}
	;

signature_keyword
	: keyMODULE {} /* not quite right */
	| keySIGNATURE {}
	;

phy_decl
	: keyPHYLUM id ';'
		{ $$ = phylum_decl(a_type($2,nil_TypeFormals(),
					  nil_Signatures(),constant,public),
				   no_type()); }
	| keyPHYLUM id '=' type ';'
		{ $$ = phylum_decl(a_type($2,nil_TypeFormals(),
					  nil_Signatures(),constant,public),
		  		   $4); }
	;

con_decl
	: keyCONSTRUCTOR id opt_polymorphism formals result_value opt_body ';'
		{ $$ = constructor_decl
		    (a_value($2,$3,function_type($4,$5),
			     constant,public),
		     a_pattern($2,copy_TypeFormals($3),
			       function_type(copy_Formals($4),
					     copy_Statement($5)),
			       constant,public),
		     $6); }
	;

pat_decl
	: keyPATTERN id opt_polymorphism formals result_value pattern_body ';'
		{ $$ = pattern_decl(a_pattern($2,$3,function_type($4,$5),
					      constant,public),
				    $6); }
	;

pattern_body
	: /* Empty */
		{ $$ = no_pattern(); }
	| '=' choices
		{ $$ = choice_pattern($2); }
	| '='
		{ $$ = choice_pattern(nil_Patterns()); }
	;

attribute_decl /* trying out two different syntaxes */
	: opt_direction keyATTRIBUTE id opt_polymorphism opt_formal ':' type default ';'
	  	{ $$ = attribute_decl(
			 a_function($3,$4,list_Formals($5),$7,FALSE,public),
			 $1,$8); }
	| opt_direction keyATTRIBUTE opt_formal '.' id ':' type default ';'
		{ $$ = attribute_decl(
			  a_function($5,nil_TypeFormals(),
				     list_Formals($3),$7,FALSE,public),
			  $1,$8); }
	| opt_direction keyATTRIBUTE polymorphism opt_formal '.' id ':' type default ';'
		{ $$ = attribute_decl(
			  a_function($6,$3,list_Formals($4),$8,
				     FALSE,public),
			  $1,$9); }
	;

opt_direction
	: /* Empty */
		{ $$ = no_direction(FALSE); }
	| direction
	;

direction
	: keyINPUT
		{ $$ = no_direction(TRUE); }
	| keyINPUT keyCOLLECTION
		{ $$ = collection(TRUE); }
	| keyCOLLECTION
		{ $$ = collection(FALSE); }
	| keyINPUT keyCIRCULAR
		{ $$ = circular(TRUE); }
	| keyCIRCULAR
		{ $$ = circular(FALSE); }
	;

opt_formal
	:	{ $$ = a_var(underscore_symbol,type_name(underscore_symbol),
		  	     TRUE,FALSE); }
	| id
		{ $$ = a_var(underscore_symbol,type_name($1),TRUE,FALSE); }
	| '(' id ':' id ')'
		{ $$ = a_var($2,type_name($4),TRUE,FALSE); }
	;

default
	: /* Empty */
		{ $$ = no_default(); }
	| COLONEQ expr
		{ $$ = simple($2); }
	| COLONEQ lattice
		{ $$ = $2; }
	;

lattice : expr ',' expr
		{ $$ = composite($1,$3); }
	| expr ',' expr ',' expr
		{ $$ = lattice($1,$3,$5); }
	;

match_decl
	: match
		{ $$ = top_level_match($1); }
	;

type_decl
	: keyTYPE id opt_signatures ';'
		{ $$ = type_decl(a_type($2,nil_TypeFormals(),$3,
					constant,public),
				 no_type()); }
	| keyTYPE id opt_signatures '=' type ';'
		{ $$ = type_decl(a_type($2,nil_TypeFormals(),$3,
					constant,public),$5); }
	;

var_decl
	: opt_direction id ':' type default ';'
		{ $$ = value_decl(a_var($2,$4,constant,public),$1,$5); }
	;

function_decl
	: keyFUNCTION id opt_polymorphism formals result_value opt_body ';'
		{ $$ = function_decl(a_value($2,$3,function_type($4,$5),
					     constant,public),
				     $6); }
	| keyPROCEDURE id opt_polymorphism formals result_value opt_body ';'
		{ $$ = procedure_decl(a_value($2,$3,function_type($4,$5),
					      constant,public),
				      $6); }
	;

result_value
	: opt_direction opt_id opt_typing default
		{ if ($2 == NULL) $2 = intern_symbol("result");
		  $$ = value_decl(a_value($2,nil_TypeFormals(),$3,FALSE,FALSE),
				  $1,$4); }
	;

opt_typing
	: /* EMPTY */
		{ $$ = no_type(); }
	| ':' type
		{ $$ = $2; }
	;

opt_body
	: /* EMPTY */
		{ $$ = no_stmt(); }
	| keyBEGIN stmts iskeyEND
		{ $$ = $2; }
	;

opt_polymorphism
	: /* EMPTY */
		{ $$ = nil_TypeFormals(); }
	| polymorphism
		{ $$ = $1; }
	;

polymorphism
	: '[' type_formals ']'
		{ $$ = $2; }
	| '[' ']'
		{ $$ = nil_TypeFormals(); }
	;

type_formals
	: type_formal
		{ $$ = $1; }
	| type_formals ';' type_formal
		{ $$ = append_TypeFormals($1,$3); }
	;

type_formal
	: id opt_signatures
		{ $$ = list_TypeFormals(a_type($1,nil_TypeFormals(),
					       $2,TRUE,FALSE)); }
	| id ',' type_formal
		{ Signatures sigs = a_type_signatures(first_TypeFormal($3));
		  $$ = append_TypeFormals(
		  	list_TypeFormals(a_type($1,nil_TypeFormals(),
						sigs,TRUE,FALSE)),$3); }
	;

opt_signatures
	: /* EMPTY */
		{ $$ = nil_Signatures(); }
	| DOUBLECOLON signatures
		{ $$ = $2; }
	;

signatures
	: signature
		{ $$ = list_Signatures($1); }
	| signatures ',' signature
		{ $$ = xcons_Signatures($1,$3); }
	;

signature
	: id
		{ $$ = sig_name($1); }
	| id type_parameters
		{ $$ = sig_inst(sig_name($1),$2); }
	| keyCONSTANT signature
		{ $$ = constant_sig($2); }
	| keyVAR signature
		{ $$ = var_sig($2); }
	| '{' types '}'
		{ $$ = fixed_sig($2); }
	;

type_parameters
	: '[' ']'
		{ $$ = nil_TypeActuals(); }
	| '[' type_actuals ']'
		{ $$ = $2; }
	;

type_actuals
	: type
		{ $$ = list_TypeActuals($1); }
	| type_actuals ',' type
		{ $$ = xcons_TypeActuals($1,$3); }
	;

types
	: /* EMPTY */
		{ $$ = nil_Types(); }
	| non_empty_types
		{ $$ = $1; }
	;

non_empty_types
	: type
		{ $$ = list_Types($1); }
	| non_empty_types ',' type
		{ $$ = xcons_Types($1,$3); }
	;

formals
	: '(' formal_list ')'
		{ $$ = $2; }
	| '(' ')'
		{ $$ = nil_Formals(); }
	;

formal_list
	: formal
		{ $$ = $1; }
	| formal_list ';' formal
		{ $$ = append_Formals($1,$3); }
	;

formal	: id ':' type
		{ $$ = list_Formals(a_var($1,$3,TRUE,FALSE)); }
	| id ',' formal
		{ Type ty = a_value_type_(first_Formal($3));
		  $$ = append_Formals(list_Formals(a_var($1,ty,TRUE,FALSE)),
				      $3); }
	;

type	: simple_type
		{ $$ = $1; }
	| keyREMOTE type
		{ $$ = remote_type($2); }
/*	| keyFOREIGN type
		{ $$ = foreign_type($2); } */
	| keyFUNCTION formals opt_typing
		{ $$ = function_type($2,value_decl(a_value(underscore_symbol,
							   nil_TypeFormals(),
							   $3,FALSE,FALSE),
						   no_direction(FALSE),
						   no_default())); }
	;

/* simple types are only left recursive */
simple_type
	: atomic_type
	| simple_type '$' id
		{ $$ = qual_type($1,type_name($3)); }
	| simple_type type_parameters
		{ $$ = type_inst($1,$2,nil_Actuals()); }
	| simple_type type_parameters actuals
		{ $$ = type_inst($1,$2,$3); }
	;

/* atomic types are properly contained and recurse neither to left nor right */
atomic_type
	: id
		{ $$ = type_name($1); }
/*	| DOUBLECOLON id
 *		{ $$ = key_type(type_name($2)); }
 */
	| '{' type '}'
		{ $$ = list_type($2); }
	| '{' type ',' expr '}'
		{ $$ = set_type($2,$4); }
	| '{' type ',' expr ',' expr '}'
		{ $$ = ordered_set_type($2,$4,$6); }
	| '(' type ')'
		{ $$ = $2; }
	;

/* type_prefix is a type that comes before a $ and can be parsed in an
 * expression context without conflicts.
 */
type_prefix
	: id
		{ $$ = type_name($1); }
	| type_prefix '$' id
		{ $$ = qual_type($1,type_name($3)); }
	;

Pattern : pattern
	| pattern keyIF expr
		{ $$ = and_pattern($1,condition($3)); }
	| error
		{ $$ = no_pattern(); }
	;

choices
	: Pattern
		{ $$ = list_Patterns($1); }
	| choices ',' Pattern
		{ $$ = xcons_Patterns($1,$3); }
	;

pattern	: simple_pattern
	| pattern keyAND simple_pattern
		{ $$ = and_pattern($1,$3); }
	| pattern '=' simple_pattern
		{ $$ = and_pattern($1,$3); }
	;

simple_pattern
	: constructor_name simple_pattern_args
		{ $$ = simple_pattern($1,$2); }
	| '{' sequence_pattern_args '}'
		{ $$ = sequence_pattern($2,no_type()); }
	| '{' sequence_pattern_args '}' ':' type
		{ $$ = sequence_pattern($2,$5); }
	| type_prefix '$' '{' sequence_pattern_args '}'
		{ $$ = sequence_pattern($4,$1); }
	| '{' '}'
		{ $$ = sequence_pattern(nil_Patterns(),no_type()); }
	| '{' '}' ':' type
		{ $$ = sequence_pattern(nil_Patterns(),$4); }
	| type_prefix '$' '{' '}'
		{ $$ = sequence_pattern(nil_Patterns(),$1); }
	| '?' id ':' type
		{ $$ = pattern_var(a_var($2,$4,TRUE,FALSE)); }
	| '?' id
		{ $$ = pattern_var(a_var($2,no_type(),TRUE,FALSE)); }
	| '?'
		{ $$ = pattern_var(a_var(underscore_symbol,no_type(),
					 TRUE,FALSE)); }
/*	| '?' id simple_pattern_args
		{ $$ = pattern_call(pattern_name($2),$3); }
 */
	| '!' simple_expr
		{ $$ = match_expression($2); }
/* fix this: to use the top-level append pattern function
	| simple_pattern DOTDOTDOT simple_pattern
		{ $$ = append_pattern($1,$3); }
 */
/* conflicts with '(' ')'
	| simple_expr
		{ $$ = match_expression($1); }
 */
	| id 	{ $$ = match_expression(value_name($1)); }
	| INTEGER		
		{ $$ = match_expression(integer_const($1)); }
	| STRING_CONSTANT	
		{ $$ = match_expression(string_const($1)); }
	| '(' Pattern ')'
		{ $$ = $2; }
	;

constructor_name
	: id
		{ $$ = pattern_name($1); }
	| type_prefix '$' id
		{ $$ = qual_pattern($1,pattern_name($3)); }
	;

simple_pattern_args
	: '(' ')'
		{ $$ = nil_PatternActuals(); }
	| '(' non_empty_pattern_args ')'
		{ $$ = $2; }
	;

non_empty_pattern_args
	: Pattern
		{ $$ = list_PatternActuals($1); }
	| id COLONEQ Pattern
		{ $$ = list_PatternActuals(pattern_actual($3,
		                                          value_name($1))); }
	| DOTDOTDOT
		{ $$ = list_PatternActuals(rest_pattern(no_pattern())); }
	/* right recursion but not too bad because the list is rarely
           long, and it enforces ... only being at the end: */
	| Pattern ',' non_empty_pattern_args
		{ $$ = cons_PatternActuals($1,$3); }
	| id COLONEQ Pattern ',' non_empty_pattern_args
		{ $$ = cons_PatternActuals
		         (pattern_actual($3,value_name($1)),$5); }
	;

sequence_pattern_args
	: sequence_pattern
		{ $$ = list_Patterns($1); }
	| sequence_pattern_args ',' sequence_pattern
		{ $$ = xcons_Patterns($1,$3); }
	;

sequence_pattern
	: pattern
		{ $$ = $1; }
	| DOTDOTDOT
		{ $$ = rest_pattern(no_pattern()); }
	| DOTDOTDOT keyAND simple_pattern
		{ $$ = rest_pattern($3); }
	;

stmts 	: stmt_list
		{ $$ = stmts($1); }
	;

stmt_list
	: 	{ $$ = nil_Statements(); }
	| stmt_list stmt
		{ $$ = xcons_Statements($1,$2); }
	;

stmt	: error
		{ $$ = no_stmt(); }
	| ';'	{ $$ = no_stmt(); }
	| expr ';'
		{ $$ = effect($1); }
	| lhs COLONEQ expr ';'
		{ $$ = assign($1,$3); }
/*	| keyRETURN expr ';'
 *		{ $$ = return_stmt($2); }
 */
	| keyIF expr keyTHEN stmts stmt_elsif_part iskeyENDIF ';'
		{ $$ = if_stmt($2,$4,$5); }
	| keyFOR id ':' type keyIN expr keyBEGIN stmts iskeyEND ';'
		{ $$ = for_in_stmt(a_var($2,$4,TRUE,FALSE),$6,$8); }
	| keyFOR id ':' type keyON expr keyBEGIN stmts iskeyEND ';'
		{ $$ = for_on_stmt(a_var($2,$4,TRUE,FALSE),$6,$8); }
/*	| keyLOCAL id ':' type default ';'
 *		{ $$ = local_decl(a_var($2,$4,FALSE,FALSE),$5,no_expr()); }
 */
	/* we have to inline opt_direction in order to avoid S/R conflicts */
	| direction id ':' type default ';'
		{ $$ = value_decl(a_var($2,$4,TRUE,FALSE),$1,$5); }
	| id ':' type default ';'
		{ $$ = value_decl(a_var($1,$3,TRUE,FALSE),
				  no_direction(FALSE),$4); }
	| keyVAR opt_direction id ':' type default ';'
		{ $$ = value_decl(a_var($3,$5,FALSE,FALSE),$2,$6); }
	| direction keyVAR id ':' type default ';'
		{ $$ = value_decl(a_var($3,$5,FALSE,FALSE),$1,$6); }
	| keyBEGIN stmts iskeyEND ';'
		{ $$ = $2; }
	| keyCASE expr keyBEGIN stmt_match_list stmt_match_default iskeyEND ';'
		{ $$ = case_stmt($2,$4,$5); }
	| keyFOR expr keyBEGIN stmt_match_list iskeyEND ';'
		{ $$ = for_stmt($2,$4); }
	;

lhs	: expr
		{ $$ = $1; }
/*
	| lhs '.' expr
		{ $$ = funcall($3,list_Actuals($1)); }
	| expr %prec LOWEST
		{ $$ = $1; yyerror("illformed LHS of assignment") }
*/
	;

stmt_elsif_part /* right recursion does the association right */
	: 	{ $$ = no_stmt(); }
	| keyELSE stmts
		{ $$ = $2; }
	| keyELSIF expr keyTHEN stmts stmt_elsif_part
		{ $$ = if_stmt($2,$4,$5); }
	;

stmt_match_list
	:	{ $$ = nil_Matches(); }
	| stmt_match_list match
		{ $$ = xcons_Matches($1,$2); }
	;

match	: keyMATCH opt_polymorphism Pattern keyBEGIN stmts iskeyEND ';'
		{ $$ = matcher($2,$3,$5); }
	;

stmt_match_default
	:	{ $$ = no_stmt(); }
	| keyELSE stmts
		{ $$ = $2; }
	;

expr	: simple_expr
	| expr DOTDOTDOT	{ $$ = repeat($1); }
	| expr DOTDOT expr	{ $$ = binop("..",$1,$3); }
	| expr OPL0 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR0 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL1 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR1 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL2 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR2 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL3 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR3 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL4 expr	{ $$ = sbinop($2,$1,$3); }
	| expr '=' expr		{ $$ = binop("=",$1,$3); }
	| expr OPR4 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL5 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR5 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL6 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR6 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL7 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR7 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL8 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR8 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPL9 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPR9 expr	{ $$ = sbinop($2,$1,$3); }
	| expr OPERATOR expr	{ $$ = sbinop($2,$1,$3); }
	| expr '-' expr		{ $$ = binop("-",$1,$3); }
	| '-' expr %prec UNARY	{ $$ = unop("#-",$2); }
	| expr keyAND expr	{ $$ = binop("and",$1,$3); }
	| expr keyOR expr	{ $$ = binop("or",$1,$3); }
	| keyNOT expr		{ $$ = unop("not",$2); }
	| expr keyIN expr	{ $$ = binop("in",$1,$3); }
	| expr keyNOT keyIN expr	{ $$ = unop("not",binop("in",$1,$4)); }
/* 	| '#' expr %prec UNARY	{ $$ = unop("#",$2); } */
/*	| keyBEGIN stmts iskeyEND
 *		{ $$ = block($2,no_direction(),no_default()); }
 *	| keyCOLLECTION lattice keyBEGIN stmts iskeyEND
 *		{ $$ = block($4,collect(),$2); }
 *	| keyCIRCULAR lattice keyBEGIN stmts iskeyEND
 *		{ $$ = block($4,fixpoint(),$2); }
 */
	;


/* only left recursion */
simple_expr
	: atomic_expr
	| simple_expr type_parameters
		{ $$ = value_inst($1,$2); }
	| simple_expr actuals
		{ $$ = funcall($1,$2); }
	| simple_expr '.' atomic_expr
		{ $$ = funcall($3,list_Actuals($1)); }
	;


/* no recursion to the left or right */
atomic_expr
	: id			{ $$ = value_name($1); }
	| keyTYPE id		{ $$ = type_value(type_name($2)); }
	| keyMODULE id		{ $$ = signature_value(sig_name($2)); }
	| keySIGNATURE id	{ $$ = signature_value(sig_name($2)); }
/* I want to get rid of :: things
 *	| DOUBLECOLON constructor_name
 *				{ $$ = constructor_key($2); }
 *	| keyCONSTRUCTOR DOUBLECOLON id
 *		{ $$ = key(type_name($3)); }
 *	| simple_expr DOUBLECOLON id
 *		{ $$ = selector($1,value_name($3)); }
 *	| id TRIPLECOLON '(' ')'
 *		{ $$ = nil_constructor(type_name($1)); }
 *	| id TRIPLECOLON '(' '.' ')'
 *		{ $$ = single_constructor(type_name($1)); }
 *	| id TRIPLECOLON '(' DOTDOTDOT ')'
 *		{ $$ = append_constructor(type_name($1)); }
 */
	| INTEGER		{ $$ = integer_const($1); }
	| STRING_CONSTANT	{ $$ = string_const($1); }
  	| CHARACTER_CONSTANT	{ $$ = char_const($1);}
	| '(' expr ')'		{ $$ = $2; }
	| '{' sequence_actuals '}'	
				{ $$ = sequence($2,no_type()); }
	| '{' '}'
				{ $$ = sequence(nil_Expressions(),no_type()); }
/* we cannot have even atomic_type here because (T)$... would cause a
 * conflict (how would T become a type_name and not a value_name).
 */
	| type_prefix '$' '{' sequence_actuals '}'	
				{ $$ = sequence($4,$1); }
	| type_prefix '$' '{' '}'
				{ $$ = sequence(nil_Expressions(), $1); }
	| type_prefix '$' id
		{ $$ = qual_value($1,value_name($3)); }
	| '(' error ')'		{ $$ = no_expr(); }
	;

actuals	: '(' ')'
		{ $$ = nil_Actuals(); }
	| '(' non_empty_actuals ')'
		{ $$ = $2; }
	;

non_empty_actuals
	: actual
		{ $$ = list_Actuals($1); }
	| non_empty_actuals ',' actual
		{ $$ = xcons_Actuals($1,$3); }
	;

actual	: expr
		{ $$ = $1; }
	;

sequence_actuals
	: sequence_actual
		{ $$ = list_Expressions($1); }
	| sequence_actuals ',' sequence_actual
		{ $$ = xcons_Expressions($1,$3); }
	;

sequence_actual
	: expr
	| sequence_actual keyIF expr
		{ $$ = guarded($1,$3); }
	| sequence_actual keyFOR id ':' type keyIN expr
		{ $$ = controlled($1,a_var($3,$5,TRUE,FALSE),$7); } 
	;


id	: IDENTIFIER
		{ $$ = $1; }
	| '(' keyAND ')'
		{ $$ = $2; }
	| '(' keyATTRIBUTE ')'
		{ $$ = $2; }
	| '(' keyATTRIBUTION ')'
		{ $$ = $2; }
	| '(' keyBEGIN ')'
		{ $$ = $2; }
	| '(' keyCASE ')'
		{ $$ = $2; }
	| '(' keyCIRCULAR ')'
		{ $$ = $2; }
	| '(' keyCOLLECTION ')'
		{ $$ = $2; }
	| '(' keyCONSTANT ')'
		{ $$ = $2; }
	| '(' keyCONSTRUCTOR ')'
		{ $$ = $2; }
	| '(' keyDECLARE ')'
		{ $$ = $2; }
	| '(' keyELSE ')'
		{ $$ = $2; }
	| '(' keyELSIF ')'
		{ $$ = $2; }
	| '(' keyEND ')'
		{ $$ = $2; }
	| '(' keyENDIF ')'
		{ $$ = $2; }
	| '(' keyEXTENDS ')'
		{ $$ = $2; }
	| '(' keyFOR ')'
		{ $$ = $2; }
/*	| '(' keyFOREIGN ')'
		{ $$ = $2; } */
	| '(' keyFUNCTION ')'
		{ $$ = $2; }
/*	| '(' keyGRAMMAR ')'
		{ $$ = $2; } */
	| '(' keyGROUP ')'
		{ $$ = $2; }
	| '(' keyIF ')'
		{ $$ = $2; }
	| '(' keyIN ')'
		{ $$ = $2; }
	| '(' keyINFIX ')'
		{ $$ = $2; }
	| '(' keyINFIXL ')'
		{ $$ = $2; }
	| '(' keyINFIXR ')'
		{ $$ = $2; }
	| '(' keyINHERIT ')'
		{ $$ = $2; }
	| '(' keyINPUT ')'
		{ $$ = $2; }
/*	| '(' keyIS ')'
		{ $$ = $2; } */
	| '(' keyLANGUAGE ')'
		{ $$ = $2; }
/*	| '(' keyLOCAL ')'
		{ $$ = $2; } */
	| '(' keyMATCH ')'
		{ $$ = $2; }
	| '(' keyMODULE ')'
		{ $$ = $2; }
	| '(' keyNOT ')'
		{ $$ = $2; }
	| '(' keyON ')'
		{ $$ = $2; }
	| '(' keyOR ')'
		{ $$ = $2; }
	| '(' keyPATTERN ')'
		{ $$ = $2; }
	| '(' keyPHYLUM ')'
		{ $$ = $2; }
	| '(' keyPRAGMA ')'
		{ $$ = $2; }
	| '(' keyPRIVATE ')'
		{ $$ = $2; }
	| '(' keyPROCEDURE ')'
		{ $$ = $2; }
	| '(' keyPUBLIC ')'
		{ $$ = $2; }
	| '(' keyREMOTE ')'
		{ $$ = $2; }
	| '(' keyRETURN ')'
		{ $$ = $2; }
/* 	| '(' keySET ')'
		{ $$ = $2; } */
	| '(' keySIGNATURE ')'
		{ $$ = $2; }
/*	| '(' keySYNTHESIZED ')'
		{ $$ = $2; } */
	| '(' keyTHEN ')'
		{ $$ = $2; }
	| '(' keyTYPE ')'
		{ $$ = $2; }
	| '(' keyUSE ')'
		{ $$ = $2; }
	| '(' keyVAR ')'
		{ $$ = $2; }
	| '(' keyWITH ')'
		{ $$ = $2; }
	| '(' OPL0 ')'
		{ $$ = $2; }
	| '(' OPR0 ')'
		{ $$ = $2; }
	| '(' OPL1 ')'
		{ $$ = $2; }
	| '(' OPR1 ')'
		{ $$ = $2; }
	| '(' OPL2 ')'
		{ $$ = $2; }
	| '(' OPR2 ')'
		{ $$ = $2; }
	| '(' OPL3 ')'
		{ $$ = $2; }
	| '(' OPR3 ')'
		{ $$ = $2; }
	| '(' OPL4 ')'
		{ $$ = $2; }
	| '(' OPR4 ')'
		{ $$ = $2; }
	| '(' OPL5 ')'
		{ $$ = $2; }
	| '(' OPR5 ')'
		{ $$ = $2; }
	| '(' OPL6 ')'
		{ $$ = $2; }
	| '(' OPR6 ')'
		{ $$ = $2; }
	| '(' OPL7 ')'
		{ $$ = $2; }
	| '(' OPR7 ')'
		{ $$ = $2; }
	| '(' OPL8 ')'
		{ $$ = $2; }
	| '(' OPR8 ')'
		{ $$ = $2; }
	| '(' OPL9 ')'
		{ $$ = $2; }
	| '(' OPR9 ')'
		{ $$ = $2; }
	| '(' '=' ')'
		{ $$ = intern_symbol("="); }
	| '(' '-' ')'
		{ $$ = intern_symbol("-"); }
	| '(' OPERATOR ')'
		{ $$ = $2; }
	| '{' '.' '}'
		{ $$ = intern_symbol("{}"); }
/*	| '(' ')'
 *		{ $$ = intern_symbol(""); }
 */
	| '(' '.' ')' 
		{ $$ = intern_symbol("."); }
	| '(' DOTDOT ')'
		{ $$ = intern_symbol(".."); }
	| '(' DOTDOTDOT ')' 
		{ $$ = intern_symbol("..."); }
	;

iskeyEND
	: keyEND {}
	| keyENDIF { yylexerror("endif used for end"); }
	;

iskeyENDIF
	: keyENDIF {}
	| keyEND { yylexerror("end used for endif"); }
	;

%%

yylexerror(char *s) {
  int saved = yytoken;
  yytoken = LEX_ERROR;
  yyerror(s);
  yytoken = saved;
}

yyerror(char *s) {
  extern int aps_parse_error;
  aps_parse_error = 1;
  fprintf(stderr,"%s:%d: %s",yyfilename,yylineno,s);
  if (yytoken == 0) {
    fprintf(stderr," at end of file\n");
  } else if (yytoken == LEX_ERROR) { /* lex error, do nothing */
    fprintf(stderr,"\n");
  } else if (yytoken < 32) { /* why is this happening? */
    fprintf(stderr," before token 0x%d\n",yytoken);
  } else if (yytoken < 128) {
    fprintf(stderr," before token \"%c\"\n",yytoken);
  } else {
    fprintf(stderr," before ");
    switch (yytoken) {
    case DOTDOTDOT:
      fprintf(stderr,"token \"...\"");
      break;
    case DOTDOT:
      fprintf(stderr,"token \"..\"");
      break;
    case DOUBLECOLON:
      fprintf(stderr,"token \"::\"");
      break;
    case TRIPLECOLON:
      fprintf(stderr,"token \":::\"");
      break;
    case COLONEQ:
      fprintf(stderr,"token \":=\"");
      break;

    case IDENTIFIER:
      fprintf(stderr,"identifier \"%s\"",symbol_name(yylval.symbol));
      break;
    case OPERATOR:
    case OPL0: case OPR0:
    case OPL1: case OPR1:
    case OPL2: case OPR2:
    case OPL3: case OPR3:
    case OPL4: case OPR4:
    case OPL5: case OPR5:
    case OPL6: case OPR6:
    case OPL7: case OPR7:
    case OPL8: case OPR8:
    case OPL9: case OPR9:
      fprintf(stderr,"operator \"%s\"",symbol_name(yylval.symbol));
      break;
    case INTEGER:
    case CHARACTER_CONSTANT:
    case STRING_CONSTANT:
      print_string(stderr,yylval.string);
      break;
      
    default:
      fprintf(stderr,"keyword \"%s\"",symbol_name(yylval.symbol));
      break;
    }
    fprintf(stderr,"\n");
  }
}
