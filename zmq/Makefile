# This make file si to be used as the base for the 
# other projects

CC=gcc
CFLAGS=-c -ansi -Wall -Werror -pedantic #-std=c++0x
NO_WARNING=-Wno-unused-variable
INCLUDE_FLAG=-I
LIBDIR_FLAG=-L
INCLUDE_DIR=/usr/local/zeromq-3.2.2/include
LIB_DIR=/usr/local/zeromq-3.2.2/lib
LIB=-lzmq

ifeq ($(DEBUG),1)
	DEBUG_FLAG=-g
else
	DEBUG_FLAG=
endif
	

objs: *.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) \
        $(INCLUDE_FLAG)$(INCLUDE_DIR)  *.c 

driver:objs
	$(CC) *.o -o $@ $(LIBDIR_FLAG)$(LIB_DIR) $(LIB)

#objects = metaprogramming.o 
#all_objs:$(objects)

#$(objects): %.o: %.c
#	$(CC) $(CFLAGS) $< -o $@ 

all:driver



weatherserver: wuserver.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(NO_WARNING) $(INCLUDE_FLAG)$(INCLUDE_DIR) $<  

weatherclient: 

clean:
	rm -rf *.o driver
