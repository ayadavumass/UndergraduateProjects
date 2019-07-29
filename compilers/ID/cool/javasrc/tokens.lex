/*
 *  The scanner definition for COOL token stream
 */

import java_cup.runtime.Symbol;

%%

%{
    int curr_lineno() {
	return line;
    }

    private AbstractSymbol filename;

    AbstractSymbol curr_filename() {
	return filename;
    }

    int prevstate;
    int line;
    StringBuffer buf = new StringBuffer();
%}

%eofval{
    return new Symbol(TokenConstants.EOF); 
%eofval}

%class CoolTokenLexer
%cup

NEWLINE		= [\n]
WHITESPACE	= [ \t\f\r\013\n]

SYM		= [A-Za-z][_A-Za-z0-9]*

SINGLE		= [+/\-*=<\.~,;:()@\}\{]

%state TOKEN STR ERR INT BOOL TYPSYM OBJSYM STRING

%%

<YYINITIAL,TOKEN,STR,ERR,INT,BOOL,TYPSYM,OBJSYM>{WHITESPACE}+ {}

#[0-9]+               { 
		        try {
			    line = Integer.parseInt(yytext().substring(1)); 
			} catch (NumberFormatException ex) {
			    ex.printStackTrace(System.err);
			    line = 0;
			}
			yybegin(TOKEN); 
		      }
#name{WHITESPACE}*\"  { buf.setLength(0); prevstate = YYINITIAL; yybegin(STRING); }

<TOKEN>CLASS	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.CLASS); }
<TOKEN>ELSE	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.ELSE); }
<TOKEN>FI	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.FI); }
<TOKEN>IF	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.IF); }
<TOKEN>IN	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.IN); }
<TOKEN>INHERITS	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.INHERITS); }
<TOKEN>LET	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.LET); }
<TOKEN>LOOP	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.LOOP); }
<TOKEN>POOL	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.POOL); }
<TOKEN>THEN	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.THEN); }
<TOKEN>WHILE	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.WHILE); }
<TOKEN>ASSIGN	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.ASSIGN); }
<TOKEN>CASE	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.CASE); }
<TOKEN>ESAC	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.ESAC); }
<TOKEN>OF	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.OF); }
<TOKEN>DARROW	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.DARROW); }
<TOKEN>NEW	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.NEW); }
<TOKEN>LE	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.LE); }
<TOKEN>NOT	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.NOT); }
<TOKEN>ISVOID	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.ISVOID); }

<TOKEN>"'+'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.PLUS); }
<TOKEN>"'/'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.DIV); }
<TOKEN>"'-'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.MINUS); }
<TOKEN>"'*'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.MULT); }
<TOKEN>"'='"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.EQ); }
<TOKEN>"'<'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.LT); }
<TOKEN>"'.'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.DOT); }
<TOKEN>"'~'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.NEG); }
<TOKEN>"','"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.COMMA); }
<TOKEN>"';'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.SEMI); }
<TOKEN>"':'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.COLON); }
<TOKEN>"'('"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.LPAREN); }
<TOKEN>"')'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.RPAREN); }
<TOKEN>"'@'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.AT); }
<TOKEN>"'}'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.RBRACE); }
<TOKEN>"'{'"	      { yybegin(YYINITIAL); return new Symbol(TokenConstants.LBRACE); }

<TOKEN>STR_CONST      { yybegin(STR); }
<TOKEN>INT_CONST      { yybegin(INT); }
<TOKEN>BOOL_CONST     { yybegin(BOOL); }
<TOKEN>TYPEID	      { yybegin(TYPSYM); }
<TOKEN>OBJECTID	      { yybegin(OBJSYM); }
<TOKEN>ERROR	      { yybegin(ERR); }

<TOKEN>.	      { throw new Error("unmatched text in token lexer; token expected"); }

<STR>\"		      { prevstate = STR; buf.setLength(0); yybegin(STRING);}
<STR>.                { throw new Error("unmatched text in token lexer; string constant expected"); }

<ERR>\"		      { prevstate = ERR; buf.setLength(0); yybegin(STRING);}
<ERR>.                { throw new Error("unmatched text in token lexer; error message expected"); }

<INT>[0-9]+	      { yybegin(YYINITIAL); 
		        return new Symbol(TokenConstants.INT_CONST,
			                  AbstractTable.inttable.addString(yytext())); }
<INT>.                { throw new Error("unmatched text in token lexer; integer constant expected"); }

<BOOL>true	      { yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.BOOL_CONST, Boolean.TRUE); }
<BOOL>false	      { yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.BOOL_CONST, Boolean.FALSE); }
<BOOL>.               { throw new Error("unmatched text in token lexer; boolean constant expected"); }

<TYPSYM>{SYM}	      { yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.TYPEID, 
			                  AbstractTable.idtable.addString(yytext())); }
<TYPSYM>.             { throw new Error("unmatched text in token lexer; type symbol expected"); }


<OBJSYM>{SYM}	      { yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.OBJECTID, 
			                  AbstractTable.idtable.addString(yytext())); }
<OBJSYM>.             { throw new Error("unmatched text in token lexer; type symbol expected"); }
			

<STRING>\"            { yybegin(YYINITIAL);
			switch(prevstate) {
			case STR: 
			    return new Symbol(TokenConstants.STR_CONST, 
			                      AbstractTable.stringtable.addString(buf.toString())); 
			case ERR:
			    return new Symbol(TokenConstants.ERROR, 
			                      buf.toString()); 
			case YYINITIAL:
			    filename = 	AbstractTable.stringtable.addString(buf.toString());
			    break;
			default:
			    throw new Error("unknown state");
			}
			
                      }

<STRING>\\n             { buf.append('\n'); }
<STRING>\\t             { buf.append('\t'); }
<STRING>\\b             { buf.append('\b'); }
<STRING>\\f             { buf.append('\f'); }
<STRING>\\.             { buf.append(yytext().substring(1)); }
<STRING>\\[0-3][0-7][0-7] { buf.append((char)Integer.parseInt(yytext().substring(1), 8)); }

<STRING>.		{ buf.append(yytext()); }

<YYINITIAL>.|\n         { throw new Error("unmatched text in token lexer; line number expected"); }
