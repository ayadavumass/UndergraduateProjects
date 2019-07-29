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

#
# Makefile for installing the Cool system.
#

#
# If you are installing on multiple platforms, you must run 
#
# % gmake clean
#
# to remove binaries compiled for one architecture prior to
# installing on the next architecture.
#

#
# Change COOL_DIR to the name of the root directory of the Cool hierarchy
# students will use.  This should not be the same as the root directory in 
# which the instructor's distribution is installed.  COOL_DIR
# is used to tell `spim' and `xspim' where to find the trap handler, and to
# tell the `install' and `distrib' targets where the student's distribution
# is located.
#
COOL_DIR = /home/aditya/Desktop/6thsem/compilers/SD

# bhackett
#COOL_DIR = /usr/class/cs143/test_build

# 
# Change ARCHITECTURES to a list of unique names identifying each of the
# architectures for which you will build binaries.
#
ARCHITECTURES = i686 # sun4 # snake i86pc

#
# Change CLASS to the name or course number of the class.
#
CLASS = cs143

#
# LIB_FLAGS should be the -L and -l flags to g++ needed to include -lfl in
# linking.  LIB_FLAGS must include at least -lfl.
#
LIB_FLAGS = -L/lib -lfl -R/lib


# The cool distribution is designed to support multiple architectures
# on a single file system.  The mechanism used in Cool is a shell
# program "ARCH" that returns a string from ${ARCHITECTURES} identifying
# the architecture being used; name of the architecture is used to
# select the correct executable file.
#
# For example, if you have HP's and DECstations, then sitting at an HP
# and running ARCH should return `snake' while running the same command
# on a DECstation should return `pmax'.  The list of architecture names
# and the machine/OS configurations they correspond to is given below as
# well as in the installation notes.  # If you want to support Cool on
# multiple architectures you will need an ARCH program and all students
# will need access to it. Edit the following line to set ARCH to the
# (preferably full pathname of) this program on your system.  If you
# plan a single architecture installation, comment out this line.
#
ARCH= arch

# If you only want to install Cool on one architecture, comment out the preceding line
# and uncomment the one appropriate line below.
#
#ARCH= echo alpha    # for DEC Alpha running OSF
#ARCH= echo pmax     # for DECStation running Ultrix
#ARCH= echo snake    # HP running HPUX
#ARCH= echo sun4     # A sun4
#ARCH= echo i586     # Pentium running Linux
#ARCH= echo rs6000   # IBM RS6000 running AIX

#
# Change the following variables to point to the corresponding programs.
# If you are compiling for multiple platforms, note that you may need to
# reset these variables between compiles for each individual architecture.
#
# GNU's `make' is needed for some of the makefiles; the standard `make'
# doesn't work on all systems.  You may need to change this line to
# "= make" if your system has GNU make installed as the default make.
#
# Similarly, gar (the GNU archiver) is the preferred archiver.  If you
# don't have gar, it may be necessary on some systems to modify the flags
# passed to ar in src/Makefile.
#
GMAKE = make                         #  GNU make
AR = gar                              #  AR names an archiver
SCRIPT_SHELL = /bin/bash      #  SCRIPT_SHELL is an sh-compatible shell
EXEC_CPP = /bin/cpp                   #  EXEC_CPP is a C preprocessor

#
# If you are using gar, you don't need to change the following settings.
# If you are not using gar, ARCHIVE_NEW should be the flags needed to
# create a new archive, and RANLIB should be a program that builds an
# archive's symbol table.

ARCHIVE_NEW = -cr
RANLIB = gar -qs

#  Remember that a few changes also need to be made to the
#  cool/src/Makefile.  See that file for instructions.

#  STOP
#
#  It should not be necessary to change anything below this line to
#  install the system.
#

ASSIGNMENTS = PA1 PA2 PA2J PA3 PA3J PA4 PA4J PA5 PA5J

#
# Allow for site-specific changes to the above defaults
#
-include site-config.mk

#
# Build spim, the aps tree package, and the coolc compiler.
# 
# bhackett: removed bin/xspim
all:	lib/trap.handler bin/spim bin/aps2c++ bin/coolc bin/lexer bin/parser bin/semant bin/cgen
	@echo "Cool build complete."

#
# for spim, we automatically install required path names, run xmkmf, 
# and force the makefile to use gcc, since the instructional lab's cc doesn't
# work.(This also means we delete CCOPTIONS, since it doesn't apply to gcc).
# spim/Configure was editted to default to gcc instead of cc.
#
spim/Makefile: ./Makefile spim/Imakefile.original
	cd ./spim; CC=gcc ./Configure
	cp spim/Imakefile.original spim/Imakefile.tmp
	sed -e "/^TRAP_DIR =/s# = [a-zA-Z0-9/_\.-]*# = ${COOL_DIR}/lib#"     \
	    -e "/^BIN_DIR =/s# = [a-zA-Z0-9/_\.-]*# = ${COOL_DIR}/bin#"      \
	    -e "/^MAN_DIR =/s# = [a-zA-Z0-9/_\.-]*# = ${COOL_DIR}/man#"      \
	    ./spim/Imakefile.tmp > ./spim/Imakefile
	rm -f ./spim/Imakefile.tmp
	cd ./spim; CC=gcc xmkmf
	cp spim/Makefile spim/Makefile.tmp
	sed -e "/CCOPTIONS =/s# = .*# = #" \
	    -e "/CC =/s# = cc# = gcc#" \
		./spim/Makefile.tmp > ./spim/Makefile
	rm -f ./spim/Makefile.tmp

bin/spim: spim/Makefile
	cd ./spim; ${GMAKE} MAKEFLAGS= spim ARCH=`${ARCH}`
	cp ./spim/spim ./bin/spim

# bhackett
#bin/xspim: spim/Makefile
#	cd ./spim; ${GMAKE} MAKEFLAGS= xspim ARCH=`${ARCH}`
#	cp ./spim/xspim ./bin/xspim

bin/aps2c++ bin/aps2java:
	cd ./aps; ${GMAKE} ARCH=`${ARCH}`
	cp ./aps/aps2c++ ./bin/aps2c++
	cp ./aps/aps2java ./bin/aps2java

#
# Build the coolc compiler and its separate phases.
#
bin/coolc bin/lexer bin/parser bin/semant bin/cgen: bin/aps2c++
	${GMAKE} -C ./src install ARCH=`${ARCH}`

src/coolc-test src/lexer-test src/parser-test src/semant-test src/cgen-test: bin/aps2c++
	${GMAKE} -C ./src test-executables ARCH=`${ARCH}`

java:
	${GMAKE} -C ./javasrc 

test: src/coolc-test src/lexer-test src/parser-test src/semant-test src/cgen-test
	cd ./regression; ${GMAKE} all



#
# Install the student's distribution in the ${COOL_DIR} directory.
# It will install the binaries appropriate for the architecture
# in which `make' is being run.  All of the `install*' dependencies
# call `gmake' on `install/Makefile' with environment variables set
# to indicate the architecture being installed, the root directory
# of the student's distribution, and the shell to use in scripts.
#
install: install_base ${ASSIGNMENTS:%=install_%} 

#
# Create the directory hierarchy in the student's distribution, and
# install the binaries.
#
install_base: all
	export ARCH; ARCH=`${ARCH}`; \
	export ARCH_PROGRAM; ARCH_PROGRAM="${ARCH}"; \
	export COOL_DIR; COOL_DIR=${COOL_DIR}; \
	export SCRIPT_SHELL; SCRIPT_SHELL=${SCRIPT_SHELL}; \
	export AR; AR=${AR}; \
	export ARCHIVE_NEW; ARCHIVE_NEW="${ARCHIVE_NEW}"; \
	export RANLIB; RANLIB="${RANLIB}"; \
	export GMAKE; GMAKE=${GMAKE}; \
	export ARCHITECTURES; ARCHITECTURES="${ARCHITECTURES}"; \
	export ASSIGNMENTS; ASSIGNMENTS="${ASSIGNMENTS}"; \
	export LIB_FLAGS; LIB_FLAGS="${LIB_FLAGS}"; \
	export CLASS; CLASS="${CLASS}"; \
	cd install; ${GMAKE} install_base;

#
# Set up the directories needed for the assignments.  Sets the
# environment variable ASSN to indicate which assignment to install.
#
${patsubst %,install_%,${ASSIGNMENTS}}:
	export ARCH; ARCH=`${ARCH}`; \
	export ARCH_PROGRAM; ARCH_PROGRAM="${ARCH}"; \
	export COOL_DIR; COOL_DIR=${COOL_DIR}; \
	export SCRIPT_SHELL; SCRIPT_SHELL=${SCRIPT_SHELL}; \
	export AR; AR=${AR}; \
	export ARCHIVE_NEW; ARCHIVE_NEW="${ARCHIVE_NEW}"; \
	export RANLIB; RANLIB="${RANLIB}"; \
	export GMAKE; GMAKE=${GMAKE}; \
	export ASSN; ASSN=${patsubst install_%,%,$@}; \
	export ARCHITECTURES; ARCHITECTURES="${ARCHITECTURES}"; \
	export ASSIGNMENTS; ASSIGNMENTS="${ASSIGNMENTS}"; \
	export LIB_FLAGS; LIB_FLAGS="${LIB_FLAGS}"; \
	export CLASS; CLASS="${CLASS}"; \
	cd install; ${GMAKE} install_assn



#
# `distrib_ARCH' - tar up a student's distribution for the
# architecture `ARCH'.  The binaries for the architecture
# should already have been created in the master version of
# the student's distribution by running `gmake install' from
# a machine with architecture `ARCH'.  Calls `gmake' on
# `install/Makefile' with the environment variables set
# to indicate the architecture, root directory, and shell
# to use in scripts.
#
${patsubst %,distrib_%,${ARCHITECTURES}}:
	export ARCH; ARCH=${patsubst distrib_%,%,$@}; \
	export COOL_DIR; COOL_DIR=${COOL_DIR}; \
	export SCRIPT_SHELL; SCRIPT_SHELL=${SCRIPT_SHELL}; \
	export ARCHITECTURES; ARCHITECTURES="${ARCHITECTURES}"; \
	export ASSIGNMENTS; ASSIGNMENTS="${ASSIGNMENTS}"; \
	export LIB_FLAGS; LIB_FLAGS="${LIB_FLAGS}"; \
	export CLASS; CLASS="${CLASS}"; \
	cd install; ${GMAKE} distrib

clean: 
	-cd ./aps; ${GMAKE} clean
# ensure the following command succeeds even if the Makefile does not exist
	-cd ./spim; ${GMAKE} MAKEFLAGS= clean; rm Makefile; echo ""
	-cd ./src; ${GMAKE} clean
	-cd ./regression; ${GMAKE} clean
	-cd ./bin; rm -f coolc parser semant cgen spim xspim aps2c++

#
# Removes all files that can be recreated.
#
maintainer-clean: 
	-cd ./src; ${GMAKE} maintainer-clean;
	-cd ./aps; ${GMAKE} clean;
# ensure the following command succeeds even if the Makefile does not exist
	-cd ./spim; ${GMAKE} MAKEFLAGS= clean; rm Makefile; echo ""
	-cd ./regression; ${GMAKE} clean;
	-cd ./bin; rm -f coolc parser semant cgen spim xspim aps2c++
