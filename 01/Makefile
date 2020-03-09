CC=g++
OPS=-c --std=c++14

all: test

run: prog
	./prog

test: prog
	./prog

prog: test.o allocator.o
	$(CC) -o prog test.o allocator.o

test.o: test.cpp allocator.h
	$(CC) $(OPS) test.cpp

allocator.o: allocator.cpp allocator.h
	$(CC) $(OPS) allocator.cpp

clean:
	rm -rf *.o prog
