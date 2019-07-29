/*
 *  cool.y
 *              Parser definition for the COOL language.
 *
 */
%{
#include <iostream>
#include "cool-tree.h"
#include "stringtab.h"
#include "utilities.h"

extern char *curr_filename;

void yyerror(char *s);        /*  defined below; called for each parse error */
extern int yylex();           /*  the entry point to the lexer  */

/************************************************************************/
/*                DONT CHANGE ANYTHING IN THIS SECTION                  */

Program ast_root;	      /* the result of the parse  */
Classes parse_results;        /* for use in semantic analysis */
int omerrs = 0;               /* number of errors in lexing and parsing */
%}

/* A union of all the types that can be the result of parsing actions. */
%union {
  Boolean boolean;
  Symbol symbol;
  Program program;
  Class_ class_;
  Classes classes;
  Feature feature;
  Features features;
  Formal formal;
  Formals formals;
  Case case_;
  Cases cases;
  Expression expression;
  Expressions expressions;
  char *error_msg;
}

/* 
   Declare the terminals; a few have types for associated lexemes.
   The token ERROR is never used in the parser; thus, it is a parse
   error when the lexer returns it.

   The integer following token declaration is the numeric constant used
   to represent that token internally.  Typically, Bison generates these
   on its own, but we give explicit numbers to prevent version parity
   problems (bison 1.25 and earlier start at 258, later versions -- at
   257)
*/
%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
%token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
%token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
%token <symbol>  STR_CONST 275 INT_CONST 276 
%token <boolean> BOOL_CONST 277
%token <symbol>  TYPEID 278 OBJECTID 279 
%token ASSIGN 280 NOT 281 LE 282 ERROR 283

/*  DON'T CHANGE ANYTHING ABOVE THIS LINE, OR YOUR PARSER WONT WORK       */
/**************************************************************************/
 
   /* Complete the nonterminal list below, giving a type for the semantic
      value of each non terminal. (See section 3.6 in the bison 
      documentation for details). */

/* Declare types for the grammar's non-terminals. */
%type <program> program
%type <classes> class_list
%type <class_> class


/* You will want to change the following line. */
%type <features> feature_list dummy_feature_list
%type <feature> feature
%type <formals> formals formal_list
%type <formal> formal
%type <expression> expr optional_initialization// let_list
%type <expressions> stmt_list actuals exp_list
//%type <cases> case_list
//%type <case_> simple_case

/* Precedence declarations go here. */

%left '-' '+'
%left '*' '/'
%left '~'
%left '@'
%left '.'


%%
/* 
   Save the root of the abstract syntax tree in a global variable.
*/
program	: class_list	{ ast_root = program($1); }
        ;

class_list
	: class			/* single class */
		{ $$ = single_Classes($1);
                  parse_results = $$; }
	| class_list class	/* several classes */
		{ $$ = append_Classes($1,single_Classes($2)); 
                  parse_results = $$; }
	;

/* If no parent is specified, the class inherits from the Object class. */
class	: CLASS TYPEID '{' dummy_feature_list '}' ';'
		{ $$ = class_($2,idtable.add_string("Object"),$4,
			      stringtable.add_string(curr_filename)); }
	| CLASS TYPEID INHERITS TYPEID '{' dummy_feature_list '}' ';'
		{ $$ = class_($2,$4,$6,stringtable.add_string(curr_filename)); }
	;

/* Feature list may be empty, but no empty features in list. */
dummy_feature_list:		/* empty */
                {  $$ = nil_Features(); }
		|	feature_list
				{ $$ =$1; }
		;

feature_list
	: feature ';'
		{  $$ = single_Features($1); }
        |
          error ';'
                { $$ = nil_Features(); }
	| feature_list feature ';'  	/* Several features */
		{  $$ = append_Features($1,single_Features($2)); }
	| feature_list error ';' 	/* error message */
		{  $$ = $1; }
	;

feature : OBJECTID  formals ':' TYPEID '{' expr '}'
		{ $$ = method($1,$2,$4,$6); }
	| OBJECTID ':' TYPEID optional_initialization
		{ $$ = attr($1,$3,$4); }
	;

optional_initialization
	:			/* Empty */
		{  $$ = no_expr(); //SET_NODELOC(0);
}
	| ASSIGN expr
		{ $$ = $2; }
	;

formals	: '(' ')' 		/* allow an empty formal list */
		{ $$ = nil_Formals(); }
	| '(' formal_list ')'
		{ $$ = $2; }
	| '(' error ')'
		{ $$ = nil_Formals();  }
	;

formal_list
	: formal 		 /* One formal */
		{  $$ = single_Formals($1); }            
	| formal_list ',' formal /* Several declarations */
		{ $$ = append_Formals($1,single_Formals($3)); }
	;

formal	: OBJECTID ':' TYPEID
		{  $$ = formal($1,$3); }
	;

expr	: OBJECTID ASSIGN expr 
          { $$ = assign($1,$3); }
        | expr '.' OBJECTID  actuals 
          { @$ = @2; //SET_NODELOC(@2);
            $$ = dispatch($1,$3,$4); }
        | IF expr THEN expr ELSE expr FI
          { $$ = cond($2,$4,$6); }
        | WHILE expr LOOP expr POOL
          { $$ = loop($2,$4); }
        | '{' stmt_list '}'
          { $$ = block($2); }
        | ISVOID expr
          { $$ = isvoid($2); }
        | expr '+' expr 
          { @$ = @2; //SET_NODELOC(@2);
            $$ = plus($1,$3); }
        | expr '-' expr
          { @$ = @2; //SET_NODELOC(@2);
            $$ = sub($1,$3); }
        | expr '*' expr
          { @$ = @2; //SET_NODELOC(@2);
            $$ = mul($1,$3); }
        | expr '/' expr
          { @$ = @2; //SET_NODELOC(@2);
            $$ = divide($1,$3); }
        | '~' expr
          { $$ = neg($2); }
        | expr '<' expr
          { @$ = @2; //SET_NODELOC(@2);
            $$ = lt($1,$3); }
        | expr '=' expr
          { @$ = @2;// SET_NODELOC(@2);
            $$ = eq($1,$3); }
        | expr LE expr
          { @$ = @2; //SET_NODELOC(@2);
            $$ = leq($1,$3); }
	| NOT expr
          { $$ = comp($2); }
        | '(' expr ')'
          { $$ = $2; }
        | INT_CONST
          { $$ = int_const($1); }
        | STR_CONST
          { $$ = string_const($1); }
        | BOOL_CONST
          { $$ = bool_const($1); }
        | OBJECTID
          { $$ = object($1); }
        | OBJECTID actuals
          { 
	    Expression self_obj = object(idtable.add_string("self"));
	    $$ = dispatch(self_obj,$1,$2); 
	  }
        ;

stmt_list: expr ';'
          { $$ = single_Expressions($1); }
        | error ';'
          { $$ = nil_Expressions(); }   // error in the first expression
        | stmt_list expr ';'            //  several expressions
          { $$ = append_Expressions($1,single_Expressions($2)); }
        | stmt_list error ';'   // error after the first expression
          { $$ = $1; }
        ;

actuals	: '(' ')'
		{  $$ = nil_Expressions(); }
	| '(' exp_list ')'	// List of argument values 
		{  $$ = $2; }
	;

exp_list
	: expr			/* One expression */
		{ $$ = single_Expressions($1); }
	| exp_list ',' expr	/* Several expressions */
		{ $$ = append_Expressions($1,single_Expressions($3)); }
	;


/* end of grammar */
%%

/* This function is called automatically when Bison detects a parse error. */
void yyerror(char *s)
{
  extern int curr_lineno;

  cerr << "\"" << curr_filename << "\", line " << curr_lineno << ": " \
    << s << " at or near ";
  print_cool_token(yychar);
  cerr << endl;
  omerrs++;

  if(omerrs>50) {fprintf(stdout, "More than 50 errors\n"); exit(1);}
}
