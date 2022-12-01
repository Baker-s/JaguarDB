CC = g++
SRC = src/jg.cc
TEST = test/test.cc
OUT = jgdb
TOUT = jgtest 

LIB = lib/jgs.cc

.PHONY: test run

all: run

run: src/jg.cc
	${CC} -o build/${OUT} ${SRC} lib/jgs.cc
	./build/${OUT}

test: test/test.cc
	${CC} -o build/${TOUT} ${TEST} ${LIB}
	./build/${TOUT}
