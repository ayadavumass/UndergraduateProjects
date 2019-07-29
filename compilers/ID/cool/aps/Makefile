BISON= bison
FLEX= flex
CC= gcc
OBJ= aps.tab.o aps-lex.o aps-tree.o alloc.o string.o symbol.o symtab.o cgen.o \
	dump.o aps-tree-dump.o main.o

# link statically on Linux, because different distribs tend to have different 
# libraries
ifeq "${ARCH}" "i686"
LDFLAGS= -static
endif

all: aps2c++ aps2java

aps2c++: ${OBJ}
	${CC} ${CLFAGS} ${LDFLAGS} ${OBJ} -o $@

aps2java: aps2c++
	cp -f $< $@

aps.tab.h aps.tab.c aps.output : aps.y
	${BISON} --defines --verbose --debug -p aps_yy aps.y

aps-lex.c : aps.lex
	${FLEX} -t aps.lex > aps-lex.c

clean:
	rm -f *.o aps.tab.h aps.tab.c aps.output aps-lex.c ${EXEC}
