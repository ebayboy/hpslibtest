
EXE?= test
STATIC_EXE ?= $(EXE)

STATIC_EXE2 ?= test2

INCL := -I./include/
LIB_NAME := ./lib/libhps.a ./lib/libhs.a

all: app

DEBUG = -g -ggdb
CFLAGS = -lstdc++ -lm

test2: 
	$(CC) $(CFLAGS) $(DEBUG) test2.c cJSON.c log.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE2)

app: 
	$(CC) $(CFLAGS) $(DEBUG) test.c cJSON.c log.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE)

clean:
	rm -rf $(STATIC_EXE) $(STATIC_EXE2)
