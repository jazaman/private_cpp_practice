# This make file si to be used as the base for the 
# other projects

CC=g++
CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x

objs: *.cpp
	$(CC) $(CFLAGS) *.cpp

driver:objs
	$(CC) *.o -o $@ 

objects = metaprogramming.o

all_objs:$(objects)

$(objects): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ 

all:driver

clean:
	rm -rf *.o driver
