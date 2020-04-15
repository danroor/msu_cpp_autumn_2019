CC=g++
OPS=-c --std=c++14

all: test

run: prog
	./prog

test: prog
	./prog

prog: test.o bigint.o
	$(CC) -o prog test.o bigint.o

test.o: test.cpp bigint.h
	$(CC) $(OPS) test.cpp

bigint.o: bigint.cpp bigint.h
	$(CC) $(OPS) bigint.cpp

clean:
	rm -rf *.o prog
