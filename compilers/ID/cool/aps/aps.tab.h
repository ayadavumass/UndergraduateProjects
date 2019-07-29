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
/* Line 1242 of yacc.c.  */
#line 231 "aps.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE aps_yylval;



