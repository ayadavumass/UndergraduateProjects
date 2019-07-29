/*
 *  The scanner definition for COOL ASTs
 */

import java_cup.runtime.Symbol;

%%

%{
    StringBuffer buf = new StringBuffer();

    int line() { return yyline; }
%}

%eofval{
    return new Symbol(ASTConstants.EOF); 
%eofval}

%class ASTLexer
%cup
%line

WHITESPACE = [ \t\b\f\r\n]
SYM        = [A-Za-z][_A-Za-z0-9]*

%state STRING

%%

<YYINITIAL>{WHITESPACE} {}
<YYINITIAL>[0-9]+       { return new Symbol(ASTConstants.INT_CONST,
					    AbstractTable.inttable.addString(yytext())); }

<YYINITIAL>#[0-9]+      { return new Symbol(ASTConstants.LINENO, 
					    new Integer(yytext().substring(1))); }

<YYINITIAL>_program     { return new Symbol(ASTConstants.PROGRAM); }
<YYINITIAL>_class       { return new Symbol(ASTConstants.CLASS); }
<YYINITIAL>_method      { return new Symbol(ASTConstants.METHOD); }
<YYINITIAL>_attr        { return new Symbol(ASTConstants.ATTR); }
<YYINITIAL>_formal      { return new Symbol(ASTConstants.FORMAL); }
<YYINITIAL>_branch      { return new Symbol(ASTConstants.BRANCH); }
<YYINITIAL>_assign      { return new Symbol(ASTConstants.ASSIGN); }
<YYINITIAL>_static_dispatch { return new Symbol(ASTConstants.STATIC_DISPATCH);}
<YYINITIAL>_dispatch    { return new Symbol(ASTConstants.DISPATCH); }
<YYINITIAL>_cond        { return new Symbol(ASTConstants.COND); }
<YYINITIAL>_loop        { return new Symbol(ASTConstants.LOOP); }
<YYINITIAL>_typcase     { return new Symbol(ASTConstants.TYPCASE); }
<YYINITIAL>_block       { return new Symbol(ASTConstants.BLOCK); }
<YYINITIAL>_let         { return new Symbol(ASTConstants.LET); }
<YYINITIAL>_plus        { return new Symbol(ASTConstants.PLUS); }
<YYINITIAL>_sub         { return new Symbol(ASTConstants.SUB); }
<YYINITIAL>_mul         { return new Symbol(ASTConstants.MUL); }
<YYINITIAL>_divide      { return new Symbol(ASTConstants.DIVIDE); }
<YYINITIAL>_neg         { return new Symbol(ASTConstants.NEG); }
<YYINITIAL>_lt          { return new Symbol(ASTConstants.LT); }
<YYINITIAL>_eq          { return new Symbol(ASTConstants.EQ); }
<YYINITIAL>_leq         { return new Symbol(ASTConstants.LEQ); }
<YYINITIAL>_comp        { return new Symbol(ASTConstants.COMP); }
<YYINITIAL>_int         { return new Symbol(ASTConstants.INT); }
<YYINITIAL>_string      { return new Symbol(ASTConstants.STR); }
<YYINITIAL>_bool        { return new Symbol(ASTConstants.BOOL); }
<YYINITIAL>_new         { return new Symbol(ASTConstants.NEW); }
<YYINITIAL>_isvoid      { return new Symbol(ASTConstants.ISVOID); }
<YYINITIAL>_no_expr     { return new Symbol(ASTConstants.NO_EXPR); }
<YYINITIAL>_no_type     { return new Symbol(ASTConstants.NO_TYPE); }
<YYINITIAL>_object      { return new Symbol(ASTConstants.OBJECT); }

<YYINITIAL>":"          { return new Symbol(ASTConstants.COLON); }
<YYINITIAL>"("          { return new Symbol(ASTConstants.LPAREN); }
<YYINITIAL>")"          { return new Symbol(ASTConstants.RPAREN); }

<YYINITIAL>{SYM}        { return new Symbol(ASTConstants.ID,
					    AbstractTable.idtable.addString(yytext())); }

<YYINITIAL>\"		{ buf.setLength(0); yybegin(STRING);}


<STRING>\"              { yybegin(YYINITIAL);
                          return new Symbol(ASTConstants.STR_CONST, 
					    AbstractTable.stringtable.addString(buf.toString())); }
<STRING>\\n             { buf.append('\n'); }
<STRING>\\t             { buf.append('\t'); }
<STRING>\\b             { buf.append('\b'); }
<STRING>\\f             { buf.append('\f'); }
<STRING>\\.             { buf.append(yytext().substring(1)); }
<STRING>\\u[0-9]*       { buf.append((char)Integer.parseInt(yytext().substring(2), 16)); }

<STRING>.		{ buf.append(yytext()); }
