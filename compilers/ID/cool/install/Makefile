#
#
#Copyright (c) 1995,1996 The Regents of the University of California.
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
#


# install/Makefile
#
#    Provides the dependencies for installing and distributing
# the student's version of the files.  Requires the following
# environment variables to be set upon entry:
#
#    CLASS    - the name or number of the course (e.g., cs164)
#    COOL_DIR - (install and distrib) root directory of the
#             student's distribution
#    SCRIPT_SHELL - (install) shell to use in scripts
#    ARCH     - (install and distrib) architecture 
#    ASSN     - (install) number of the assignment to install
#    GMAKE    - GNU make
#    AR       - an archiver
#    ARCHIVE_NEW - flags to AR for creating an archive
#    RANLIB   - a program that builds archive symbol tables
#    ARCHITECTURES - a list of all architectures to be built
#    ARCH_PROGRAM - a program that returns the string naming to the
#                   architecture (unfortunately, this program is called "ARCH"
#		    in the other system Makefiles)
#    ASSIGNMENTS -   a list of all assignments to be installed
#    LIB_FLAGS   - library command line flags for g++ to be used in student
#               assignments.  This variable must include -lfl, and may
#               include -Ldir to add 'dir' to the search path.
#
#  The install procedure actually consists of three rather different
#  components.  The first phase (install_base) builds the directory structure
#  for all ARCHITECTURES and ASSIGNMENTS.  The second phase installs
#  for each ARCH and ASSN and the files specific to that
#  combination.  Not all the variables are needed by
#  both phases.
#
#  The third component is a facility to build a distribution file for
#  a particular architecture; see the target distrib.
#

BIN=${COOL_DIR}/bin/.${ARCH}
LIB=${COOL_DIR}/lib/.${ARCH}/${ASSN}
INC=${COOL_DIR}/include/${ASSN}
SRC=${COOL_DIR}/src/${ASSN}


# bhackett: removed xspim from BINARIES
BINARIES = spim aps2c++ aps2java coolc
DIRECTORIES = bin etc examples lib src include assignments dist doc handouts ${ASSIGNMENTS:%=assignments/%} \
              ${ASSIGNMENTS:%=include/%} ${ARCHITECTURES:%=lib/.%} \
              ${foreach arch,${ARCHITECTURES},${foreach assn,${ASSIGNMENTS},lib/.${arch}/${assn}}} \
              ${ASSIGNMENTS:%=src/%}

INSTALL= ${ARCHITECTURES:%=install_%}
ETC_FILES = copy-skel.SKEL link-object.SKEL link-shared.SKEL
BIN_FILES = dispatch.SKEL
LIB_FILES = trap.handler java-cup-11a.jar jlex.jar

#
# Specify the files needed by the students for each assignment ${ASSN}.
#
#      ${ASSN}_LIBS     - the libraries and object files
#      ${ASSN}_INCLUDES - the include files
#      ${ASSN}_SOURCES  - the C++ source files
#
#    Each of the above are copied from the instructor's `src' directory
# (where they are created during the construction of `coolc').
#
#  There is another group of files, the "skeleton" code files, for
#  each assignment.  These files are in directories 
#  assignments/${ASSN}/*.SKEL, and are simply copied to the student 
#  distribution with the .SKEL extension removed.
#
PA2_LIBS = parser semant cgen
PA2_INCLUDES = \
	cgen_gc.h \
	cool-parse.h \
	cool-io.h \
	cool.h \
	copyright.h \
	list.h \
	stringtab.h \
	stringtab_functions.h \
	tree.h \
	utilities.h
PA2_SOURCES = \
	handle_flags.cc \
	lextest.cc \
	stringtab.cc \
	utilities.cc \
	mycoolc

PA2J_LIBS = parser semant cgen
PA2J_SOURCES = \
	AbstractSymbol.java \
	AbstractTable.java \
	BoolConst.java \
	Flags.java \
	IdSymbol.java \
	ClassTable.java \
	IdTable.java \
	IntSymbol.java \
	IntTable.java \
	Lexer.java \
	StringSymbol.java \
	StringTable.java \
	TokenConstants.java \
	Utilities.java \
	CgenSupport.java \
	mycoolc


PA3_LIBS = lexer semant cgen 
PA3_INCLUDES = \
	cgen_gc.h \
	cool-parse.h \
	cool-io.h \
	cool-tree.h \
	cool.h \
	copyright.h \
	list.h \
	stringtab.h \
	stringtab_functions.h \
	tree.h \
	utilities.h
PA3_SOURCES = \
	cool-tree.aps \
	cool-tree.cc \
	dumptype.cc \
	handle_flags.cc \
	parser-phase.cc \
	stringtab.cc \
	tokens-lex.cc \
	tree.cc \
	utilities.cc \
	mycoolc \
	myparser 

PA3J_LIBS = lexer semant cgen
PA3J_SOURCES = \
	AbstractSymbol.java \
	AbstractTable.java \
	BoolConst.java \
	CgenClassTable.java \
	CgenNode.java \
	CgenSupport.java \
	ClassTable.java \
	CoolTokenLexer.java \
	Flags.java \
	IdSymbol.java \
	IdTable.java \
	IntSymbol.java \
	IntTable.java \
	ListNode.java \
	Parser.java \
	StringSymbol.java \
	StringTable.java \
	SymbolTable.java \
	TreeConstants.java \
	TreeNode.java \
	Utilities.java \
	cool-tree.aps \
	cool-tree.java \
	mycoolc \
	myparser


PA4_LIBS = lexer parser cgen
PA4_INCLUDES = \
	ast-parse.h \
	cgen_gc.h \
	cool-parse.h \
	cool-io.h \
	cool.h \
	copyright.h \
	list.h \
	stringtab.h \
	stringtab_functions.h \
	symtab.h \
	tree.h \
	utilities.h
PA4_SOURCES =  \
	ast-lex.cc \
	ast-parse.cc \
	cool-tree.cc \
	dumptype.cc \
	handle_flags.cc \
	semant-phase.cc \
	stringtab.cc \
	symtab_example.cc  \
	tree.cc \
	utilities.cc \
	cool-tree.aps \
	mycoolc \
	mysemant

PA4J_LIBS = lexer parser cgen
PA4J_SOURCES = \
	ASTConstants.java \
	ASTLexer.java \
	ASTParser.java \
	AbstractSymbol.java \
	AbstractTable.java \
	BoolConst.java \
	Flags.java \
	IdSymbol.java \
	IdTable.java \
	IntSymbol.java \
	CgenSupport.java \
	IntTable.java \
	ListNode.java \
	Semant.java \
	StringSymbol.java \
	StringTable.java \
	SymbolTable.java \
	SymtabExample.java \
	TreeNode.java \
	TokenConstants.java \
	Utilities.java \
	cool-tree.aps \
	mycoolc \
	mysemant

PA5_LIBS = lexer parser semant
PA5_INCLUDES = \
	ast-parse.h \
	cgen_gc.h \
	cool-parse.h \
	cool-io.h \
	cool-tree.h \
	cool.h \
	copyright.h \
	list.h \
	stringtab.h \
	stringtab_functions.h \
	symtab.h \
	tree.h \
	utilities.h
PA5_SOURCES = \
	ast-lex.cc \
	ast-parse.cc \
	cgen-phase.cc \
	cool-tree.cc \
	dumptype.cc \
	handle_flags.cc \
	stringtab.cc \
	tree.cc \
	utilities.cc \
	mycoolc

PA5J_LIBS = lexer parser semant
PA5J_SOURCES = \
	ASTConstants.java \
	ASTLexer.java \
	ASTParser.java \
	AbstractSymbol.java \
	AbstractTable.java \
	Cgen.java \
	ClassTable.java \
	Flags.java \
	IdSymbol.java \
	IdTable.java \
	IntTable.java \
	ListNode.java \
	StringTable.java \
	SymbolTable.java \
	SymtabExample.java \
	TokenConstants.java \
	TreeNode.java \
	Utilities.java \
	cool-tree.aps \
	mycoolc

# Make sure the directory hierarchy exists for the student distribution,
# copy over `etc/Makefile.SKEL' into the root of that hierarchy (setting
# the COOL_DIR and SCRIPT_SHELL variables to the appropriate values), and
# then call that makefile to instantiate all of the other skeleton files
# in the student distribution.  (See `etc/Makefile.SKEL' for details).
#
install_base: ${DIRECTORIES:%=${COOL_DIR}/%} \
	      ${ARCHITECTURES:%=${COOL_DIR}/bin/.%} \
	      ${BINARIES:%=${BIN}/%} \
              ${ETC_FILES:%=${COOL_DIR}/etc/%} \
	      ${BIN_FILES:%=${COOL_DIR}/bin/%} \
	      ${LIB_FILES:%=${COOL_DIR}/lib/%} \
              ${ASSIGNMENTS:%=${COOL_DIR}/assignments/%/Makefile.SKEL} \
	      ${COOL_DIR}/Makefile \
	      ${COOL_DIR}/examples/README \
	      ${COOL_DIR}/handouts/Makefile \
	      ${COOL_DIR}/doc/spim.ps.gz 
	cd ${COOL_DIR}; ${GMAKE} install;


#
# Copy over the files from `bin', `etc', and `lib'.
#
${patsubst %,${COOL_DIR}/bin/%,${BIN_FILES}}: ${COOL_DIR}/bin/%: ../bin/%
	cp $< $@; chmod ugo+rx $@;

${patsubst %,${COOL_DIR}/etc/%,${ETC_FILES}}: ${COOL_DIR}/etc/%: ../etc/%
	cp $< $@; chmod ugo+rx $@;

${patsubst %,${COOL_DIR}/lib/%,${LIB_FILES}}: ${COOL_DIR}/lib/%: ../lib/%
	cp $< $@; chmod ugo+r $@;

#
# Install the student's makefile
#
# It is important that RANLIB_V be listed before LIB_V in this sed script!
#
${COOL_DIR}/Makefile: ../etc/Makefile.SKEL
	sed -e 's#COOL_DIR_V#${COOL_DIR}#g' \
	    -e 's#SCRIPT_SHELL_V#${SCRIPT_SHELL}#g' \
	    -e 's#CLASS_V#${CLASS}#g' \
	    -e 's#ARCH_V#${ARCH_PROGRAM}#g' \
	    -e 's#AR_V#${AR}#g' \
	    -e 's#ARCHIVE_NEW_V#${ARCHIVE_NEW}#g' \
	    -e 's#RANLIB_V#${RANLIB}#g' \
	    -e 's#LIB_V#${LIB_FLAGS}#g' \
	    -e 's#ASSIGNMENTS_V#${ASSIGNMENTS}#g' < $< > tmp; \
	mv -f tmp $@; rm -f tmp; \


${patsubst %,${BIN}/%,${BINARIES}}: ${BIN}/%: ../bin/%
	rm -f $@
	cp $< $@
	chmod ugo+x $@;

#
# Install the files into the student's hierarchy needed for assignment ${ASSN}
#
install_assn: install_base ${COOL_DIR}/assignments/${ASSN}/Makefile.SKEL \
              ${foreach lib,${${ASSN}_LIBS},${LIB}/${lib}} \
              ${foreach inc,${${ASSN}_INCLUDES},${INC}/${inc}} \
              ${foreach src,${${ASSN}_SOURCES},${SRC}/${src}}

#
# Copy over the files from `assignments/${ASSIGNMENTS}/' in the instructor's
# distribution. The dependency is only on Makefile.SKEL, but all files with
# an extension (with a "." in the file name) are copied.
#
${patsubst %,${COOL_DIR}/assignments/%/Makefile.SKEL,${ASSIGNMENTS}}: ${COOL_DIR}/assignments/%/Makefile.SKEL: \
        ../assignments/%/Makefile.SKEL
	rm -f  ${dir $@}*
	cp ${dir $<}*.* ${dir $@}.
	chmod ugo+r ${dir $@}*

#
#  Copy over the example Cool programs.  The dependency is only
# on the README file, but all files are copied.
#
${COOL_DIR}/examples/README: ../examples/README
	rm -f ${dir $@}*
	cp ${dir $<}*.cl ${dir $@}.
	cp ${dir $<}README ${dir $@}.
	cp ${dir $<}*.graph ${dir $@}.
	chmod ugo+r ${dir $@}*

#
# copy over handouts and tool documentation.
#
${COOL_DIR}/handouts/Makefile: ../handouts/Makefile
#	rm -f ${dir $@}*
#	chmod ugo+r ${dir $@}*
#	cp ${dir $<}Makefile ${dir $@}.
#	cp ${dir $<}*.tex ${dir $@}.
#	cp ${dir $<}*.ps ${dir $@}.

${COOL_DIR}/doc/spim.ps.gz: ../doc/spim.ps.gz
	rm -f ${dir $@}*
	cp ${dir $<}*.* ${dir $@}.
	chmod ugo+r ${dir $@}*

# 
# Create the directories for the binaries for the various architectures.
#
${patsubst %,${COOL_DIR}/bin/.%,${ARCHITECTURES}}:
	mkdir -p $@;
	chmod ugo+rx $@

${foreach lib,${${ASSN}_LIBS},${LIB}/${lib}}: ${LIB}/%: ${COOL_DIR}/lib/.${ARCH}/%
	cd ${LIB}; rm -f $@; ln -s ../${notdir $<} $@


#
# For each directory needed by the hierarchy, create it.
#
${patsubst %,${COOL_DIR}/%,${DIRECTORIES}}:
	mkdir -p $@; chmod ugo+rx $@;

#
# Copy the libraries, include files, and source files needed by the students.
# All of these come from the instructor's src or javasrc directory.
# 
${COOL_DIR}/lib/.${ARCH}/%: ../src/%
	rm -f $@
	cp $< $@ 
	chmod ugo+r $@

${COOL_DIR}/include/${ASSN}/%: ../src/%
	rm -f $@
	cp $< $@
	chmod ugo+r $@

${COOL_DIR}/src/${ASSN}/%.java: ../javasrc/%.java
	rm -f $@
	cp $< $@
	chmod ugo+r $@

# sm: added the 'X' to the chmod, which means, if it's executable
# for anyone, make it executable for all; this fixes a problem
# where some binaries get installed u+x but og-x
${COOL_DIR}/src/${ASSN}/%: ../src/%
	rm -f $@
	cp $< $@
	chmod ugo+rX $@

../src/%:
	cd ../src; ${GMAKE} ${notdir $@}

../javasrc/%:
	cd ../javasrc; ${GMAKE} ${notdir $@}

../bin/%:
	cd ..; ${GMAKE} bin/${notdir $@}

#
# Create a uuencoded tarfile containing all of the files needed
# by a student for the architecture ${ARCH}.  These files are copied
# from the version of the student distribution in ${COOL_DIR}.
#
distrib: ${ETC_FILES:%=${COOL_DIR}/etc/%} ${BIN_FILES:%=${COOL_DIR}/bin/%} ${LIB_FILES:%=${COOL_DIR}/lib/%} \
         ${COOL_DIR}/Makefile ${COOL_DIR}/assignments ${COOL_DIR}/doc ${COOL_DIR}/examples ${COOL_DIR}/handouts ${COOL_DIR}/include ${COOL_DIR}/src \
         ${COOL_DIR}/lib/.${ARCH} ${patsubst %,${COOL_DIR}/bin/.${ARCH}/%,${BINARIES}}
	cd ${COOL_DIR}; \
	tar cvf ${ARCH}.tar ${^:${COOL_DIR}/%=%}; \
	mv ${ARCH}.tar ${COOL_DIR}/dist; \
	cd ${COOL_DIR}/dist; \
	gzip ${ARCH}.tar; \
	uuencode ${ARCH}.tar.gz ${ARCH}.tar.gz > ${ARCH}.u; \
	rm -f ${ARCH}.tar.gz; \
	chmod ogu+r ${ARCH}.u;




