# This make file si to be used as the base for the 
# other projects

CC=g++
CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable
LIBDIR_FLAG=-L
LIB_DIRS=
EXTRA_FLAG=


ifeq ($(TOOL),clang)
	CC=clang++
	EXTRA_FLAG=-stdlib=libc++
endif

ifndef LIBS
	LIBS=
endif

ifndef LIB_DIRS
	LIBDIR_FLAG=
else
	LIBDIR_FLAG=-L 
endif

ifeq ($(DEBUG),1)
	DEBUG_FLAG=-g
else
	DEBUG_FLAG=
endif
	

objs: *.cpp
	$(CC) $(CFLAGS) -pthread $(EXTRA_FLAG) $(DEBUG_FLAG) $(NO_WARNING) *.cpp

driver:objs
	$(CC) *.o $(LIBDIR_FLAG)$(LIB_DIRS) $(LIBS) -o $@ 

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.cpp
#	$(CC) $(CFLAGS) $< -o $@ 

all:driver

clean:
	rm -rf *.o driver
