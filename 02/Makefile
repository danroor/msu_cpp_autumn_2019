CC=g++
OPS=-c --std=c++14

all: test

run: prog
	./prog

test: prog
	./prog

prog: test.o parser.o
	$(CC) -o prog test.o parser.o

test.o: test.cpp parser.h
	$(CC) $(OPS) test.cpp

parser.o: parser.cpp parser.h
	$(CC) $(OPS) parser.cpp

clean:
	rm -rf *.o prog
