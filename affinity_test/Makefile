# This make file si to be used as the base for the 
# other projects

CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x 
NO_WARNING=-Wno-unused-variable
GCCVERSION = $(gcc --version | grep ^gcc | sed 's/^.* //g')

ifndef $(CC)
	CC=g++
else ifeq ($(TOOL),clang)
	CC=clang++
endif

ifeq ($(CC),"g++")
ifeq ($(GCCVERSION), "4.8.1")
	CFLAGS+=-Wl,--no-as-needed
endif
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

objs: *.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) *.cpp 

driver:objs
	$(CC) -Wl,--no-as-needed *.o $(LIBDIR_FLAG)$(LIB_DIR) -o $@ $(LIBS)

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.cpp
#	$(CC) $(CFLAGS) $< -o $@ 

all:driver

clean:
	rm -rf *.o driver
