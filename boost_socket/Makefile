# This make file si to be used as the base for the 
# other projects
SHELL:= /bin/bash 
CFLAGS=-c -Wall -pedantic
NO_WARNING=-Wno-unused-variable
LIBS=-lwsock32 -lws2_32
#-lboost_system
TARGET=$(shell basename $(shell pwd)) #derive the executable name from hte directory
CLIENT=client
SERVER=server
CC=clang


ifndef $(CC)
	CC=clang++
else ifeq ($(CC),clang)
	override CC=clang++
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

ifdef INCLUDE_DIR
        override INC_DIR_FLAG=-I
else
	INC_DIR_FLAG=
endif


%.o:%.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $(INC_DIR_FLAG) $(INCLUDE_DIR) $< 
	
#SOURCE := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CLIENT_OBJS:=boost_client.o
SERVER_OBJS:=boost_server.o


#$(SOURCE:.cpp=.o): SOURCE
#	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $<
$(CLIENT):$(CLIENT_OBJS)
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) $(LIBS) -o $@ 
	
$(SERVER):$(SERVER_OBJS)
	$(CC) $^ $(LIBDIR_FLAG)$(LIB_DIR) -o $@ $(LIBS)

all:$(SERVER) 

clean:
	rm -rf *.o $(SERVER) 
