# This make file si to be used as the base for the 
# other projects

CC=g++
CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable -Wno-error=format

ifeq ($(DEBUG),1)
	DEBUG_FLAG=-g
else
	DEBUG_FLAG=
endif
	

objs: *.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) *.cpp

driver:objs
	$(CC) *.o -o $@ 

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.cpp
#	$(CC) $(CFLAGS) $< -o $@ 

all:driver

clean:
	rm -rf *.o driver
