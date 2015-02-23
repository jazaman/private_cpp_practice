# This make file si to be used as the base for the 
# other projects
SHELL:= /bin/bash 
CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable -Wno-unused-local-typedefs
TARGET=$(shell basename $(shell pwd)) #derive the executable name from hte directory


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

ifdef INCLUDE_DIR
        override INCDIR_FLAG=-I
else
	INCDIR_FLAG=
endif

ifdef STATIC 
        override STATIC_FLAG=-static
else
	STATIC_FLAG=
endif

%.o:%.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $(INCDIR_FLAG)$(INCLUDE_DIR) $< 

$(TARGET):%:%.o
	$(CC) $^ $(STATIC_FLAG) $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 

all:$(TARGET)

clean:
	rm -rf *.o $(TARGET) 
