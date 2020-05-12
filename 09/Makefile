CC=g++
OPS=-c --std=c++1z

all: test

run: prog
	./prog

test: prog
	./prog

prog: test.o bin.o
	$(CC) -o prog test.o bin.o -pthread

test.o: test.cpp bin.h
	$(CC) $(OPS) test.cpp

bin.o: bin.cpp bin.h
	$(CC) $(OPS) bin.cpp

clean:
	rm -rf *.o prog
