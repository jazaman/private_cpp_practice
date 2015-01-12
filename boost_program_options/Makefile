# This make file si to be used as the base for the 
# other projects

CPPFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable -Wno-unused-local-typedefs
INCLUDE_DIR=-I/usr/local/boost-1.53.0/include
LIB_DIR=/usr/local/boost-1.53.0/lib
LIBS=-lboost_program_options
TARGET=first options_description 


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
%.o:%.cpp
	$(CC) $(CPPFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $(INCLUDE_DIR) $<

$(TARGET):%:%.o
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 

all:$(TARGET)

clean:
	rm -rf *.o driver first options_description
