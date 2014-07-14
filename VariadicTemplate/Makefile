# This make file si to be used as the base for the 
# other projects

CC=g++
CFLAGS=-c -ansi -Wall -pedantic -std=c++0x

test: VariadicTemplate.o
	$(CC) VariadicTemplate.o -o test

VariadicTemplate.o: VariadicTemplate.cpp
	$(CC) $(CFLAGS) VariadicTemplate.cpp

clean:
	rm -rf *.o test
