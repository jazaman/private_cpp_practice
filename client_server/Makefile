# This make file si to be used as the base for the 
# other projects
SHELL:= /bin/bash 
CFLAGS=-c -ansi -Wall -Werror -pedantic
NO_WARNING=-Wno-unused-variable
LIBS=
TARGET=$(shell basename $(shell pwd)) #derive the executable name from hte directory
CLIENT=client
SERVER=server

ifndef $(CC)
	CC=g++
else ifeq ($(TOOL),clang)
	CC=clang++
endif

ifndef LIBS
	LIBS=
endif

ifeq ($(DEBUG),1)
	DEBUG_FLAG=-g -DDEBUG
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
	
#SOURCE := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CLIENT_OBJS:=client.o
SERVER_OBJS:=sample_server.o


#$(SOURCE:.cpp=.o): SOURCE
#	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $<
$(CLIENT):$(CLIENT_OBJS)
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 
	
$(SERVER):$(SERVER_OBJS)
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@

all:$(SERVER) $(CLIENT)

clean:
	rm -rf *.o $(SERVER) $(CLIENT) 
