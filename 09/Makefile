CC=g++
OPS=-c --std=c++14

all: test

run: prog
	./prog

test: prog
	./prog

prog: test.o
	$(CC) -o prog test.o -pthread

test.o: test.cpp bin.h
	$(CC) $(OPS) test.cpp

clean:
	rm -rf *.o prog

