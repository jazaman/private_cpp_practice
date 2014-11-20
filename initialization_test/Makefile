# This make file si to be used as the base for the 
# other projects

CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable
TARGET=initialization_test


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

%.o:%.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $< 

initialization_test:%:%.o
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.cpp
#	$(CC) $(CFLAGS) $< -o $@ 

all:$(TARGET)
#initialization_test

clean:
	rm -rf *.o driver
