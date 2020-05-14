CC=g++
OPS=-c --std=c++14

all: run

run: prog
	./prog

prog: exam.o
	$(CC) -o prog exam.o -pthread

exam.o: exam.cpp
	$(CC) $(OPS) exam.cpp

clean:
	rm -rf *.o prog
