# This make file si to be used as the base for the 
# other projects

CFLAGS=-c -ansi -Wall -Werror -pedantic -std=c++0x
NO_WARNING=-Wno-unused-variable

ZEROMQ_PREFIX=/usr/local/zeromq-3.2.4

if [ ! -d "$(ZEROMQ_PREFIX)" ]; then ZEROMQ_PREFIX=/usr/local/zeromq-3.2.2; fi

LIBS=-lzmq
INCLUDE_DIR=$(ZEROMQ_PREFIX)/include/
INCLUDE_FLAG=-I

LIBRARY_FLAG=-L
LIBS=-lzmq
LIB_PATH=-L$(ZEROMQ_PREFIX)/lib/
#/usr/local/boost-1.53.0/lib/



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

%.o: %.cpp 
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $(INCLUDE_FLAG)$(INCLUDE_DIR) $< 

server: zmq_server.o
	$(CC) $^ $(LIB_PATH) $(LIBS) -o $@ 

client: zmq_client.o
	$(CC) $^ $(LIB_PATH) $(LIBS) -o $@ 

.PHONY: clean

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.cpp
#	$(CC) $(CFLAGS) $< -o $@ 

all: server client

clean:
	rm -rf *.o server client
