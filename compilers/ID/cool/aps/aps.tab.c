/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse aps_yyparse
#define yylex   aps_yylex
#define yyerror aps_yyerror
#define yylval  aps_yylval
#define yychar  aps_yychar
#define yydebug aps_yydebug
#define yynerrs aps_yynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     keyAND = 258,
     keyATTRIBUTE = 259,
     keyATTRIBUTION = 260,
     keyBEGIN = 261,
     keyCASE = 262,
     keyCIRCULAR = 263,
     keyCOLLECTION = 264,
     keyCONSTANT = 265,
     keyCONSTRUCTOR = 266,
     keyDECLARE = 267,
     keyELSE = 268,
     keyELSIF = 269,
     keyEND = 270,
     keyENDIF = 271,
     keyEXTENDS = 272,
     keyFOR = 273,
     keyFUNCTION = 274,
     keyGROUP = 275,
     keyIF = 276,
     keyIN = 277,
     keyINFIX = 278,
     keyINFIXL = 279,
     keyINFIXR = 280,
     keyINHERIT = 281,
     keyINPUT = 282,
     keyLANGUAGE = 283,
     keyMATCH = 284,
     keyMODULE = 285,
     keyNOT = 286,
     keyON = 287,
     keyOR = 288,
     keyPATTERN = 289,
     keyPHYLUM = 290,
     keyPRAGMA = 291,
     keyPRIVATE = 292,
     keyPROCEDURE = 293,
     keyPUBLIC = 294,
     keyREMOTE = 295,
     keyRETURN = 296,
     keySIGNATURE = 297,
     keyTHEN = 298,
     keyTYPE = 299,
     keyUSE = 300,
     keyVAR = 301,
     keyWITH = 302,
     DOTDOT = 303,
     DOUBLECOLON = 304,
     DOTDOTDOT = 305,
     TRIPLECOLON = 306,
     ARROW = 307,
     IDENTIFIER = 308,
     OPERATOR = 309,
     INTEGER = 310,
     STRING_CONSTANT = 311,
     CHARACTER_CONSTANT = 312,
     COLONEQ = 313,
     OPL0 = 314,
     OPR0 = 315,
     OPL1 = 316,
     OPR1 = 317,
     OPL2 = 318,
     OPR2 = 319,
     OPL3 = 320,
     OPR3 = 321,
     OPL4 = 322,
     OPR4 = 323,
     OPL5 = 324,
     OPR5 = 325,
     OPL6 = 326,
     UNARY = 327,
     OPR6 = 328,
     OPL7 = 329,
     OPR7 = 330,
     OPL8 = 331,
     OPR8 = 332,
     OPL9 = 333,
     OPR9 = 334
   };
#endif
#define keyAND 258
#define keyATTRIBUTE 259
#define keyATTRIBUTION 260
#define keyBEGIN 261
#define keyCASE 262
#define keyCIRCULAR 263
#define keyCOLLECTION 264
#define keyCONSTANT 265
#define keyCONSTRUCTOR 266
#define keyDECLARE 267
#define keyELSE 268
#define keyELSIF 269
#define keyEND 270
#define keyENDIF 271
#define keyEXTENDS 272
#define keyFOR 273
#define keyFUNCTION 274
#define keyGROUP 275
#define keyIF 276
#define keyIN 277
#define keyINFIX 278
#define keyINFIXL 279
#define keyINFIXR 280
#define keyINHERIT 281
#define keyINPUT 282
#define keyLANGUAGE 283
#define keyMATCH 284
#define keyMODULE 285
#define keyNOT 286
#define keyON 287
#define keyOR 288
#define keyPATTERN 289
#define keyPHYLUM 290
#define keyPRAGMA 291
#define keyPRIVATE 292
#define keyPROCEDURE 293
#define keyPUBLIC 294
#define keyREMOTE 295
#define keyRETURN 296
#define keySIGNATURE 297
#define keyTHEN 298
#define keyTYPE 299
#define keyUSE 300
#define keyVAR 301
#define keyWITH 302
#define DOTDOT 303
#define DOUBLECOLON 304
#define DOTDOTDOT 305
#define TRIPLECOLON 306
#define ARROW 307
#define IDENTIFIER 308
#define OPERATOR 309
#define INTEGER 310
#define STRING_CONSTANT 311
#define CHARACTER_CONSTANT 312
#define COLONEQ 313
#define OPL0 314
#define OPR0 315
#define OPL1 316
#define OPR1 317
#define OPL2 318
#define OPR2 319
#define OPL3 320
#define OPR3 321
#define OPL4 322
#define OPR4 323
#define OPL5 324
#define OPR5 325
#define OPL6 326
#define UNARY 327
#define OPR6 328
#define OPL7 329
#define OPR7 330
#define OPL8 331
#define OPR8 332
#define OPL9 333
#define OPR9 334




/* Copy the first part of user declarations.  */
#line 1 "aps.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 144 "aps.y"
typedef union YYSTYPE { /* two elements for each abstract non-terminal, exceptions noted*/
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
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 422 "aps.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 434 "aps.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2745

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  95
/* YYNRULES -- Number of rules. */
#define YYNRULES  367
/* YYNRULES -- Number of states. */
#define YYNSTATES  748

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   334

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    94,     2,     2,    92,     2,     2,     2,
      84,    85,     2,     2,    59,    74,    87,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    86,    83,
       2,    68,     2,    93,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    88,     2,    89,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,     2,    91,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    60,    61,    62,    63,    64,    65,
      66,    67,    69,    70,    71,    72,    73,    75,    76,    77,
      78,    79,    80,    81,    82
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     4,     7,     8,    11,    15,    19,    23,
      26,    28,    30,    34,    38,    42,    43,    45,    49,    50,
      53,    57,    61,    64,    66,    68,    70,    72,    74,    76,
      78,    80,    82,    84,    86,    88,    89,    93,    94,    98,
     101,   102,   106,   108,   115,   123,   130,   134,   135,   137,
     138,   141,   144,   145,   146,   151,   155,   161,   168,   175,
     179,   180,   184,   185,   189,   190,   194,   196,   201,   206,
     208,   213,   218,   223,   228,   229,   238,   239,   241,   243,
     245,   247,   249,   251,   253,   255,   257,   259,   263,   269,
     277,   285,   286,   289,   291,   301,   311,   322,   323,   325,
     327,   330,   332,   335,   337,   338,   340,   346,   347,   350,
     353,   357,   363,   365,   370,   377,   384,   392,   400,   405,
     406,   409,   410,   414,   415,   417,   421,   424,   426,   430,
     433,   437,   438,   441,   443,   447,   449,   452,   455,   458,
     462,   465,   469,   471,   475,   476,   478,   480,   484,   488,
     491,   493,   497,   501,   505,   507,   510,   514,   516,   520,
     523,   527,   529,   533,   539,   547,   551,   553,   557,   559,
     563,   565,   567,   571,   573,   577,   581,   584,   588,   594,
     600,   603,   608,   613,   618,   621,   623,   626,   628,   630,
     632,   636,   638,   642,   645,   649,   651,   655,   657,   661,
     667,   669,   673,   675,   677,   681,   683,   684,   687,   689,
     691,   694,   699,   707,   718,   729,   736,   742,   750,   758,
     763,   771,   778,   780,   781,   784,   790,   791,   794,   802,
     803,   806,   808,   811,   815,   819,   823,   827,   831,   835,
     839,   843,   847,   851,   855,   859,   863,   867,   871,   875,
     879,   883,   887,   891,   895,   899,   903,   907,   910,   914,
     918,   921,   925,   930,   932,   935,   938,   942,   944,   947,
     950,   953,   955,   957,   959,   963,   967,   970,   976,   981,
     985,   989,   992,   996,   998,  1002,  1004,  1006,  1010,  1012,
    1016,  1024,  1026,  1030,  1034,  1038,  1042,  1046,  1050,  1054,
    1058,  1062,  1066,  1070,  1074,  1078,  1082,  1086,  1090,  1094,
    1098,  1102,  1106,  1110,  1114,  1118,  1122,  1126,  1130,  1134,
    1138,  1142,  1146,  1150,  1154,  1158,  1162,  1166,  1170,  1174,
    1178,  1182,  1186,  1190,  1194,  1198,  1202,  1206,  1210,  1214,
    1218,  1222,  1226,  1230,  1234,  1238,  1242,  1246,  1250,  1254,
    1258,  1262,  1266,  1270,  1274,  1278,  1282,  1286,  1290,  1294,
    1298,  1302,  1306,  1310,  1314,  1316,  1318,  1320
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      96,     0,    -1,    -1,    97,    98,    -1,    -1,    98,    99,
      -1,    98,   100,    83,    -1,    98,    37,    83,    -1,    98,
      39,    83,    -1,    98,    83,    -1,   102,    -1,   104,    -1,
      23,   101,    54,    -1,    24,   101,    54,    -1,    25,   101,
      54,    -1,    -1,    55,    -1,    47,    56,    83,    -1,    -1,
     103,    83,    -1,   103,    37,    83,    -1,   103,    39,    83,
      -1,   103,   104,    -1,   108,    -1,   109,    -1,   115,    -1,
     138,    -1,   140,    -1,   139,    -1,   116,    -1,   127,    -1,
     128,    -1,   129,    -1,   131,    -1,   137,    -1,    -1,    37,
     105,   104,    -1,    -1,    39,   106,   104,    -1,    10,   104,
      -1,    -1,    46,   107,   104,    -1,     1,    -1,    42,   187,
     145,   110,   113,    83,    -1,    30,   187,   145,   155,   110,
     113,    83,    -1,    30,   187,   145,   110,   113,    83,    -1,
     111,   148,   112,    -1,    -1,   187,    -1,    -1,    68,   158,
      -1,    17,   158,    -1,    -1,    -1,     6,   114,   103,   188,
      -1,    36,   187,    83,    -1,    36,   187,    84,    85,    83,
      -1,    36,   187,    84,   185,    85,    83,    -1,    26,   158,
       6,   117,   188,    83,    -1,    26,   158,    83,    -1,    -1,
     117,   118,    83,    -1,    -1,    39,   119,   121,    -1,    -1,
      37,   120,   121,    -1,   121,    -1,   123,   187,    52,   179,
      -1,   125,   187,    52,   158,    -1,     1,    -1,   124,   187,
      68,   187,    -1,   123,   187,    68,   187,    -1,   125,   187,
      68,   187,    -1,   126,   187,    68,   187,    -1,    -1,   126,
     187,    68,   187,   122,     6,   117,   188,    -1,    -1,    46,
      -1,    19,    -1,    38,    -1,     4,    -1,    11,    -1,    34,
      -1,    44,    -1,    35,    -1,    30,    -1,    42,    -1,    35,
     187,    83,    -1,    35,   187,    68,   158,    83,    -1,    11,
     187,   144,   155,   141,   143,    83,    -1,    34,   187,   144,
     155,   141,   130,    83,    -1,    -1,    68,   163,    -1,    68,
      -1,   132,     4,   187,   144,   134,    86,   158,   135,    83,
      -1,   132,     4,   134,    87,   187,    86,   158,   135,    83,
      -1,   132,     4,   145,   134,    87,   187,    86,   158,   135,
      83,    -1,    -1,   133,    -1,    27,    -1,    27,     9,    -1,
       9,    -1,    27,     8,    -1,     8,    -1,    -1,   187,    -1,
      84,   187,    86,   187,    85,    -1,    -1,    58,   179,    -1,
      58,   136,    -1,   179,    59,   179,    -1,   179,    59,   179,
      59,   179,    -1,   177,    -1,    44,   187,   148,    83,    -1,
      44,   187,   148,    68,   158,    83,    -1,   132,   187,    86,
     158,   135,    83,    -1,    19,   187,   144,   155,   141,   143,
      83,    -1,    38,   187,   144,   155,   141,   143,    83,    -1,
     132,   111,   142,   135,    -1,    -1,    86,   158,    -1,    -1,
       6,   171,   188,    -1,    -1,   145,    -1,    88,   146,    89,
      -1,    88,    89,    -1,   147,    -1,   146,    83,   147,    -1,
     187,   148,    -1,   187,    59,   147,    -1,    -1,    49,   149,
      -1,   150,    -1,   149,    59,   150,    -1,   187,    -1,   187,
     151,    -1,    10,   150,    -1,    46,   150,    -1,    90,   153,
      91,    -1,    88,    89,    -1,    88,   152,    89,    -1,   158,
      -1,   152,    59,   158,    -1,    -1,   154,    -1,   158,    -1,
     154,    59,   158,    -1,    84,   156,    85,    -1,    84,    85,
      -1,   157,    -1,   156,    83,   157,    -1,   187,    86,   158,
      -1,   187,    59,   157,    -1,   159,    -1,    40,   158,    -1,
      19,   155,   142,    -1,   160,    -1,   159,    92,   187,    -1,
     159,   151,    -1,   159,   151,   182,    -1,   187,    -1,    90,
     158,    91,    -1,    90,   158,    59,   179,    91,    -1,    90,
     158,    59,   179,    59,   179,    91,    -1,    84,   158,    85,
      -1,   187,    -1,   161,    92,   187,    -1,   164,    -1,   164,
      21,   179,    -1,     1,    -1,   162,    -1,   163,    59,   162,
      -1,   165,    -1,   164,     3,   165,    -1,   164,    68,   165,
      -1,   166,   167,    -1,    90,   169,    91,    -1,    90,   169,
      91,    86,   158,    -1,   161,    92,    90,   169,    91,    -1,
      90,    91,    -1,    90,    91,    86,   158,    -1,   161,    92,
      90,    91,    -1,    93,   187,    86,   158,    -1,    93,   187,
      -1,    93,    -1,    94,   180,    -1,   187,    -1,    55,    -1,
      56,    -1,    84,   162,    85,    -1,   187,    -1,   161,    92,
     187,    -1,    84,    85,    -1,    84,   168,    85,    -1,   162,
      -1,   187,    58,   162,    -1,    50,    -1,   162,    59,   168,
      -1,   187,    58,   162,    59,   168,    -1,   170,    -1,   169,
      59,   170,    -1,   164,    -1,    50,    -1,    50,     3,   165,
      -1,   172,    -1,    -1,   172,   173,    -1,     1,    -1,    83,
      -1,   179,    83,    -1,   174,    58,   179,    83,    -1,    21,
     179,    43,   171,   175,   189,    83,    -1,    18,   187,    86,
     158,    22,   179,     6,   171,   188,    83,    -1,    18,   187,
      86,   158,    32,   179,     6,   171,   188,    83,    -1,   133,
     187,    86,   158,   135,    83,    -1,   187,    86,   158,   135,
      83,    -1,    46,   132,   187,    86,   158,   135,    83,    -1,
     133,    46,   187,    86,   158,   135,    83,    -1,     6,   171,
     188,    83,    -1,     7,   179,     6,   176,   178,   188,    83,
      -1,    18,   179,     6,   176,   188,    83,    -1,   179,    -1,
      -1,    13,   171,    -1,    14,   179,    43,   171,   175,    -1,
      -1,   176,   177,    -1,    29,   144,   162,     6,   171,   188,
      83,    -1,    -1,    13,   171,    -1,   180,    -1,   179,    50,
      -1,   179,    48,   179,    -1,   179,    60,   179,    -1,   179,
      61,   179,    -1,   179,    62,   179,    -1,   179,    63,   179,
      -1,   179,    64,   179,    -1,   179,    65,   179,    -1,   179,
      66,   179,    -1,   179,    67,   179,    -1,   179,    69,   179,
      -1,   179,    68,   179,    -1,   179,    70,   179,    -1,   179,
      71,   179,    -1,   179,    72,   179,    -1,   179,    73,   179,
      -1,   179,    76,   179,    -1,   179,    77,   179,    -1,   179,
      78,   179,    -1,   179,    79,   179,    -1,   179,    80,   179,
      -1,   179,    81,   179,    -1,   179,    82,   179,    -1,   179,
      54,   179,    -1,   179,    74,   179,    -1,    74,   179,    -1,
     179,     3,   179,    -1,   179,    33,   179,    -1,    31,   179,
      -1,   179,    22,   179,    -1,   179,    31,    22,   179,    -1,
     181,    -1,   180,   151,    -1,   180,   182,    -1,   180,    87,
     181,    -1,   187,    -1,    44,   187,    -1,    30,   187,    -1,
      42,   187,    -1,    55,    -1,    56,    -1,    57,    -1,    84,
     179,    85,    -1,    90,   185,    91,    -1,    90,    91,    -1,
     161,    92,    90,   185,    91,    -1,   161,    92,    90,    91,
      -1,   161,    92,   187,    -1,    84,     1,    85,    -1,    84,
      85,    -1,    84,   183,    85,    -1,   184,    -1,   183,    59,
     184,    -1,   179,    -1,   186,    -1,   185,    59,   186,    -1,
     179,    -1,   186,    21,   179,    -1,   186,    18,   187,    86,
     158,    22,   179,    -1,    53,    -1,    84,     3,    85,    -1,
      84,     4,    85,    -1,    84,     5,    85,    -1,    84,     6,
      85,    -1,    84,     7,    85,    -1,    84,     8,    85,    -1,
      84,     9,    85,    -1,    84,    10,    85,    -1,    84,    11,
      85,    -1,    84,    12,    85,    -1,    84,    13,    85,    -1,
      84,    14,    85,    -1,    84,    15,    85,    -1,    84,    16,
      85,    -1,    84,    17,    85,    -1,    84,    18,    85,    -1,
      84,    19,    85,    -1,    84,    20,    85,    -1,    84,    21,
      85,    -1,    84,    22,    85,    -1,    84,    23,    85,    -1,
      84,    24,    85,    -1,    84,    25,    85,    -1,    84,    26,
      85,    -1,    84,    27,    85,    -1,    84,    28,    85,    -1,
      84,    29,    85,    -1,    84,    30,    85,    -1,    84,    31,
      85,    -1,    84,    32,    85,    -1,    84,    33,    85,    -1,
      84,    34,    85,    -1,    84,    35,    85,    -1,    84,    36,
      85,    -1,    84,    37,    85,    -1,    84,    38,    85,    -1,
      84,    39,    85,    -1,    84,    40,    85,    -1,    84,    41,
      85,    -1,    84,    42,    85,    -1,    84,    43,    85,    -1,
      84,    44,    85,    -1,    84,    45,    85,    -1,    84,    46,
      85,    -1,    84,    47,    85,    -1,    84,    60,    85,    -1,
      84,    61,    85,    -1,    84,    62,    85,    -1,    84,    63,
      85,    -1,    84,    64,    85,    -1,    84,    65,    85,    -1,
      84,    66,    85,    -1,    84,    67,    85,    -1,    84,    69,
      85,    -1,    84,    70,    85,    -1,    84,    71,    85,    -1,
      84,    72,    85,    -1,    84,    73,    85,    -1,    84,    76,
      85,    -1,    84,    77,    85,    -1,    84,    78,    85,    -1,
      84,    79,    85,    -1,    84,    80,    85,    -1,    84,    81,
      85,    -1,    84,    82,    85,    -1,    84,    68,    85,    -1,
      84,    74,    85,    -1,    84,    54,    85,    -1,    90,    87,
      91,    -1,    84,    87,    85,    -1,    84,    48,    85,    -1,
      84,    50,    85,    -1,    15,    -1,    16,    -1,    16,    -1,
      15,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   317,   317,   317,   323,   324,   326,   328,   330,   332,
     336,   338,   342,   345,   347,   353,   354,   364,   369,   370,
     372,   374,   376,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   393,   395,   395,   397,
     399,   399,   401,   406,   412,   420,   431,   438,   439,   445,
     446,   448,   454,   455,   455,   464,   466,   468,   473,   477,
     483,   484,   489,   489,   491,   491,   493,   495,   497,   502,
     504,   508,   512,   516,   521,   521,   533,   534,   535,   536,
     537,   538,   542,   546,   547,   551,   552,   556,   560,   567,
     579,   587,   588,   590,   595,   599,   604,   613,   614,   618,
     620,   622,   624,   626,   631,   633,   635,   641,   642,   644,
     648,   650,   655,   660,   664,   670,   675,   679,   686,   694,
     695,   701,   702,   708,   709,   714,   716,   721,   723,   728,
     731,   740,   741,   746,   748,   753,   755,   757,   759,   761,
     766,   768,   773,   775,   781,   782,   787,   789,   794,   796,
     801,   803,   807,   809,   815,   817,   821,   831,   832,   834,
     836,   842,   847,   849,   851,   853,   861,   863,   867,   868,
     870,   875,   877,   881,   882,   884,   889,   891,   893,   895,
     897,   899,   901,   903,   905,   907,   913,   923,   924,   926,
     928,   933,   935,   940,   942,   947,   949,   952,   956,   958,
     964,   966,   971,   973,   975,   979,   984,   985,   989,   991,
     992,   994,   999,  1001,  1003,  1009,  1011,  1014,  1016,  1018,
    1020,  1022,  1026,  1037,  1038,  1040,  1045,  1046,  1050,  1055,
    1056,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,
    1089,  1090,  1091,  1105,  1106,  1108,  1110,  1117,  1118,  1119,
    1120,  1135,  1136,  1137,  1138,  1139,  1141,  1146,  1148,  1150,
    1152,  1155,  1157,  1162,  1164,  1168,  1173,  1175,  1180,  1181,
    1183,  1188,  1190,  1192,  1194,  1196,  1198,  1200,  1202,  1204,
    1206,  1208,  1210,  1212,  1214,  1216,  1218,  1220,  1224,  1228,
    1230,  1232,  1234,  1236,  1238,  1240,  1242,  1246,  1250,  1252,
    1254,  1256,  1258,  1260,  1262,  1264,  1266,  1268,  1270,  1272,
    1274,  1278,  1282,  1284,  1286,  1288,  1290,  1292,  1294,  1296,
    1298,  1300,  1302,  1304,  1306,  1308,  1310,  1312,  1314,  1316,
    1318,  1320,  1322,  1324,  1326,  1328,  1330,  1332,  1334,  1336,
    1338,  1343,  1345,  1347,  1352,  1353,  1357,  1358
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "keyAND", "keyATTRIBUTE", "keyATTRIBUTION", 
  "keyBEGIN", "keyCASE", "keyCIRCULAR", "keyCOLLECTION", "keyCONSTANT", 
  "keyCONSTRUCTOR", "keyDECLARE", "keyELSE", "keyELSIF", "keyEND", 
  "keyENDIF", "keyEXTENDS", "keyFOR", "keyFUNCTION", "keyGROUP", "keyIF", 
  "keyIN", "keyINFIX", "keyINFIXL", "keyINFIXR", "keyINHERIT", "keyINPUT", 
  "keyLANGUAGE", "keyMATCH", "keyMODULE", "keyNOT", "keyON", "keyOR", 
  "keyPATTERN", "keyPHYLUM", "keyPRAGMA", "keyPRIVATE", "keyPROCEDURE", 
  "keyPUBLIC", "keyREMOTE", "keyRETURN", "keySIGNATURE", "keyTHEN", 
  "keyTYPE", "keyUSE", "keyVAR", "keyWITH", "DOTDOT", "DOUBLECOLON", 
  "DOTDOTDOT", "TRIPLECOLON", "ARROW", "IDENTIFIER", "OPERATOR", 
  "INTEGER", "STRING_CONSTANT", "CHARACTER_CONSTANT", "COLONEQ", "','", 
  "OPL0", "OPR0", "OPL1", "OPR1", "OPL2", "OPR2", "OPL3", "OPR3", "'='", 
  "OPL4", "OPR4", "OPL5", "OPR5", "OPL6", "'-'", "UNARY", "OPR6", "OPL7", 
  "OPR7", "OPL8", "OPR8", "OPL9", "OPR9", "';'", "'('", "')'", "':'", 
  "'.'", "'['", "']'", "'{'", "'}'", "'$'", "'?'", "'!'", "$accept", 
  "program", "@1", "unit_list", "unit", "fixity", "optional_integer", 
  "with", "decl_list", "decl", "@2", "@3", "@4", "signature_decl", 
  "module_decl", "result_type", "opt_id", "opt_extension", "module_body", 
  "@5", "pragma_decl", "inherit_decl", "renamings", "renaming", "@6", 
  "@7", "base_renaming", "@8", "value_keyword", "pattern_keyword", 
  "type_keyword", "signature_keyword", "phy_decl", "con_decl", "pat_decl", 
  "pattern_body", "attribute_decl", "opt_direction", "direction", 
  "opt_formal", "default", "lattice", "match_decl", "type_decl", 
  "var_decl", "function_decl", "result_value", "opt_typing", "opt_body", 
  "opt_polymorphism", "polymorphism", "type_formals", "type_formal", 
  "opt_signatures", "signatures", "signature", "type_parameters", 
  "type_actuals", "types", "non_empty_types", "formals", "formal_list", 
  "formal", "type", "simple_type", "atomic_type", "type_prefix", 
  "Pattern", "choices", "pattern", "simple_pattern", "constructor_name", 
  "simple_pattern_args", "non_empty_pattern_args", 
  "sequence_pattern_args", "sequence_pattern", "stmts", "stmt_list", 
  "stmt", "lhs", "stmt_elsif_part", "stmt_match_list", "match", 
  "stmt_match_default", "expr", "simple_expr", "atomic_expr", "actuals", 
  "non_empty_actuals", "actual", "sequence_actuals", "sequence_actual", 
  "id", "iskeyEND", "iskeyENDIF", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,    44,
     314,   315,   316,   317,   318,   319,   320,   321,    61,   322,
     323,   324,   325,   326,    45,   327,   328,   329,   330,   331,
     332,   333,   334,    59,    40,    41,    58,    46,    91,    93,
     123,   125,    36,    63,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    95,    97,    96,    98,    98,    98,    98,    98,    98,
      99,    99,   100,   100,   100,   101,   101,   102,   103,   103,
     103,   103,   103,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   105,   104,   106,   104,   104,
     107,   104,   104,   108,   109,   109,   110,   111,   111,   112,
     112,   112,   113,   114,   113,   115,   115,   115,   116,   116,
     117,   117,   119,   118,   120,   118,   118,   118,   118,   121,
     121,   121,   121,   121,   122,   121,   123,   123,   123,   123,
     123,   123,   124,   125,   125,   126,   126,   127,   127,   128,
     129,   130,   130,   130,   131,   131,   131,   132,   132,   133,
     133,   133,   133,   133,   134,   134,   134,   135,   135,   135,
     136,   136,   137,   138,   138,   139,   140,   140,   141,   142,
     142,   143,   143,   144,   144,   145,   145,   146,   146,   147,
     147,   148,   148,   149,   149,   150,   150,   150,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   154,   155,   155,
     156,   156,   157,   157,   158,   158,   158,   159,   159,   159,
     159,   160,   160,   160,   160,   160,   161,   161,   162,   162,
     162,   163,   163,   164,   164,   164,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   166,   166,   167,   167,   168,   168,   168,   168,   168,
     169,   169,   170,   170,   170,   171,   172,   172,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   174,   175,   175,   175,   176,   176,   177,   178,
     178,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   180,   180,   180,   180,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   182,   182,   183,   183,   184,   185,   185,   186,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   188,   189,   189
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     2,     0,     2,     3,     3,     3,     2,
       1,     1,     3,     3,     3,     0,     1,     3,     0,     2,
       3,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     0,     3,     2,
       0,     3,     1,     6,     7,     6,     3,     0,     1,     0,
       2,     2,     0,     0,     4,     3,     5,     6,     6,     3,
       0,     3,     0,     3,     0,     3,     1,     4,     4,     1,
       4,     4,     4,     4,     0,     8,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     5,     7,
       7,     0,     2,     1,     9,     9,    10,     0,     1,     1,
       2,     1,     2,     1,     0,     1,     5,     0,     2,     2,
       3,     5,     1,     4,     6,     6,     7,     7,     4,     0,
       2,     0,     3,     0,     1,     3,     2,     1,     3,     2,
       3,     0,     2,     1,     3,     1,     2,     2,     2,     3,
       2,     3,     1,     3,     0,     1,     1,     3,     3,     2,
       1,     3,     3,     3,     1,     2,     3,     1,     3,     2,
       3,     1,     3,     5,     7,     3,     1,     3,     1,     3,
       1,     1,     3,     1,     3,     3,     2,     3,     5,     5,
       2,     4,     4,     4,     2,     1,     2,     1,     1,     1,
       3,     1,     3,     2,     3,     1,     3,     1,     3,     5,
       1,     3,     1,     1,     3,     1,     0,     2,     1,     1,
       2,     4,     7,    10,    10,     6,     5,     7,     7,     4,
       7,     6,     1,     0,     2,     5,     0,     2,     7,     0,
       2,     1,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     3,
       2,     3,     4,     1,     2,     2,     3,     1,     2,     2,
       2,     1,     1,     1,     3,     3,     2,     5,     4,     3,
       3,     2,     3,     1,     3,     1,     1,     3,     1,     3,
       7,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       2,     0,     4,     1,     0,    42,   103,   101,     0,     0,
       0,    15,    15,    15,     0,    99,   123,     0,     0,     0,
       0,    35,     0,    37,     0,     0,    40,     0,     9,     5,
       0,    10,    11,    23,    24,    25,    29,    30,    31,    32,
      33,     0,    98,    34,    26,    28,    27,   112,    35,    37,
      39,   291,     0,     0,   123,   123,    16,     0,     0,     0,
       0,     0,     0,     0,     0,   154,   157,   161,   102,   100,
       0,     0,   124,     0,   123,     0,     0,     7,     0,   123,
       8,     0,     0,   131,     0,     0,     6,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    12,    13,    14,     0,   119,   155,     0,
       0,     0,     0,    60,    59,     0,     0,   159,   126,     0,
     127,   131,   170,   188,   189,     0,     0,   185,     0,     0,
       0,   168,   173,     0,   187,    47,     0,     0,    87,    55,
       0,    36,     0,    38,    47,     0,     0,    41,    17,     0,
       0,   104,   123,     0,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   362,
     363,   359,   337,   338,   339,   340,   341,   342,   343,   344,
     357,   345,   346,   347,   348,   349,   358,   350,   351,   352,
     353,   354,   355,   356,   361,   360,    97,    97,   149,     0,
     150,     0,     0,   156,   165,     0,   162,     0,   140,     0,
     142,   158,     0,   160,     0,   125,     0,   129,     0,   203,
     180,   202,     0,   200,   184,     0,     0,     0,   271,   272,
     273,     0,     0,     0,   186,   263,   267,     0,   206,     0,
       0,     0,     0,   176,     0,    52,   131,    47,    48,    97,
       0,     0,     0,     0,   288,   231,     0,   286,    97,    52,
       0,     0,   144,   132,   133,   135,     0,   113,     0,     0,
       0,   105,   104,   107,    47,   121,   121,     0,   148,     0,
       0,   120,     0,    69,    80,    81,   364,   365,    78,    85,
      82,    84,    64,    79,    62,    86,    83,    77,     0,    66,
       0,     0,     0,     0,     0,     0,   141,   281,   285,     0,
     283,   128,   130,   190,     0,     0,     0,   177,     0,   269,
     270,   268,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,   264,   265,     0,   167,     0,     0,   174,
     169,   175,   197,   193,   195,     0,   187,    53,     0,    49,
      52,    91,    88,   260,   257,    56,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
       0,   137,   138,     0,   145,   146,     0,   136,     0,     0,
       0,     0,     0,     0,     0,   119,   206,     0,     0,   151,
     153,   152,     0,   163,     0,     0,    61,     0,     0,     0,
       0,    58,   143,     0,   282,   204,   181,   201,     0,   183,
     280,   274,   275,     0,   279,   266,   182,     0,     0,   208,
     206,     0,     0,     0,    97,   209,     0,   207,     0,   222,
     267,     0,   194,     0,    18,    45,     0,     0,    46,     0,
       0,     0,   258,   261,     0,   259,   233,   255,   234,   235,
     236,   237,   238,   239,   240,   241,   243,   242,   244,   245,
     246,   247,   256,   248,   249,   250,   251,   252,   253,   254,
     287,    57,     0,   289,     0,    43,   139,     0,   134,   114,
       0,     0,     0,     0,   109,   108,   115,   107,     0,    89,
     116,     0,    65,     0,     0,    63,     0,     0,     0,     0,
       0,     0,   284,   178,   278,     0,   179,   228,     0,     0,
       0,   267,     0,     0,     0,     0,     0,   210,     0,   198,
     196,     0,    51,    50,    44,   171,    92,    90,   262,     0,
     117,   147,   106,   107,     0,   107,     0,   118,   122,   164,
       0,     0,    67,    71,    70,    68,    72,    73,   277,     0,
     226,   226,     0,   206,     0,     0,     0,     0,   107,     0,
      35,    37,    19,    22,    54,     0,     0,     0,   107,     0,
     110,     0,   219,   229,     0,     0,   223,     0,     0,   107,
     211,     0,   199,    20,    21,   172,     0,    95,     0,    94,
       0,    60,   206,   227,     0,     0,     0,     0,   206,     0,
       0,   107,   107,     0,   216,   290,    96,   111,     0,   230,
       0,   221,     0,     0,   224,     0,   367,   366,     0,     0,
       0,   215,    75,   220,   206,   206,   206,   212,   217,   218,
       0,     0,   223,     0,     0,   225,   213,   214
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,     4,    29,    30,    57,    31,   631,    32,
      78,    81,    84,    33,    34,   335,   336,   548,   438,   544,
      35,    36,   297,   388,   505,   504,   389,   681,   390,   391,
     392,   393,    37,    38,    39,   551,    40,    41,    42,   210,
     494,   594,    43,    44,    45,    46,   365,   293,   497,    71,
      72,   179,   180,   206,   353,   354,   423,   299,   483,   484,
     167,   289,   290,   168,    65,    66,   323,   434,   636,   191,
     192,   193,   333,   435,   312,   313,   427,   428,   537,   538,
     710,   683,    47,   704,   344,   345,   325,   424,   399,   400,
     346,   347,   326,   394,   728
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -563
static const short yypact[] =
{
    -563,    51,  -563,  -563,   413,  -563,  -563,  -563,   912,   180,
     180,    14,    14,    14,   154,   337,     5,   180,   180,   180,
     180,    62,   180,    71,   180,   180,  -563,   109,  -563,  -563,
     104,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,    35,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  1656,   119,     5,     5,  -563,   186,   213,   255,
     174,   154,  1480,    15,    24,   129,  -563,  -563,  -563,  -563,
     130,   155,  -563,     5,     5,    60,   273,  -563,   912,     5,
    -563,   912,     5,   251,   912,   234,  -563,   140,   258,   247,
     264,   274,   289,   305,   309,   315,   318,   320,   333,   340,
     345,   359,   368,   371,   373,   380,   394,   396,   399,   402,
     405,   407,   414,   419,   420,   423,   425,   433,   440,   444,
     445,   446,   448,   451,   454,   455,   459,   460,   462,   468,
     469,   470,   472,   473,   476,   477,   478,   479,   480,   483,
     484,   485,   486,   488,   489,   490,   491,   494,   500,   503,
     506,   507,   508,   511,   512,   518,   519,   522,   523,   524,
     355,   174,   174,  -563,  -563,  -563,    78,   390,  -563,   280,
     178,   526,   -15,  -563,  -563,   163,   180,   497,  -563,   172,
    -563,   177,  -563,  -563,  -563,  1041,   246,   180,   458,   475,
     537,    28,  -563,   528,   -44,   197,   174,   154,  -563,  -563,
     229,  -563,   174,  -563,   180,    19,    96,  -563,  -563,  1568,
     529,   221,   284,   154,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,    55,    55,  -563,   116,
    -563,     1,   154,  -563,  -563,   336,  -563,  1217,  -563,   -23,
    -563,  -563,  1218,  -563,   180,  -563,   180,  -563,   533,   610,
     534,    47,    26,  -563,   535,   180,   180,   180,  -563,  -563,
    -563,  1135,   913,   530,   314,  -563,   532,   237,  -563,   466,
     336,   466,    45,  -563,  1392,   619,   251,   180,  -563,    55,
     543,   336,   336,   545,  2346,   314,    -1,   233,    55,   619,
      19,    19,    15,   572,  -563,   544,   154,  -563,   547,   180,
     550,  -563,   221,   576,   180,   632,   632,   180,  -563,   180,
     154,  -563,  1250,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,   556,  -563,
     180,   180,   180,   180,   557,   154,  -563,  -563,  2346,    27,
    -563,  -563,  -563,  -563,   466,   154,   681,   555,   154,  -563,
    -563,  -563,   558,   204,  2558,   278,   285,  2565,  1709,  -563,
      67,   295,   458,  -563,  -563,   496,   560,   361,   843,  -563,
    2346,  -563,  -563,  -563,   583,   562,   -17,  -563,   565,    21,
     619,   577,  -563,  2452,   678,  -563,   336,   336,   627,   336,
     336,  -563,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   568,   180,   336,   632,
     571,  -563,  -563,   567,   597,  -563,    19,  -563,   578,   180,
     573,   180,   579,   336,   580,   390,  -563,   584,   586,  -563,
    -563,  -563,   336,  -563,   482,   482,  -563,    42,   594,    85,
     602,  -563,  -563,   336,  -563,  -563,  -563,  -563,   154,  -563,
    -563,  -563,  -563,   933,   581,  -563,  -563,    81,   588,  -563,
    -563,   336,   336,   336,    55,  -563,   145,  -563,   614,  1762,
     183,    58,  -563,   155,  -563,  -563,   154,   154,  -563,   591,
      91,   599,  2399,  2452,   336,  2505,  2597,    54,  2597,  2597,
    2630,  2630,  2663,  2663,   761,   761,  1912,  1912,  1912,   829,
     829,   678,   678,   678,   391,   391,   381,   381,    54,    54,
     233,  -563,   600,  2346,   609,  -563,  -563,   154,  -563,  -563,
     592,   154,   607,   154,  -563,  1868,  -563,   576,   361,  -563,
    -563,  1303,  -563,   180,   180,  -563,   336,   180,   180,   154,
     180,   180,  -563,  -563,  -563,   131,  -563,  -563,   361,  1948,
    2001,   218,  2054,   180,   180,   608,   336,  -563,   154,  -563,
     621,   680,  -563,  -563,  -563,  -563,   624,  -563,  2452,   154,
    -563,  -563,  -563,   576,   154,   576,   336,  -563,  -563,  -563,
     617,   629,  2346,  -563,  -563,  -563,  -563,   694,  -563,   618,
    -563,  -563,   154,  -563,   616,   622,   154,  1815,   576,    58,
     628,   630,  -563,  -563,  -563,   155,   682,   637,   576,   640,
    2107,   699,  -563,   176,   300,   244,   374,   154,   154,   576,
    -563,   644,  -563,  -563,  -563,  -563,   336,  -563,   646,  -563,
     336,  -563,  -563,  -563,   361,   647,   336,   336,  -563,   336,
     393,   576,   576,   652,  -563,  2346,  -563,  2346,  1217,  -563,
     655,  -563,  2187,  2240,  -563,  2293,  -563,  -563,   656,   657,
     658,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
     361,   361,   374,   659,   660,  -563,  -563,  -563
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -563,  -563,  -563,  -563,  -563,  -563,   415,  -563,  -563,    -4,
    -563,  -563,  -563,  -563,  -563,  -178,   348,  -563,  -316,  -563,
    -563,  -563,    43,  -563,  -563,  -563,   -71,  -563,   -30,  -563,
     -10,  -563,  -563,  -563,  -563,  -563,  -563,  -269,   317,  -191,
    -562,  -563,  -563,  -563,  -563,  -563,  -216,   254,  -338,   -32,
     150,  -563,   -13,  -167,  -563,  -326,   -63,  -563,  -563,  -563,
    -105,  -563,   -43,    59,  -563,  -563,   -52,   -65,  -563,  -179,
    -286,  -563,  -563,  -514,   325,   347,  -493,  -563,  -563,  -563,
      25,   100,  -177,  -563,   334,   585,   344,   595,  -563,   256,
    -310,   293,    -9,  -406,  -563
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -206
static const short yytable[] =
{
      54,    55,   177,   598,    50,    67,   190,   311,    73,    74,
      75,    76,   420,    79,   307,    82,    83,   364,   364,   189,
     360,   528,   161,   162,   481,   482,   349,   629,   498,   350,
     173,   329,    88,   480,    60,   647,   395,   618,   546,    87,
    -191,   543,   196,   429,   295,   431,   182,   202,  -166,   330,
     329,     3,    67,    67,    67,    61,   286,   287,   475,   182,
     369,   181,   194,     6,     7,   351,   396,  -191,    51,    56,
     364,   366,    51,    64,   201,  -166,   296,   203,   212,   364,
     207,   677,    15,   679,   476,   406,   513,   370,    51,   547,
     337,   339,   182,    70,   606,   432,   331,   348,    51,    62,
     183,   184,   160,    52,   451,    63,   691,   174,   432,   352,
     607,    51,   514,   183,   184,   331,   698,   407,   515,    52,
     308,   171,   172,   441,   549,    53,   475,   713,   197,   185,
     433,    51,   479,   189,   189,   186,   474,   609,   187,   188,
     406,   584,   185,   198,    51,    77,   183,   184,   186,   729,
     730,   187,   188,   610,    80,   692,   182,   291,   522,   440,
     588,    67,    52,   288,   356,    85,    67,   301,    53,   439,
     686,   492,   616,    60,   -93,   185,   194,   194,   314,   357,
     362,   186,    60,    51,   187,   188,   338,    86,    67,   702,
     475,   624,   648,    51,    61,   338,   355,    60,    51,   367,
     358,   368,   361,    61,    67,    16,   160,    51,    51,   719,
     183,   184,   659,   615,    52,   724,    51,   175,    61,   178,
      53,   176,   658,   195,   209,   674,   205,   311,    70,    52,
      53,    51,   204,    51,   300,    53,   306,   211,    62,   185,
     163,   740,   741,   742,    63,   186,   311,    62,   187,   188,
      51,   477,   298,    63,   478,   304,   340,    51,   166,   315,
     341,   305,    62,   251,    52,   623,   706,   164,    63,   628,
      53,   316,   363,   317,    51,  -166,   707,   189,   705,   189,
     189,   334,    51,    67,   318,   319,   320,    53,    52,   241,
      51,   401,   487,   402,    53,   181,   309,   181,   720,    51,
     205,   183,   184,   342,   662,   209,   409,   410,   411,   165,
    -166,    53,   732,   321,   343,   376,   377,   208,   426,   322,
     194,    52,   194,   436,   499,   291,   500,   425,   338,    16,
     185,    51,   214,   160,   743,   744,   186,   310,    51,   187,
     188,   355,   355,    67,   213,    68,    69,    67,    51,   215,
     490,   371,   189,   361,   189,   338,   199,   200,   291,   216,
     291,    67,    52,   253,   166,   230,   315,   341,    53,    52,
     255,  -105,    70,   189,   217,    53,   376,   377,   316,    52,
     317,   507,   508,   509,   510,   523,    67,   708,   709,    51,
     218,   318,   319,   320,   219,   194,    67,   194,   302,    67,
     220,   422,   175,   221,   409,   222,   410,   411,   726,   727,
     342,   485,   524,    -3,     5,   488,   194,   -97,   223,   540,
     321,     6,     7,     8,     9,   224,   322,    58,    59,   501,
     225,   451,    10,   602,   605,   452,    11,    12,    13,    14,
      15,   451,    16,    17,   226,   452,   285,    18,    19,    20,
      21,    22,    23,   227,   512,    24,   228,    25,   229,    26,
      27,   472,   473,   474,   516,   230,   -97,   519,   582,   470,
     471,   472,   473,   474,   603,   603,   292,   355,   630,   231,
     590,   232,   592,   373,   233,   635,   374,   234,   315,   189,
     235,   189,   236,   375,   604,   604,    28,   -97,   189,   237,
     316,   378,   317,   -97,   238,   239,   703,   703,   240,    67,
     241,    51,   379,   318,   319,   320,   380,   381,   242,    51,
     383,   183,   184,   621,   385,   243,   386,   625,   387,   244,
     245,   246,   436,   247,   194,   -76,   248,    67,    67,   249,
     250,   194,   321,   328,   251,   252,   309,   253,   322,    51,
     185,   183,   184,   254,   255,   256,   186,   257,   258,   187,
     188,   259,   260,   261,   262,   263,   -76,   327,   264,   265,
     266,   267,   -76,   268,   269,   270,   271,   613,    67,   272,
     185,   302,    67,   160,    67,   273,   186,   526,   274,   187,
     188,   275,   276,   277,   650,   651,   278,   279,   653,   654,
      67,   656,   657,   280,   281,   632,   633,   282,   283,   284,
     695,   294,   332,   404,   664,   665,   359,   189,   403,    67,
     405,   408,   421,   189,  -166,   437,   442,   673,   445,   372,
      67,   486,   175,   489,   493,    67,   398,   491,   496,   506,
     511,   518,   541,   520,  -192,   550,   641,   542,   545,   554,
     643,   581,   645,    67,   585,   418,   587,    67,   586,   591,
     436,   589,   608,   596,   430,   593,   194,   599,   655,   600,
     611,   617,   626,  -167,   634,   443,   444,   642,    67,    67,
     669,     5,   637,   675,   -97,   607,   639,   668,     6,     7,
       8,     9,   640,   644,   666,   376,   377,   610,   676,    10,
     -74,   682,   687,   678,   696,   701,    14,    15,   688,    16,
      17,   693,   495,   694,    18,    19,    20,   670,    22,   671,
     697,   685,    24,   699,    25,   689,    26,   714,   451,   716,
     721,   309,   452,   -97,    51,   731,   183,   184,   733,   737,
     738,   739,   746,   747,   718,   536,   711,   712,   443,   597,
     527,   444,   467,   517,   468,   469,   470,   471,   472,   473,
     474,   684,   539,   672,   -97,   185,   525,   745,   580,   612,
     -97,   186,   303,   324,   187,   188,     0,     0,     0,     0,
     552,   553,     0,   555,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,     0,
       0,   451,   583,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   595,   460,   461,
     462,   463,   464,   465,   466,   467,   601,   468,   469,   470,
     471,   472,   473,   474,   529,     0,     0,   398,     0,   530,
     531,     6,     7,     0,     0,     0,  -205,  -205,  -205,  -205,
       0,   532,     0,     0,   533,   619,   620,   622,     0,     0,
      15,     0,     0,   315,   341,     0,     0,     0,     0,   451,
       0,     0,     0,   452,     0,   316,     0,   317,   638,   534,
       0,     0,     0,     0,     0,     0,    51,     0,   318,   319,
     320,   465,   466,   467,     0,   468,   469,   470,   471,   472,
     473,   474,     0,     5,     0,     0,   -97,   342,     0,     0,
       6,     7,     8,     9,     0,     0,   535,   321,     0,     0,
       0,    10,     0,   322,     0,     0,     0,     0,    14,    15,
     652,    16,    17,   315,   341,     0,    18,    19,    20,    48,
      22,    49,     0,     0,    24,   316,    25,   317,    26,     0,
     667,     0,     0,   315,   341,   -97,    51,     0,   318,   319,
     320,     0,     0,     0,     0,   316,     0,   317,     0,     0,
     680,     0,     0,     0,     0,     0,    51,   342,   318,   319,
     320,     0,     0,     0,     0,     0,   -97,   321,     0,     0,
     160,     0,   -97,   322,   419,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
     160,     0,     0,   322,   614,     0,     0,     0,     0,     0,
     715,     0,     0,     0,   717,     0,     0,     0,     0,     0,
     722,   723,   182,   725,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,   135,     0,     0,    51,   136,   183,   184,     0,     0,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,     0,   152,   153,   154,
     155,   156,   157,   158,     0,   185,     0,     0,   159,     0,
       0,   186,     0,     0,   187,   188,   412,     0,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   413,   414,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   415,   129,   416,
     131,   132,   133,   134,     0,   135,     0,     0,    51,   136,
     318,   319,   320,     0,     0,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   417,
       0,   152,   153,   154,   155,   156,   157,   158,   373,   321,
       0,   374,   159,     0,     0,   322,     0,     0,   375,     0,
       0,     0,   376,   377,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   315,   341,
       0,   380,   381,   446,   382,   383,   384,     0,     0,   385,
     316,   386,   317,   387,     0,     0,     0,     0,     0,     0,
     -76,    51,   447,   318,   319,   320,     0,     0,     0,     0,
       0,   448,     0,   449,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,   450,     0,
     451,   -76,   321,   397,   452,     0,   446,   -76,   322,   502,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   447,   468,   469,   470,   471,
     472,   473,   474,     0,   448,     0,   449,     0,     0,     0,
       0,   503,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,     0,   451,     0,     0,     0,   452,     0,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,   468,
     469,   470,   471,   472,   473,   474,     0,     0,     0,     0,
       0,     0,     0,     0,   649,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,     0,   135,     0,     0,    51,   136,     0,     0,     0,
       0,     0,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,     0,   152,   153,
     154,   155,   156,   157,   158,     0,    52,   288,     0,   159,
       0,     0,    53,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   169,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     170,   127,   128,   129,   130,   131,   132,   133,   134,     0,
     135,     0,     0,    51,   136,     0,     0,     0,     0,     0,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,     0,   152,   153,   154,   155,
     156,   157,   158,     0,    62,     0,     0,   159,     0,     0,
      63,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   135,     0,
       0,    51,   136,     0,     0,     0,     0,     0,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,     0,   152,   153,   154,   155,   156,   157,
     158,     0,    52,     0,     0,   159,     0,     0,    53,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,     0,   135,     0,     0,     0,
     136,     0,   446,     0,     0,     0,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   447,   152,   153,   154,   155,   156,   157,   158,     0,
     448,     0,   449,   159,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   450,     0,   451,
       0,     0,     0,   452,     0,   446,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   447,   468,   469,   470,   471,   472,
     473,   474,     0,   448,   521,   449,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,     0,   451,     0,     0,     0,   452,     0,   446,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   447,   468,   469,
     470,   471,   472,   473,   474,   627,   448,     0,   449,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,     0,   451,     0,     0,     0,   452,
       0,   446,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     447,   468,   469,   470,   471,   472,   473,   474,   690,   448,
       0,   449,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   450,     0,   451,     0,
       0,     0,   452,     0,     0,     0,     0,   646,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,   468,   469,   470,   471,   472,   473,
     474,   446,     0,     0,   660,     0,     0,     0,     0,     0,
       0,     0,   451,     0,     0,     0,   452,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,     0,   448,
       0,   449,   463,   464,   465,   466,   467,     0,   468,   469,
     470,   471,   472,   473,   474,     0,   450,     0,   451,     0,
       0,     0,   452,     0,   446,     0,     0,   661,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   447,   468,   469,   470,   471,   472,   473,
     474,     0,   448,     0,   449,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   450,
       0,   451,     0,     0,     0,   452,     0,   446,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   447,   468,   469,   470,
     471,   472,   473,   474,     0,   448,     0,   449,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   663,     0,     0,
       0,     0,   450,     0,   451,     0,     0,     0,   452,     0,
     446,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   447,
     468,   469,   470,   471,   472,   473,   474,     0,   448,     0,
     449,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   450,     0,   451,     0,     0,
       0,   452,     0,     0,     0,     0,   700,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,   468,   469,   470,   471,   472,   473,   474,
     446,     0,     0,   734,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,     0,   448,     0,
     449,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   450,     0,   451,     0,     0,
       0,   452,     0,   446,     0,     0,   735,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   447,   468,   469,   470,   471,   472,   473,   474,
       0,   448,     0,   449,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,     0,
     451,     0,     0,     0,   452,     0,   446,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   447,   468,   469,   470,   471,
     472,   473,   474,     0,   448,     0,   449,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   736,     0,     0,     0,
       0,   450,     0,   451,     0,     0,     0,   452,     0,   446,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   447,   468,
     469,   470,   471,   472,   473,   474,     0,   448,     0,   449,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   450,     0,   451,     0,     0,     0,
     452,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   447,   468,   469,   470,   471,   472,   473,   474,     0,
     448,     0,   449,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   450,     0,   451,
       0,     0,     0,   452,     0,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,  -206,   468,   469,   470,   471,   472,
     473,   474,     0,  -206,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,     0,   451,     0,     0,     0,   452,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   447,   468,   469,
     470,   471,   472,   473,   474,     0,   448,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,     0,   451,     0,     0,     0,   452,
       0,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
       0,   468,   469,   470,   471,   472,   473,   474,   315,   341,
       0,     0,     0,     0,     0,   315,   341,     0,     0,     0,
     316,     0,   317,     0,     0,     0,     0,   316,     0,   317,
       0,    51,     0,   318,   319,   320,     0,     0,    51,     0,
     318,   319,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,   342,
       0,     0,   321,   242,     0,     0,     0,   451,   322,   321,
     276,   452,     0,     0,     0,   322,     0,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,   468,   469,   470,   471,   472,   473,   474,
     451,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,   468,   469,   470,   471,
     472,   473,   474,   451,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,   468,
     469,   470,   471,   472,   473,   474
};

static const short yycheck[] =
{
       9,    10,    65,   496,     8,    14,    71,   186,    17,    18,
      19,    20,   322,    22,   181,    24,    25,   286,   287,    71,
     211,   427,    54,    55,   350,   351,   204,   541,   366,    10,
       6,     3,    41,   349,    19,   597,    59,   530,    17,     4,
      84,    58,    74,   329,    59,   331,     1,    79,    92,    21,
       3,     0,    61,    62,    63,    40,   161,   162,    59,     1,
      59,    70,    71,     8,     9,    46,    89,    84,    53,    55,
     339,   287,    53,    14,    78,    92,    91,    81,    87,   348,
      84,   643,    27,   645,    85,    59,    59,    86,    53,    68,
     195,   196,     1,    88,    52,    50,    68,   202,    53,    84,
      55,    56,    87,    84,    50,    90,   668,    83,    50,    90,
      68,    53,    85,    55,    56,    68,   678,    91,   404,    84,
     185,    62,    63,   339,   440,    90,    59,   689,    68,    84,
      85,    53,   348,   185,   186,    90,    82,    52,    93,    94,
      59,   479,    84,    83,    53,    83,    55,    56,    90,   711,
     712,    93,    94,    68,    83,   669,     1,   166,    91,   337,
     486,   170,    84,    85,    68,    56,   175,   176,    90,   336,
     663,   362,    91,    19,    83,    84,   185,   186,   187,    83,
     212,    90,    19,    53,    93,    94,   195,    83,   197,    13,
      59,    46,   598,    53,    40,   204,   205,    19,    53,    83,
     209,    85,   211,    40,   213,    29,    87,    53,    53,   702,
      55,    56,   618,   523,    84,   708,    53,    88,    40,    89,
      90,    92,    91,    73,    84,   631,    49,   406,    88,    84,
      90,    53,    82,    53,   175,    90,    59,    87,    84,    84,
      54,   734,   735,   736,    90,    90,   425,    84,    93,    94,
      53,    18,    89,    90,    21,    83,   197,    53,    84,    30,
      31,    89,    84,    85,    84,   534,    22,    54,    90,    86,
      90,    42,   213,    44,    53,    92,    32,   329,   684,   331,
     332,    84,    53,   292,    55,    56,    57,    90,    84,    85,
      53,   304,   355,   306,    90,   304,    50,   306,   704,    53,
      49,    55,    56,    74,    86,    84,   315,   316,   317,    54,
      92,    90,   718,    84,    85,    15,    16,    83,   327,    90,
     329,    84,   331,   332,   367,   334,   369,    90,   337,    29,
      84,    53,    85,    87,   740,   741,    90,    91,    53,    93,
      94,   350,   351,   352,    86,     8,     9,   356,    53,    85,
     359,   292,   404,   362,   406,   364,    83,    84,   367,    85,
     369,   370,    84,    85,    84,    85,    30,    31,    90,    84,
      85,    87,    88,   425,    85,    90,    15,    16,    42,    84,
      44,   390,   391,   392,   393,    90,   395,    13,    14,    53,
      85,    55,    56,    57,    85,   404,   405,   406,    84,   408,
      85,    87,    88,    85,   413,    85,   415,   416,    15,    16,
      74,   352,   421,     0,     1,   356,   425,     4,    85,   428,
      84,     8,     9,    10,    11,    85,    90,    12,    13,   370,
      85,    50,    19,   504,   505,    54,    23,    24,    25,    26,
      27,    50,    29,    30,    85,    54,    91,    34,    35,    36,
      37,    38,    39,    85,   395,    42,    85,    44,    85,    46,
      47,    80,    81,    82,   405,    85,    53,   408,   477,    78,
      79,    80,    81,    82,   504,   505,    86,   486,   543,    85,
     489,    85,   491,     1,    85,   550,     4,    85,    30,   541,
      85,   543,    85,    11,   504,   505,    83,    84,   550,    85,
      42,    19,    44,    90,    85,    85,   683,   684,    85,   518,
      85,    53,    30,    55,    56,    57,    34,    35,    85,    53,
      38,    55,    56,   532,    42,    85,    44,   536,    46,    85,
      85,    85,   541,    85,   543,    53,    85,   546,   547,    85,
      85,   550,    84,     6,    85,    85,    50,    85,    90,    53,
      84,    55,    56,    85,    85,    85,    90,    85,    85,    93,
      94,    85,    85,    85,    85,    85,    84,    92,    85,    85,
      85,    85,    90,    85,    85,    85,    85,   518,   587,    85,
      84,    84,   591,    87,   593,    85,    90,    91,    85,    93,
      94,    85,    85,    85,   603,   604,    85,    85,   607,   608,
     609,   610,   611,    85,    85,   546,   547,    85,    85,    85,
     675,    85,    84,     3,   623,   624,    87,   669,    85,   628,
      86,    86,    92,   675,    92,     6,    83,   631,    83,   295,
     639,    59,    88,    86,    58,   644,   302,    87,     6,    83,
      83,    86,    59,    85,    84,    68,   587,    85,    83,    22,
     591,    83,   593,   662,    83,   321,    59,   666,    91,    86,
     669,    83,    68,    83,   330,    86,   675,    83,   609,    83,
      68,    83,    58,    92,    83,   341,   342,    85,   687,   688,
      59,     1,    83,    59,     4,    68,    86,   628,     8,     9,
      10,    11,    83,    86,    86,    15,    16,    68,   639,    19,
       6,    83,    86,   644,    22,     6,    26,    27,    86,    29,
      30,    83,   364,    83,    34,    35,    36,    37,    38,    39,
      83,   662,    42,    83,    44,   666,    46,    83,    50,    83,
      83,    50,    54,    53,    53,    83,    55,    56,    83,    83,
      83,    83,    83,    83,   701,   428,   687,   688,   414,   495,
     425,   417,    74,   406,    76,    77,    78,    79,    80,    81,
      82,   661,   428,    83,    84,    84,   422,   742,   475,   513,
      90,    90,   177,   188,    93,    94,    -1,    -1,    -1,    -1,
     446,   447,    -1,   449,   450,    -1,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,    -1,
      -1,    50,   478,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   493,    67,    68,
      69,    70,    71,    72,    73,    74,   502,    76,    77,    78,
      79,    80,    81,    82,     1,    -1,    -1,   513,    -1,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    18,    -1,    -1,    21,   531,   532,   533,    -1,    -1,
      27,    -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    42,    -1,    44,   554,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,
      57,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      81,    82,    -1,     1,    -1,    -1,     4,    74,    -1,    -1,
       8,     9,    10,    11,    -1,    -1,    83,    84,    -1,    -1,
      -1,    19,    -1,    90,    -1,    -1,    -1,    -1,    26,    27,
     606,    29,    30,    30,    31,    -1,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    42,    44,    44,    46,    -1,
     626,    -1,    -1,    30,    31,    53,    53,    -1,    55,    56,
      57,    -1,    -1,    -1,    -1,    42,    -1,    44,    -1,    -1,
     646,    -1,    -1,    -1,    -1,    -1,    53,    74,    55,    56,
      57,    -1,    -1,    -1,    -1,    -1,    84,    84,    -1,    -1,
      87,    -1,    90,    90,    91,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      87,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    -1,
     696,    -1,    -1,    -1,   700,    -1,    -1,    -1,    -1,    -1,
     706,   707,     1,   709,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    50,    -1,    -1,    53,    54,    55,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    90,    -1,    -1,    93,    94,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    -1,    -1,    53,    54,
      55,    56,    57,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,     1,    84,
      -1,     4,    87,    -1,    -1,    90,    -1,    -1,    11,    -1,
      -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    30,    31,
      -1,    34,    35,     3,    37,    38,    39,    -1,    -1,    42,
      42,    44,    44,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    53,    22,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      50,    84,    84,    85,    54,    -1,     3,    90,    90,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    22,    76,    77,    78,    79,
      80,    81,    82,    -1,    31,    -1,    33,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    50,    -1,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    -1,    87,
      -1,    -1,    90,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    -1,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    -1,    -1,    87,    -1,    -1,
      90,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    -1,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    -1,    -1,    -1,
      54,    -1,     3,    -1,    -1,    -1,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    22,    76,    77,    78,    79,    80,    81,    82,    -1,
      31,    -1,    33,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    50,
      -1,    -1,    -1,    54,    -1,     3,    -1,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    22,    76,    77,    78,    79,    80,
      81,    82,    -1,    31,    85,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    50,    -1,    -1,    -1,    54,    -1,     3,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    22,    76,    77,
      78,    79,    80,    81,    82,    83,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    50,    -1,    -1,    -1,    54,
      -1,     3,    -1,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      22,    76,    77,    78,    79,    80,    81,    82,    83,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,     3,    -1,    -1,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    33,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    -1,    48,    -1,    50,    -1,
      -1,    -1,    54,    -1,     3,    -1,    -1,     6,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    22,    76,    77,    78,    79,    80,    81,
      82,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    50,    -1,    -1,    -1,    54,    -1,     3,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    22,    76,    77,    78,
      79,    80,    81,    82,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,
       3,    -1,    -1,    -1,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    22,
      76,    77,    78,    79,    80,    81,    82,    -1,    31,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
       3,    -1,    -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    50,    -1,    -1,
      -1,    54,    -1,     3,    -1,    -1,     6,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    22,    76,    77,    78,    79,    80,    81,    82,
      -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      50,    -1,    -1,    -1,    54,    -1,     3,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    22,    76,    77,    78,    79,
      80,    81,    82,    -1,    31,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,     3,
      -1,    -1,    -1,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    22,    76,
      77,    78,    79,    80,    81,    82,    -1,    31,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    22,    76,    77,    78,    79,    80,    81,    82,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    22,    76,    77,    78,    79,    80,
      81,    82,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    22,    76,    77,
      78,    79,    80,    81,    82,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    30,    31,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,
      42,    -1,    44,    -1,    -1,    -1,    -1,    42,    -1,    44,
      -1,    53,    -1,    55,    56,    57,    -1,    -1,    53,    -1,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    84,    85,    -1,    -1,    -1,    50,    90,    84,
      85,    54,    -1,    -1,    -1,    90,    -1,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    96,    97,     0,    98,     1,     8,     9,    10,    11,
      19,    23,    24,    25,    26,    27,    29,    30,    34,    35,
      36,    37,    38,    39,    42,    44,    46,    47,    83,    99,
     100,   102,   104,   108,   109,   115,   116,   127,   128,   129,
     131,   132,   133,   137,   138,   139,   140,   177,    37,    39,
     104,    53,    84,    90,   187,   187,    55,   101,   101,   101,
      19,    40,    84,    90,   158,   159,   160,   187,     8,     9,
      88,   144,   145,   187,   187,   187,   187,    83,   105,   187,
      83,   106,   187,   187,   107,    56,    83,     4,   187,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    50,    54,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    76,    77,    78,    79,    80,    81,    82,    87,
      87,   144,   144,    54,    54,    54,    84,   155,   158,    19,
      40,   158,   158,     6,    83,    88,    92,   151,    89,   146,
     147,   187,     1,    55,    56,    84,    90,    93,    94,   161,
     162,   164,   165,   166,   187,   145,   144,    68,    83,    83,
      84,   104,   144,   104,   145,    49,   148,   104,    83,    84,
     134,   145,   187,    86,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    91,   155,   155,    85,   156,
     157,   187,    86,   142,    85,    59,    91,   117,    89,   152,
     158,   187,    84,   182,    83,    89,    59,   148,   162,    50,
      91,   164,   169,   170,   187,    30,    42,    44,    55,    56,
      57,    84,    90,   161,   180,   181,   187,    92,     6,     3,
      21,    68,    84,   167,    84,   110,   111,   155,   187,   155,
     158,    31,    74,    85,   179,   180,   185,   186,   155,   110,
      10,    46,    90,   149,   150,   187,    68,    83,   187,    87,
     134,   187,   144,   158,   132,   141,   141,    83,    85,    59,
      86,   158,   179,     1,     4,    11,    15,    16,    19,    30,
      34,    35,    37,    38,    39,    42,    44,    46,   118,   121,
     123,   124,   125,   126,   188,    59,    89,    85,   179,   183,
     184,   147,   147,    85,     3,    86,    59,    91,    86,   187,
     187,   187,     1,    30,    31,    42,    44,    74,   179,    91,
     185,    92,    87,   151,   182,    90,   187,   171,   172,   165,
     179,   165,    50,    85,   162,   168,   187,     6,   113,   148,
     110,   141,    83,   179,   179,    83,     3,    22,    31,    33,
      48,    50,    54,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    76,    77,
      78,    79,    80,    81,    82,    59,    85,    18,    21,   141,
     113,   150,   150,   153,   154,   158,    59,   151,   158,    86,
     187,    87,   134,    58,   135,   111,     6,   143,   143,   157,
     157,   158,    59,    91,   120,   119,    83,   187,   187,   187,
     187,    83,   158,    59,    85,   165,   158,   170,    86,   158,
      85,    85,    91,    90,   187,   181,    91,   169,   188,     1,
       6,     7,    18,    21,    46,    83,   133,   173,   174,   179,
     187,    59,    85,    58,   114,    83,    17,    68,   112,   113,
      68,   130,   179,   179,    22,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     186,    83,   187,   179,   143,    83,    91,    59,   150,    83,
     187,    86,   187,    86,   136,   179,    83,   142,   171,    83,
      83,   179,   121,   123,   125,   121,    52,    68,    68,    52,
      68,    68,   184,   158,    91,   185,    91,    83,   171,   179,
     179,   187,   179,   132,    46,   187,    58,    83,    86,   168,
     162,   103,   158,   158,    83,   162,   163,    83,   179,    86,
      83,   158,    85,   158,    86,   158,    59,   135,   188,    91,
     187,   187,   179,   187,   187,   158,   187,   187,    91,   188,
       6,     6,    86,    43,   187,   187,    86,   179,   158,    59,
      37,    39,    83,   104,   188,    59,   158,   135,   158,   135,
     179,   122,    83,   176,   176,   158,   171,    86,    86,   158,
      83,   135,   168,    83,    83,   162,    22,    83,   135,    83,
      59,     6,    13,   177,   178,   188,    22,    32,    13,    14,
     175,   158,   158,   135,    83,   179,    83,   179,   117,   171,
     188,    83,   179,   179,   171,   179,    15,    16,   189,   135,
     135,    83,   188,    83,     6,     6,    43,    83,    83,    83,
     171,   171,   171,   188,   188,   175,    83,    83
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 317 "aps.y"
    { public=TRUE; ;}
    break;

  case 3:
#line 318 "aps.y"
    { the_tree = program(yyvsp[0].units); ;}
    break;

  case 4:
#line 323 "aps.y"
    { yyval.units = nil_Units(); ;}
    break;

  case 5:
#line 325 "aps.y"
    { yyval.units = xcons_Units(yyvsp[-1].units,yyvsp[0].unit); ;}
    break;

  case 6:
#line 327 "aps.y"
    { yyval.units = yyvsp[-2].units; ;}
    break;

  case 7:
#line 329 "aps.y"
    { yyval.units = yyvsp[-2].units; public = FALSE; ;}
    break;

  case 8:
#line 331 "aps.y"
    { yyval.units = yyvsp[-2].units; public = TRUE; ;}
    break;

  case 9:
#line 333 "aps.y"
    { yyval.units = yyvsp[-1].units; ;}
    break;

  case 10:
#line 337 "aps.y"
    { yyval.unit = yyvsp[0].unit; ;}
    break;

  case 11:
#line 339 "aps.y"
    { yyval.unit = decl_unit(yyvsp[0].declaration); ;}
    break;

  case 12:
#line 343 "aps.y"
    { /* printf("infix[%s] := %d ?\n",symbol_name($3),$2); */
		  set_infix(yyvsp[0].symbol,infix_table[yyvsp[-1].integer][0]); ;}
    break;

  case 13:
#line 346 "aps.y"
    { set_infix(yyvsp[0].symbol,infix_table[yyvsp[-1].integer][0]); ;}
    break;

  case 14:
#line 348 "aps.y"
    { set_infix(yyvsp[0].symbol,infix_table[yyvsp[-1].integer][1]); ;}
    break;

  case 15:
#line 353 "aps.y"
    { yyval.integer = 9; ;}
    break;

  case 16:
#line 355 "aps.y"
    { char *s = (char *)yyvsp[0].string;
		  if (s[1] != '\0') {
		    yyerror("fixity levels must be single digits:");
		  }
		  yyval.integer = s[0]-'0';
		;}
    break;

  case 17:
#line 365 "aps.y"
    { yyval.unit = with_unit(yyvsp[-1].string); ;}
    break;

  case 18:
#line 369 "aps.y"
    { yyval.declarations = nil_Declarations(); ;}
    break;

  case 19:
#line 371 "aps.y"
    { yyval.declarations = yyvsp[-1].declarations; ;}
    break;

  case 20:
#line 373 "aps.y"
    { yyval.declarations = yyvsp[-2].declarations; public = FALSE; ;}
    break;

  case 21:
#line 375 "aps.y"
    { yyval.declarations = yyvsp[-2].declarations; public = TRUE; ;}
    break;

  case 22:
#line 377 "aps.y"
    { yyval.declarations = xcons_Declarations(yyvsp[-1].declarations,yyvsp[0].declaration); ;}
    break;

  case 35:
#line 393 "aps.y"
    { yyval.integer = public; public = FALSE; ;}
    break;

  case 36:
#line 394 "aps.y"
    { yyval.declaration = yyvsp[0].declaration; public = yyvsp[-1].integer; ;}
    break;

  case 37:
#line 395 "aps.y"
    { yyval.integer = public; public = TRUE; ;}
    break;

  case 38:
#line 396 "aps.y"
    { yyval.declaration = yyvsp[0].declaration; public = yyvsp[-1].integer; ;}
    break;

  case 39:
#line 398 "aps.y"
    { yyval.declaration = yyvsp[0].declaration; ;}
    break;

  case 40:
#line 399 "aps.y"
    { constant = FALSE; ;}
    break;

  case 41:
#line 400 "aps.y"
    { yyval.declaration = yyvsp[0].declaration; constant = TRUE; ;}
    break;

  case 42:
#line 402 "aps.y"
    { yyval.declaration = no_decl(); ;}
    break;

  case 43:
#line 407 "aps.y"
    { yyval.declaration = signature_decl(a_signature(yyvsp[-4].symbol,yyvsp[-3].typeFormals,constant,public),
				      yyvsp[-2].declaration,yyvsp[-1].declarations); ;}
    break;

  case 44:
#line 413 "aps.y"
    { yyval.declaration = module_decl(a_signature(yyvsp[-5].symbol,yyvsp[-4].typeFormals,constant,public),
				   a_type(yyvsp[-5].symbol,nil_TypeFormals(),
					  list_Signatures
					  (module_sig(copy_TypeFormals(yyvsp[-4].typeFormals),
						      yyvsp[-3].formals,yyvsp[-2].declaration)),
					  constant,public),
				   yyvsp[-1].declarations); ;}
    break;

  case 45:
#line 421 "aps.y"
    { yyval.declaration = module_decl(a_signature(yyvsp[-4].symbol,yyvsp[-3].typeFormals,constant,public),
				   a_type(yyvsp[-4].symbol,nil_TypeFormals(),
					  list_Signatures
					  (module_sig(copy_TypeFormals(yyvsp[-3].typeFormals),
						      nil_Formals(),yyvsp[-2].declaration)),
					  constant,public),
				   yyvsp[-1].declarations); ;}
    break;

  case 46:
#line 432 "aps.y"
    { if (yyvsp[-2].symbol == NULL) yyvsp[-2].symbol = intern_symbol("Result");
		  yyval.declaration = type_decl(a_type(yyvsp[-2].symbol,nil_TypeFormals(),yyvsp[-1].signatures,
					constant,public),yyvsp[0].type); ;}
    break;

  case 47:
#line 438 "aps.y"
    { yyval.symbol = NULL; ;}
    break;

  case 48:
#line 440 "aps.y"
    { yyval.symbol = yyvsp[0].symbol; ;}
    break;

  case 49:
#line 445 "aps.y"
    { yyval.type = no_type(); ;}
    break;

  case 50:
#line 447 "aps.y"
    { yyval.type = yyvsp[0].type; ;}
    break;

  case 51:
#line 449 "aps.y"
    { yyval.type = yyvsp[0].type; ;}
    break;

  case 52:
#line 454 "aps.y"
    { yyval.declarations = nil_Declarations(); ;}
    break;

  case 53:
#line 455 "aps.y"
    { yyval.integer = public; public = TRUE; ;}
    break;

  case 54:
#line 458 "aps.y"
    { public = yyvsp[-2].integer;
                  /* force the list to contain something if there is a block */
                  yyval.declarations = xcons_Declarations(yyvsp[-1].declarations,no_decl()); ;}
    break;

  case 55:
#line 465 "aps.y"
    { yyval.declaration = pragma_decl(yyvsp[-1].symbol,nil_Expressions()); ;}
    break;

  case 56:
#line 467 "aps.y"
    { yyval.declaration = pragma_decl(yyvsp[-3].symbol,nil_Expressions()); ;}
    break;

  case 57:
#line 469 "aps.y"
    { yyval.declaration = pragma_decl(yyvsp[-4].symbol,yyvsp[-2].expressions); ;}
    break;

  case 58:
#line 476 "aps.y"
    { yyval.declaration = inheritance(yyvsp[-4].type,yyvsp[-2].renamings); ;}
    break;

  case 59:
#line 478 "aps.y"
    { yyval.declaration = inheritance(yyvsp[-1].type,nil_Renamings()); ;}
    break;

  case 60:
#line 483 "aps.y"
    { yyval.renamings = nil_Renamings(); ;}
    break;

  case 61:
#line 485 "aps.y"
    { yyval.renamings = xcons_Renamings(yyvsp[-2].renamings,yyvsp[-1].renaming); ;}
    break;

  case 62:
#line 489 "aps.y"
    { yyval.integer = public; public=TRUE; ;}
    break;

  case 63:
#line 490 "aps.y"
    { yyval.renaming = yyvsp[0].renaming; public = yyvsp[-1].integer; ;}
    break;

  case 64:
#line 491 "aps.y"
    { yyval.integer = public; public=FALSE; ;}
    break;

  case 65:
#line 492 "aps.y"
    { yyval.renaming = yyvsp[0].renaming; public = yyvsp[-1].integer; ;}
    break;

  case 66:
#line 494 "aps.y"
    { yyval.renaming = yyvsp[0].renaming; ;}
    break;

  case 67:
#line 496 "aps.y"
    { yyval.renaming = value_replacement(value_name(yyvsp[-2].symbol),yyvsp[0].expression); ;}
    break;

  case 68:
#line 498 "aps.y"
    { yyval.renaming = type_replacement(type_name(yyvsp[-2].symbol),yyvsp[0].type); ;}
    break;

  case 69:
#line 503 "aps.y"
    { yyval.renaming = no_renaming(); ;}
    break;

  case 70:
#line 505 "aps.y"
    { yyval.renaming = pattern_renaming(a_pattern(yyvsp[-2].symbol,nil_TypeFormals(),
						  no_type(),constant,public),
					pattern_name(yyvsp[0].symbol)); ;}
    break;

  case 71:
#line 509 "aps.y"
    { yyval.renaming = value_renaming(a_value(yyvsp[-2].symbol,nil_TypeFormals(),
					      no_type(),constant,public),
				      value_name(yyvsp[0].symbol)); ;}
    break;

  case 72:
#line 513 "aps.y"
    { yyval.renaming = type_renaming(a_type(yyvsp[-2].symbol,nil_TypeFormals(),
					    nil_Signatures(),constant,public),
				     type_name(yyvsp[0].symbol)); ;}
    break;

  case 73:
#line 517 "aps.y"
    { yyval.renaming = signature_renaming(a_signature(yyvsp[-2].symbol,nil_TypeFormals(),
						      constant,public),
					  sig_name(yyvsp[0].symbol),
					  nil_Renamings()); ;}
    break;

  case 74:
#line 521 "aps.y"
    {yyval.integer = public;;}
    break;

  case 75:
#line 525 "aps.y"
    { public = yyvsp[-3].integer;
		  yyval.renaming = signature_renaming(a_signature(yyvsp[-6].symbol,nil_TypeFormals(),
						      constant,public),
					  sig_name(yyvsp[-4].symbol),
					  yyvsp[-1].renamings); ;}
    break;

  case 76:
#line 533 "aps.y"
    {;}
    break;

  case 77:
#line 534 "aps.y"
    {;}
    break;

  case 78:
#line 535 "aps.y"
    {;}
    break;

  case 79:
#line 536 "aps.y"
    {;}
    break;

  case 80:
#line 537 "aps.y"
    {;}
    break;

  case 81:
#line 538 "aps.y"
    {;}
    break;

  case 82:
#line 542 "aps.y"
    {;}
    break;

  case 83:
#line 546 "aps.y"
    {;}
    break;

  case 84:
#line 547 "aps.y"
    {;}
    break;

  case 85:
#line 551 "aps.y"
    {;}
    break;

  case 86:
#line 552 "aps.y"
    {;}
    break;

  case 87:
#line 557 "aps.y"
    { yyval.declaration = phylum_decl(a_type(yyvsp[-1].symbol,nil_TypeFormals(),
					  nil_Signatures(),constant,public),
				   no_type()); ;}
    break;

  case 88:
#line 561 "aps.y"
    { yyval.declaration = phylum_decl(a_type(yyvsp[-3].symbol,nil_TypeFormals(),
					  nil_Signatures(),constant,public),
		  		   yyvsp[-1].type); ;}
    break;

  case 89:
#line 568 "aps.y"
    { yyval.declaration = constructor_decl
		    (a_value(yyvsp[-5].symbol,yyvsp[-4].typeFormals,function_type(yyvsp[-3].formals,yyvsp[-2].statement),
			     constant,public),
		     a_pattern(yyvsp[-5].symbol,copy_TypeFormals(yyvsp[-4].typeFormals),
			       function_type(copy_Formals(yyvsp[-3].formals),
					     copy_Statement(yyvsp[-2].statement)),
			       constant,public),
		     yyvsp[-1].statement); ;}
    break;

  case 90:
#line 580 "aps.y"
    { yyval.declaration = pattern_decl(a_pattern(yyvsp[-5].symbol,yyvsp[-4].typeFormals,function_type(yyvsp[-3].formals,yyvsp[-2].statement),
					      constant,public),
				    yyvsp[-1].pattern); ;}
    break;

  case 91:
#line 587 "aps.y"
    { yyval.pattern = no_pattern(); ;}
    break;

  case 92:
#line 589 "aps.y"
    { yyval.pattern = choice_pattern(yyvsp[0].patterns); ;}
    break;

  case 93:
#line 591 "aps.y"
    { yyval.pattern = choice_pattern(nil_Patterns()); ;}
    break;

  case 94:
#line 596 "aps.y"
    { yyval.declaration = attribute_decl(
			 a_function(yyvsp[-6].symbol,yyvsp[-5].typeFormals,list_Formals(yyvsp[-4].valueDecl),yyvsp[-2].type,FALSE,public),
			 yyvsp[-8].direction,yyvsp[-1].default_tag); ;}
    break;

  case 95:
#line 600 "aps.y"
    { yyval.declaration = attribute_decl(
			  a_function(yyvsp[-4].symbol,nil_TypeFormals(),
				     list_Formals(yyvsp[-6].valueDecl),yyvsp[-2].type,FALSE,public),
			  yyvsp[-8].direction,yyvsp[-1].default_tag); ;}
    break;

  case 96:
#line 605 "aps.y"
    { yyval.declaration = attribute_decl(
			  a_function(yyvsp[-4].symbol,yyvsp[-7].typeFormals,list_Formals(yyvsp[-6].valueDecl),yyvsp[-2].type,
				     FALSE,public),
			  yyvsp[-9].direction,yyvsp[-1].default_tag); ;}
    break;

  case 97:
#line 613 "aps.y"
    { yyval.direction = no_direction(FALSE); ;}
    break;

  case 99:
#line 619 "aps.y"
    { yyval.direction = no_direction(TRUE); ;}
    break;

  case 100:
#line 621 "aps.y"
    { yyval.direction = collection(TRUE); ;}
    break;

  case 101:
#line 623 "aps.y"
    { yyval.direction = collection(FALSE); ;}
    break;

  case 102:
#line 625 "aps.y"
    { yyval.direction = circular(TRUE); ;}
    break;

  case 103:
#line 627 "aps.y"
    { yyval.direction = circular(FALSE); ;}
    break;

  case 104:
#line 631 "aps.y"
    { yyval.valueDecl = a_var(underscore_symbol,type_name(underscore_symbol),
		  	     TRUE,FALSE); ;}
    break;

  case 105:
#line 634 "aps.y"
    { yyval.valueDecl = a_var(underscore_symbol,type_name(yyvsp[0].symbol),TRUE,FALSE); ;}
    break;

  case 106:
#line 636 "aps.y"
    { yyval.valueDecl = a_var(yyvsp[-3].symbol,type_name(yyvsp[-1].symbol),TRUE,FALSE); ;}
    break;

  case 107:
#line 641 "aps.y"
    { yyval.default_tag = no_default(); ;}
    break;

  case 108:
#line 643 "aps.y"
    { yyval.default_tag = simple(yyvsp[0].expression); ;}
    break;

  case 109:
#line 645 "aps.y"
    { yyval.default_tag = yyvsp[0].default_tag; ;}
    break;

  case 110:
#line 649 "aps.y"
    { yyval.default_tag = composite(yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 111:
#line 651 "aps.y"
    { yyval.default_tag = lattice(yyvsp[-4].expression,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 112:
#line 656 "aps.y"
    { yyval.declaration = top_level_match(yyvsp[0].match); ;}
    break;

  case 113:
#line 661 "aps.y"
    { yyval.declaration = type_decl(a_type(yyvsp[-2].symbol,nil_TypeFormals(),yyvsp[-1].signatures,
					constant,public),
				 no_type()); ;}
    break;

  case 114:
#line 665 "aps.y"
    { yyval.declaration = type_decl(a_type(yyvsp[-4].symbol,nil_TypeFormals(),yyvsp[-3].signatures,
					constant,public),yyvsp[-1].type); ;}
    break;

  case 115:
#line 671 "aps.y"
    { yyval.declaration = value_decl(a_var(yyvsp[-4].symbol,yyvsp[-2].type,constant,public),yyvsp[-5].direction,yyvsp[-1].default_tag); ;}
    break;

  case 116:
#line 676 "aps.y"
    { yyval.declaration = function_decl(a_value(yyvsp[-5].symbol,yyvsp[-4].typeFormals,function_type(yyvsp[-3].formals,yyvsp[-2].statement),
					     constant,public),
				     yyvsp[-1].statement); ;}
    break;

  case 117:
#line 680 "aps.y"
    { yyval.declaration = procedure_decl(a_value(yyvsp[-5].symbol,yyvsp[-4].typeFormals,function_type(yyvsp[-3].formals,yyvsp[-2].statement),
					      constant,public),
				      yyvsp[-1].statement); ;}
    break;

  case 118:
#line 687 "aps.y"
    { if (yyvsp[-2].symbol == NULL) yyvsp[-2].symbol = intern_symbol("result");
		  yyval.statement = value_decl(a_value(yyvsp[-2].symbol,nil_TypeFormals(),yyvsp[-1].type,FALSE,FALSE),
				  yyvsp[-3].direction,yyvsp[0].default_tag); ;}
    break;

  case 119:
#line 694 "aps.y"
    { yyval.type = no_type(); ;}
    break;

  case 120:
#line 696 "aps.y"
    { yyval.type = yyvsp[0].type; ;}
    break;

  case 121:
#line 701 "aps.y"
    { yyval.statement = no_stmt(); ;}
    break;

  case 122:
#line 703 "aps.y"
    { yyval.statement = yyvsp[-1].statement; ;}
    break;

  case 123:
#line 708 "aps.y"
    { yyval.typeFormals = nil_TypeFormals(); ;}
    break;

  case 124:
#line 710 "aps.y"
    { yyval.typeFormals = yyvsp[0].typeFormals; ;}
    break;

  case 125:
#line 715 "aps.y"
    { yyval.typeFormals = yyvsp[-1].typeFormals; ;}
    break;

  case 126:
#line 717 "aps.y"
    { yyval.typeFormals = nil_TypeFormals(); ;}
    break;

  case 127:
#line 722 "aps.y"
    { yyval.typeFormals = yyvsp[0].typeFormals; ;}
    break;

  case 128:
#line 724 "aps.y"
    { yyval.typeFormals = append_TypeFormals(yyvsp[-2].typeFormals,yyvsp[0].typeFormals); ;}
    break;

  case 129:
#line 729 "aps.y"
    { yyval.typeFormals = list_TypeFormals(a_type(yyvsp[-1].symbol,nil_TypeFormals(),
					       yyvsp[0].signatures,TRUE,FALSE)); ;}
    break;

  case 130:
#line 732 "aps.y"
    { Signatures sigs = a_type_signatures(first_TypeFormal(yyvsp[0].typeFormals));
		  yyval.typeFormals = append_TypeFormals(
		  	list_TypeFormals(a_type(yyvsp[-2].symbol,nil_TypeFormals(),
						sigs,TRUE,FALSE)),yyvsp[0].typeFormals); ;}
    break;

  case 131:
#line 740 "aps.y"
    { yyval.signatures = nil_Signatures(); ;}
    break;

  case 132:
#line 742 "aps.y"
    { yyval.signatures = yyvsp[0].signatures; ;}
    break;

  case 133:
#line 747 "aps.y"
    { yyval.signatures = list_Signatures(yyvsp[0].signature); ;}
    break;

  case 134:
#line 749 "aps.y"
    { yyval.signatures = xcons_Signatures(yyvsp[-2].signatures,yyvsp[0].signature); ;}
    break;

  case 135:
#line 754 "aps.y"
    { yyval.signature = sig_name(yyvsp[0].symbol); ;}
    break;

  case 136:
#line 756 "aps.y"
    { yyval.signature = sig_inst(sig_name(yyvsp[-1].symbol),yyvsp[0].typeActuals); ;}
    break;

  case 137:
#line 758 "aps.y"
    { yyval.signature = constant_sig(yyvsp[0].signature); ;}
    break;

  case 138:
#line 760 "aps.y"
    { yyval.signature = var_sig(yyvsp[0].signature); ;}
    break;

  case 139:
#line 762 "aps.y"
    { yyval.signature = fixed_sig(yyvsp[-1].types); ;}
    break;

  case 140:
#line 767 "aps.y"
    { yyval.typeActuals = nil_TypeActuals(); ;}
    break;

  case 141:
#line 769 "aps.y"
    { yyval.typeActuals = yyvsp[-1].typeActuals; ;}
    break;

  case 142:
#line 774 "aps.y"
    { yyval.typeActuals = list_TypeActuals(yyvsp[0].type); ;}
    break;

  case 143:
#line 776 "aps.y"
    { yyval.typeActuals = xcons_TypeActuals(yyvsp[-2].typeActuals,yyvsp[0].type); ;}
    break;

  case 144:
#line 781 "aps.y"
    { yyval.types = nil_Types(); ;}
    break;

  case 145:
#line 783 "aps.y"
    { yyval.types = yyvsp[0].types; ;}
    break;

  case 146:
#line 788 "aps.y"
    { yyval.types = list_Types(yyvsp[0].type); ;}
    break;

  case 147:
#line 790 "aps.y"
    { yyval.types = xcons_Types(yyvsp[-2].types,yyvsp[0].type); ;}
    break;

  case 148:
#line 795 "aps.y"
    { yyval.formals = yyvsp[-1].formals; ;}
    break;

  case 149:
#line 797 "aps.y"
    { yyval.formals = nil_Formals(); ;}
    break;

  case 150:
#line 802 "aps.y"
    { yyval.formals = yyvsp[0].formals; ;}
    break;

  case 151:
#line 804 "aps.y"
    { yyval.formals = append_Formals(yyvsp[-2].formals,yyvsp[0].formals); ;}
    break;

  case 152:
#line 808 "aps.y"
    { yyval.formals = list_Formals(a_var(yyvsp[-2].symbol,yyvsp[0].type,TRUE,FALSE)); ;}
    break;

  case 153:
#line 810 "aps.y"
    { Type ty = a_value_type_(first_Formal(yyvsp[0].formals));
		  yyval.formals = append_Formals(list_Formals(a_var(yyvsp[-2].symbol,ty,TRUE,FALSE)),
				      yyvsp[0].formals); ;}
    break;

  case 154:
#line 816 "aps.y"
    { yyval.type = yyvsp[0].type; ;}
    break;

  case 155:
#line 818 "aps.y"
    { yyval.type = remote_type(yyvsp[0].type); ;}
    break;

  case 156:
#line 822 "aps.y"
    { yyval.type = function_type(yyvsp[-1].formals,value_decl(a_value(underscore_symbol,
							   nil_TypeFormals(),
							   yyvsp[0].type,FALSE,FALSE),
						   no_direction(FALSE),
						   no_default())); ;}
    break;

  case 158:
#line 833 "aps.y"
    { yyval.type = qual_type(yyvsp[-2].type,type_name(yyvsp[0].symbol)); ;}
    break;

  case 159:
#line 835 "aps.y"
    { yyval.type = type_inst(yyvsp[-1].type,yyvsp[0].typeActuals,nil_Actuals()); ;}
    break;

  case 160:
#line 837 "aps.y"
    { yyval.type = type_inst(yyvsp[-2].type,yyvsp[-1].typeActuals,yyvsp[0].actuals); ;}
    break;

  case 161:
#line 843 "aps.y"
    { yyval.type = type_name(yyvsp[0].symbol); ;}
    break;

  case 162:
#line 848 "aps.y"
    { yyval.type = list_type(yyvsp[-1].type); ;}
    break;

  case 163:
#line 850 "aps.y"
    { yyval.type = set_type(yyvsp[-3].type,yyvsp[-1].expression); ;}
    break;

  case 164:
#line 852 "aps.y"
    { yyval.type = ordered_set_type(yyvsp[-5].type,yyvsp[-3].expression,yyvsp[-1].expression); ;}
    break;

  case 165:
#line 854 "aps.y"
    { yyval.type = yyvsp[-1].type; ;}
    break;

  case 166:
#line 862 "aps.y"
    { yyval.type = type_name(yyvsp[0].symbol); ;}
    break;

  case 167:
#line 864 "aps.y"
    { yyval.type = qual_type(yyvsp[-2].type,type_name(yyvsp[0].symbol)); ;}
    break;

  case 169:
#line 869 "aps.y"
    { yyval.pattern = and_pattern(yyvsp[-2].pattern,condition(yyvsp[0].expression)); ;}
    break;

  case 170:
#line 871 "aps.y"
    { yyval.pattern = no_pattern(); ;}
    break;

  case 171:
#line 876 "aps.y"
    { yyval.patterns = list_Patterns(yyvsp[0].pattern); ;}
    break;

  case 172:
#line 878 "aps.y"
    { yyval.patterns = xcons_Patterns(yyvsp[-2].patterns,yyvsp[0].pattern); ;}
    break;

  case 174:
#line 883 "aps.y"
    { yyval.pattern = and_pattern(yyvsp[-2].pattern,yyvsp[0].pattern); ;}
    break;

  case 175:
#line 885 "aps.y"
    { yyval.pattern = and_pattern(yyvsp[-2].pattern,yyvsp[0].pattern); ;}
    break;

  case 176:
#line 890 "aps.y"
    { yyval.pattern = simple_pattern(yyvsp[-1].pattern,yyvsp[0].patternActuals); ;}
    break;

  case 177:
#line 892 "aps.y"
    { yyval.pattern = sequence_pattern(yyvsp[-1].patterns,no_type()); ;}
    break;

  case 178:
#line 894 "aps.y"
    { yyval.pattern = sequence_pattern(yyvsp[-3].patterns,yyvsp[0].type); ;}
    break;

  case 179:
#line 896 "aps.y"
    { yyval.pattern = sequence_pattern(yyvsp[-1].patterns,yyvsp[-4].type); ;}
    break;

  case 180:
#line 898 "aps.y"
    { yyval.pattern = sequence_pattern(nil_Patterns(),no_type()); ;}
    break;

  case 181:
#line 900 "aps.y"
    { yyval.pattern = sequence_pattern(nil_Patterns(),yyvsp[0].type); ;}
    break;

  case 182:
#line 902 "aps.y"
    { yyval.pattern = sequence_pattern(nil_Patterns(),yyvsp[-3].type); ;}
    break;

  case 183:
#line 904 "aps.y"
    { yyval.pattern = pattern_var(a_var(yyvsp[-2].symbol,yyvsp[0].type,TRUE,FALSE)); ;}
    break;

  case 184:
#line 906 "aps.y"
    { yyval.pattern = pattern_var(a_var(yyvsp[0].symbol,no_type(),TRUE,FALSE)); ;}
    break;

  case 185:
#line 908 "aps.y"
    { yyval.pattern = pattern_var(a_var(underscore_symbol,no_type(),
					 TRUE,FALSE)); ;}
    break;

  case 186:
#line 914 "aps.y"
    { yyval.pattern = match_expression(yyvsp[0].expression); ;}
    break;

  case 187:
#line 923 "aps.y"
    { yyval.pattern = match_expression(value_name(yyvsp[0].symbol)); ;}
    break;

  case 188:
#line 925 "aps.y"
    { yyval.pattern = match_expression(integer_const(yyvsp[0].string)); ;}
    break;

  case 189:
#line 927 "aps.y"
    { yyval.pattern = match_expression(string_const(yyvsp[0].string)); ;}
    break;

  case 190:
#line 929 "aps.y"
    { yyval.pattern = yyvsp[-1].pattern; ;}
    break;

  case 191:
#line 934 "aps.y"
    { yyval.pattern = pattern_name(yyvsp[0].symbol); ;}
    break;

  case 192:
#line 936 "aps.y"
    { yyval.pattern = qual_pattern(yyvsp[-2].type,pattern_name(yyvsp[0].symbol)); ;}
    break;

  case 193:
#line 941 "aps.y"
    { yyval.patternActuals = nil_PatternActuals(); ;}
    break;

  case 194:
#line 943 "aps.y"
    { yyval.patternActuals = yyvsp[-1].patternActuals; ;}
    break;

  case 195:
#line 948 "aps.y"
    { yyval.patternActuals = list_PatternActuals(yyvsp[0].pattern); ;}
    break;

  case 196:
#line 950 "aps.y"
    { yyval.patternActuals = list_PatternActuals(pattern_actual(yyvsp[0].pattern,
		                                          value_name(yyvsp[-2].symbol))); ;}
    break;

  case 197:
#line 953 "aps.y"
    { yyval.patternActuals = list_PatternActuals(rest_pattern(no_pattern())); ;}
    break;

  case 198:
#line 957 "aps.y"
    { yyval.patternActuals = cons_PatternActuals(yyvsp[-2].pattern,yyvsp[0].patternActuals); ;}
    break;

  case 199:
#line 959 "aps.y"
    { yyval.patternActuals = cons_PatternActuals
		         (pattern_actual(yyvsp[-2].pattern,value_name(yyvsp[-4].symbol)),yyvsp[0].patternActuals); ;}
    break;

  case 200:
#line 965 "aps.y"
    { yyval.patterns = list_Patterns(yyvsp[0].pattern); ;}
    break;

  case 201:
#line 967 "aps.y"
    { yyval.patterns = xcons_Patterns(yyvsp[-2].patterns,yyvsp[0].pattern); ;}
    break;

  case 202:
#line 972 "aps.y"
    { yyval.pattern = yyvsp[0].pattern; ;}
    break;

  case 203:
#line 974 "aps.y"
    { yyval.pattern = rest_pattern(no_pattern()); ;}
    break;

  case 204:
#line 976 "aps.y"
    { yyval.pattern = rest_pattern(yyvsp[0].pattern); ;}
    break;

  case 205:
#line 980 "aps.y"
    { yyval.statement = stmts(yyvsp[0].statements); ;}
    break;

  case 206:
#line 984 "aps.y"
    { yyval.statements = nil_Statements(); ;}
    break;

  case 207:
#line 986 "aps.y"
    { yyval.statements = xcons_Statements(yyvsp[-1].statements,yyvsp[0].statement); ;}
    break;

  case 208:
#line 990 "aps.y"
    { yyval.statement = no_stmt(); ;}
    break;

  case 209:
#line 991 "aps.y"
    { yyval.statement = no_stmt(); ;}
    break;

  case 210:
#line 993 "aps.y"
    { yyval.statement = effect(yyvsp[-1].expression); ;}
    break;

  case 211:
#line 995 "aps.y"
    { yyval.statement = assign(yyvsp[-3].expression,yyvsp[-1].expression); ;}
    break;

  case 212:
#line 1000 "aps.y"
    { yyval.statement = if_stmt(yyvsp[-5].expression,yyvsp[-3].statement,yyvsp[-2].statement); ;}
    break;

  case 213:
#line 1002 "aps.y"
    { yyval.statement = for_in_stmt(a_var(yyvsp[-8].symbol,yyvsp[-6].type,TRUE,FALSE),yyvsp[-4].expression,yyvsp[-2].statement); ;}
    break;

  case 214:
#line 1004 "aps.y"
    { yyval.statement = for_on_stmt(a_var(yyvsp[-8].symbol,yyvsp[-6].type,TRUE,FALSE),yyvsp[-4].expression,yyvsp[-2].statement); ;}
    break;

  case 215:
#line 1010 "aps.y"
    { yyval.statement = value_decl(a_var(yyvsp[-4].symbol,yyvsp[-2].type,TRUE,FALSE),yyvsp[-5].direction,yyvsp[-1].default_tag); ;}
    break;

  case 216:
#line 1012 "aps.y"
    { yyval.statement = value_decl(a_var(yyvsp[-4].symbol,yyvsp[-2].type,TRUE,FALSE),
				  no_direction(FALSE),yyvsp[-1].default_tag); ;}
    break;

  case 217:
#line 1015 "aps.y"
    { yyval.statement = value_decl(a_var(yyvsp[-4].symbol,yyvsp[-2].type,FALSE,FALSE),yyvsp[-5].direction,yyvsp[-1].default_tag); ;}
    break;

  case 218:
#line 1017 "aps.y"
    { yyval.statement = value_decl(a_var(yyvsp[-4].symbol,yyvsp[-2].type,FALSE,FALSE),yyvsp[-6].direction,yyvsp[-1].default_tag); ;}
    break;

  case 219:
#line 1019 "aps.y"
    { yyval.statement = yyvsp[-2].statement; ;}
    break;

  case 220:
#line 1021 "aps.y"
    { yyval.statement = case_stmt(yyvsp[-5].expression,yyvsp[-3].matches,yyvsp[-2].statement); ;}
    break;

  case 221:
#line 1023 "aps.y"
    { yyval.statement = for_stmt(yyvsp[-4].expression,yyvsp[-2].matches); ;}
    break;

  case 222:
#line 1027 "aps.y"
    { yyval.expression = yyvsp[0].expression; ;}
    break;

  case 223:
#line 1037 "aps.y"
    { yyval.statement = no_stmt(); ;}
    break;

  case 224:
#line 1039 "aps.y"
    { yyval.statement = yyvsp[0].statement; ;}
    break;

  case 225:
#line 1041 "aps.y"
    { yyval.statement = if_stmt(yyvsp[-3].expression,yyvsp[-1].statement,yyvsp[0].statement); ;}
    break;

  case 226:
#line 1045 "aps.y"
    { yyval.matches = nil_Matches(); ;}
    break;

  case 227:
#line 1047 "aps.y"
    { yyval.matches = xcons_Matches(yyvsp[-1].matches,yyvsp[0].match); ;}
    break;

  case 228:
#line 1051 "aps.y"
    { yyval.match = matcher(yyvsp[-5].typeFormals,yyvsp[-4].pattern,yyvsp[-2].statement); ;}
    break;

  case 229:
#line 1055 "aps.y"
    { yyval.statement = no_stmt(); ;}
    break;

  case 230:
#line 1057 "aps.y"
    { yyval.statement = yyvsp[0].statement; ;}
    break;

  case 232:
#line 1061 "aps.y"
    { yyval.expression = repeat(yyvsp[-1].expression); ;}
    break;

  case 233:
#line 1062 "aps.y"
    { yyval.expression = binop("..",yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 234:
#line 1063 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 235:
#line 1064 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 236:
#line 1065 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 237:
#line 1066 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 238:
#line 1067 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 239:
#line 1068 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 240:
#line 1069 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 241:
#line 1070 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 242:
#line 1071 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 243:
#line 1072 "aps.y"
    { yyval.expression = binop("=",yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 244:
#line 1073 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 245:
#line 1074 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 246:
#line 1075 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 247:
#line 1076 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 248:
#line 1077 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 249:
#line 1078 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 250:
#line 1079 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 251:
#line 1080 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 252:
#line 1081 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 253:
#line 1082 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 254:
#line 1083 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 255:
#line 1084 "aps.y"
    { yyval.expression = sbinop(yyvsp[-1].symbol,yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 256:
#line 1085 "aps.y"
    { yyval.expression = binop("-",yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 257:
#line 1086 "aps.y"
    { yyval.expression = unop("#-",yyvsp[0].expression); ;}
    break;

  case 258:
#line 1087 "aps.y"
    { yyval.expression = binop("and",yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 259:
#line 1088 "aps.y"
    { yyval.expression = binop("or",yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 260:
#line 1089 "aps.y"
    { yyval.expression = unop("not",yyvsp[0].expression); ;}
    break;

  case 261:
#line 1090 "aps.y"
    { yyval.expression = binop("in",yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 262:
#line 1091 "aps.y"
    { yyval.expression = unop("not",binop("in",yyvsp[-3].expression,yyvsp[0].expression)); ;}
    break;

  case 264:
#line 1107 "aps.y"
    { yyval.expression = value_inst(yyvsp[-1].expression,yyvsp[0].typeActuals); ;}
    break;

  case 265:
#line 1109 "aps.y"
    { yyval.expression = funcall(yyvsp[-1].expression,yyvsp[0].actuals); ;}
    break;

  case 266:
#line 1111 "aps.y"
    { yyval.expression = funcall(yyvsp[0].expression,list_Actuals(yyvsp[-2].expression)); ;}
    break;

  case 267:
#line 1117 "aps.y"
    { yyval.expression = value_name(yyvsp[0].symbol); ;}
    break;

  case 268:
#line 1118 "aps.y"
    { yyval.expression = type_value(type_name(yyvsp[0].symbol)); ;}
    break;

  case 269:
#line 1119 "aps.y"
    { yyval.expression = signature_value(sig_name(yyvsp[0].symbol)); ;}
    break;

  case 270:
#line 1120 "aps.y"
    { yyval.expression = signature_value(sig_name(yyvsp[0].symbol)); ;}
    break;

  case 271:
#line 1135 "aps.y"
    { yyval.expression = integer_const(yyvsp[0].string); ;}
    break;

  case 272:
#line 1136 "aps.y"
    { yyval.expression = string_const(yyvsp[0].string); ;}
    break;

  case 273:
#line 1137 "aps.y"
    { yyval.expression = char_const(yyvsp[0].string);;}
    break;

  case 274:
#line 1138 "aps.y"
    { yyval.expression = yyvsp[-1].expression; ;}
    break;

  case 275:
#line 1140 "aps.y"
    { yyval.expression = sequence(yyvsp[-1].expressions,no_type()); ;}
    break;

  case 276:
#line 1142 "aps.y"
    { yyval.expression = sequence(nil_Expressions(),no_type()); ;}
    break;

  case 277:
#line 1147 "aps.y"
    { yyval.expression = sequence(yyvsp[-1].expressions,yyvsp[-4].type); ;}
    break;

  case 278:
#line 1149 "aps.y"
    { yyval.expression = sequence(nil_Expressions(), yyvsp[-3].type); ;}
    break;

  case 279:
#line 1151 "aps.y"
    { yyval.expression = qual_value(yyvsp[-2].type,value_name(yyvsp[0].symbol)); ;}
    break;

  case 280:
#line 1152 "aps.y"
    { yyval.expression = no_expr(); ;}
    break;

  case 281:
#line 1156 "aps.y"
    { yyval.actuals = nil_Actuals(); ;}
    break;

  case 282:
#line 1158 "aps.y"
    { yyval.actuals = yyvsp[-1].actuals; ;}
    break;

  case 283:
#line 1163 "aps.y"
    { yyval.actuals = list_Actuals(yyvsp[0].expression); ;}
    break;

  case 284:
#line 1165 "aps.y"
    { yyval.actuals = xcons_Actuals(yyvsp[-2].actuals,yyvsp[0].expression); ;}
    break;

  case 285:
#line 1169 "aps.y"
    { yyval.expression = yyvsp[0].expression; ;}
    break;

  case 286:
#line 1174 "aps.y"
    { yyval.expressions = list_Expressions(yyvsp[0].expression); ;}
    break;

  case 287:
#line 1176 "aps.y"
    { yyval.expressions = xcons_Expressions(yyvsp[-2].expressions,yyvsp[0].expression); ;}
    break;

  case 289:
#line 1182 "aps.y"
    { yyval.expression = guarded(yyvsp[-2].expression,yyvsp[0].expression); ;}
    break;

  case 290:
#line 1184 "aps.y"
    { yyval.expression = controlled(yyvsp[-6].expression,a_var(yyvsp[-4].symbol,yyvsp[-2].type,TRUE,FALSE),yyvsp[0].expression); ;}
    break;

  case 291:
#line 1189 "aps.y"
    { yyval.symbol = yyvsp[0].symbol; ;}
    break;

  case 292:
#line 1191 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 293:
#line 1193 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 294:
#line 1195 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 295:
#line 1197 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 296:
#line 1199 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 297:
#line 1201 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 298:
#line 1203 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 299:
#line 1205 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 300:
#line 1207 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 301:
#line 1209 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 302:
#line 1211 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 303:
#line 1213 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 304:
#line 1215 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 305:
#line 1217 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 306:
#line 1219 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 307:
#line 1221 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 308:
#line 1225 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 309:
#line 1229 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 310:
#line 1231 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 311:
#line 1233 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 312:
#line 1235 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 313:
#line 1237 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 314:
#line 1239 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 315:
#line 1241 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 316:
#line 1243 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 317:
#line 1247 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 318:
#line 1251 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 319:
#line 1253 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 320:
#line 1255 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 321:
#line 1257 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 322:
#line 1259 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 323:
#line 1261 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 324:
#line 1263 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 325:
#line 1265 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 326:
#line 1267 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 327:
#line 1269 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 328:
#line 1271 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 329:
#line 1273 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 330:
#line 1275 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 331:
#line 1279 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 332:
#line 1283 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 333:
#line 1285 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 334:
#line 1287 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 335:
#line 1289 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 336:
#line 1291 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 337:
#line 1293 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 338:
#line 1295 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 339:
#line 1297 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 340:
#line 1299 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 341:
#line 1301 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 342:
#line 1303 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 343:
#line 1305 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 344:
#line 1307 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 345:
#line 1309 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 346:
#line 1311 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 347:
#line 1313 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 348:
#line 1315 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 349:
#line 1317 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 350:
#line 1319 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 351:
#line 1321 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 352:
#line 1323 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 353:
#line 1325 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 354:
#line 1327 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 355:
#line 1329 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 356:
#line 1331 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 357:
#line 1333 "aps.y"
    { yyval.symbol = intern_symbol("="); ;}
    break;

  case 358:
#line 1335 "aps.y"
    { yyval.symbol = intern_symbol("-"); ;}
    break;

  case 359:
#line 1337 "aps.y"
    { yyval.symbol = yyvsp[-1].symbol; ;}
    break;

  case 360:
#line 1339 "aps.y"
    { yyval.symbol = intern_symbol("{}"); ;}
    break;

  case 361:
#line 1344 "aps.y"
    { yyval.symbol = intern_symbol("."); ;}
    break;

  case 362:
#line 1346 "aps.y"
    { yyval.symbol = intern_symbol(".."); ;}
    break;

  case 363:
#line 1348 "aps.y"
    { yyval.symbol = intern_symbol("..."); ;}
    break;

  case 364:
#line 1352 "aps.y"
    {;}
    break;

  case 365:
#line 1353 "aps.y"
    { yylexerror("endif used for end"); ;}
    break;

  case 366:
#line 1357 "aps.y"
    {;}
    break;

  case 367:
#line 1358 "aps.y"
    { yylexerror("end used for endif"); ;}
    break;


    }

/* Line 991 of yacc.c.  */
#line 4224 "aps.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1361 "aps.y"


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

