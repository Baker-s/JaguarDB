CC = g++
SRC = src/jg.cc
TEST = test/test.cc
OUT = jgdb

.PHONY: test

test: src/jg.cc
	${CC} -o build/${OUT} ${SRC} lib/jgs.cc
	./build/${OUT}
