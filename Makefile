
EXE?= test
STATIC_EXE ?= $(EXE)

INCL := -I./include/
LIB_NAME := ./lib/libhps.a ./lib/libhs.a

all: app

test2: test2

DEBUG = -g -ggdb
CFLAGS = -lstdc++ -lm

app: 
	$(CC) $(CFLAGS) $(DEBUG) test.c cJSON.c log.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE)

test2: 
	$(CC) $(CFLAGS) $(DEBUG) test2.c cJSON.c log.c $(INCL) $(LIB_NAME) -o test2

clean:
	rm -rf $(STATIC_EXE)
