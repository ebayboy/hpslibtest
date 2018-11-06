
EXE?= test
STATIC_EXE ?= $(EXE)

INCL := -I./include/
LIB_NAME := ./lib/libhps.a ./lib/libhs.a

all: app

DEBUG = -g -ggdb
CFLAGS = -lstdc++ -lm

app: 
	$(CC) $(CFLAGS) $(DEBUG) test.c cJSON.c log.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE)

clean:
	rm -rf $(STATIC_EXE)
