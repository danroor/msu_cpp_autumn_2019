CC=g++
OPS=-c --std=c++14

all: test

run: prog
	./prog

test: prog
	./prog

prog: test.o matrix.o
	$(CC) -o prog test.o matrix.o

test.o: test.cpp matrix.h
	$(CC) $(OPS) test.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) $(OPS) matrix.cpp

clean:
	rm -rf *.o prog
