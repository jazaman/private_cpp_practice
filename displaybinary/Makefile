# This make file si to be used as the base for the 
# other projects

CC=g++
CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable
INCLUDE_DIR=/usr/local/boost-1.53.0/include/
INCLUDE_FLAG=-I
LIBRARY_FLAG=-L
LIBRARIES=-lboost_program_options
LIBRARY_PATH=/usr/local/boost-1.53.0/lib/

ifeq ($(TOOL),clang)
	CC=clang++
endif

ifeq ($(DEBUG),1)
	DEBUG_FLAG=-g
else
	DEBUG_FLAG=
endif
	

objs: *.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(INCLUDE_FLAG)$(INCLUDE_DIR) $(NO_WARNING) *.cpp

driver:objs
	$(CC) *.o $(LIBRARY_FLAG)$(LIBRARY_PATH) $(LIBRARIES) -o $@ 

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.cpp
#	$(CC) $(CFLAGS) $< -o $@ 

all:driver

clean:
	rm -rf *.o driver
