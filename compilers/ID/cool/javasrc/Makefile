#Copyright (c) 2000 The Regents of the University of California.
#All rights reserved.
#
#Permission to use, copy, modify, and distribute this software
#for any purpose, without fee, and without written agreement is
#hereby granted, provided that the above copyright notice and the following
#two paragraphs appear in all copies of this software.
#
#IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
#DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
#OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
#CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
#INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
#AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
#ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
#PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

JAVAC	:= javac
#JAVAC   := jikes +E
JLEX	:= /usr/class/cs143/cool/cool/bin/jlex
CUP	:= /usr/class/cs143/cool/cool/bin/java_cup
CUPCLASSPATH := /usr/class/cs143/cool/cool/lib/java-cup-11a.jar

#CUPCLASSPATH := /usr/misc/share/lib:/usr/misc/java/jre/lib/rt.jar
#CUPCLASSPATH := /usr/misc/share/lib
#CUPCLASSPATH := /home/bec/cs164/lib
#CUPCLASSPATH := /usr/misc/java/lib/classes.zip:/usr/misc/lib

CLASSPATH = ${CUPCLASSPATH}:.

-include site-config.mk

SUPP_SRC := \
	AbstractSymbol.java \
	StringSymbol.java \
	IdSymbol.java \
	IntSymbol.java \
	AbstractTable.java \
	StringTable.java \
	IntTable.java \
	IdTable.java \
	Utilities.java \
	Lexer.java \
	Flags.java \
	TreeNode.java \
	ListNode.java \
	Parser.java \
	TokenLexer.java \
	ASTTest.java \
	SymbolTable.java \
	SymtabExample.java \
	ClassTable.java \
	TreeConstants.java \
	Semant.java \
	CgenClassTable.java \
	CgenSupport.java \
	CgenNode.java \
	Cgen.java \
	BoolConst.java

ASSN_GENSRC := \
	CoolLexer.java \
	CoolTokenLexer.java \
	CoolParser.java \
	TokenConstants.java \
	ASTLexer.java \
	ASTParser.java \
	ASTConstants.java

EXTRA_SRC := \
	cool-tree.java

GENSRC	:= ${SUPP_GENSRC} ${ASSN_GENSRC}

SRC := ${SUPP_SRC} ${SUPP_GENSRC} ${ASSN_SRC} ${ASSN_GENSRC}

CLS := ${patsubst %.java,%.class,${SRC}}

all: ${CLS} cool-tree.m4 lexer parser semant cgen

## cool-tree.class is not a real class file, but we need to have it
## for dependency tracking
${CLS}: ${SRC} ${EXTRA_SRC}
	${JAVAC} -g -nowarn -classpath ${CLASSPATH} ${SRC} ${EXTRA_SRC}
	touch cool-tree.class

CoolLexer.java: cool.lex
	${JLEX} $<
	mv cool.lex.java CoolLexer.java

CoolTokenLexer.java: tokens.lex
	${JLEX} $<
	mv tokens.lex.java CoolTokenLexer.java

ASTLexer.java: ast.lex
	${JLEX} $<
	mv ast.lex.java ASTLexer.java

CoolParser.java TokenConstants.java: cool.cup
	${CUP} -parser CoolParser -symbols TokenConstants -dump -nopositions < $< >cool.output 2>&1

ASTParser.java ASTConstants.java: ast.cup
	${CUP} -parser ASTParser -symbols ASTConstants -dump -nopositions < $< >ast.output 2>&1

cool-tree.java: cool-tree.m4 cool-tree.handcode.m4
	m4 cool-tree.m4 > $@

cool-tree.m4: ../aps/aps2java cool-tree.aps
	./../aps/aps2java cool-tree.aps

../aps/aps2java:
	${MAKE} -C ../aps

cool-tree.cc cool-tree.h: cool-tree.aps ../bin/aps2c++
	./../aps/aps2c++ cool-tree.aps

lexer: Makefile ${CLS}
	@rm -f lexer
	echo '#!/bin/sh' >> lexer
	echo 'java -classpath ${CLASSPATH}:`dirname $$0` Lexer $$*' >> lexer
	chmod 755 lexer

parser: Makefile ${CLS}
	@rm -f parser
	echo '#!/bin/sh' >> parser
	echo 'java -classpath ${CLASSPATH} Parser $$*' >> parser
	chmod 755 parser

semant: Makefile ${CLS}
	@rm -f semant
	echo '#!/bin/sh' >> semant
	echo 'java -classpath ${CLASSPATH} Semant $$*' >> semant
	chmod 755 semant

cgen: Makefile ${CLS}
	@rm -f cgen
	echo '#!/bin/sh' >> cgen
	echo 'java -classpath ${CLASSPATH} Cgen $$*' >> cgen
	chmod 755 cgen

ast-parser: Makefile ${CLS}
	@rm -f ast-parser
	echo '#!/bin/sh' >> ast-parser
	echo 'java -classpath ${CLASSPATH} ASTTest $$*' >> ast-parser
	chmod 755 ast-parser

symtab-example: Makefile ${CLS}
	@rm -f symtab-example
	echo '#!/bin/sh' >> symtab-example
	echo 'java -classpath ${CLASSPATH} SymtabExample $$*' >> symtab-example
	chmod 755 symtab-example

clean:
	rm -f `cat .cvsignore`
