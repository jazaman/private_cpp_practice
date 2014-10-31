# This make file si to be used as the base for the 
# other projects

CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable
INCLUDE_DIR=-I/usr/local/boost-1.53.0/include
LIB_DIR=/usr/local/boost-1.53.0/lib
LIBS=-lboost_program_options


ifndef $(CC)
	CC=g++
else ifeq ($(TOOL),clang)
	CC=clang++
endif

ifndef LIBS
	LIBS=
endif

ifeq ($(DEBUG),1)
	DEBUG_FLAG=-g
else
	DEBUG_FLAG=
endif
	
ifdef LIB_DIR
        override LIBDIR_FLAG=-L
else
	LIBDIR_FLAG=
endif

#objs: *.cpp
#	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING)  *.cpp
%.o: %.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $(INCLUDE_DIR) $<

options_description:options_description.o
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 

first:first.o
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 

#objects = metaprogramming.o 
#all_objs:$(objects)


all:first options_description

clean:
	rm -rf *.o driver first options_description
