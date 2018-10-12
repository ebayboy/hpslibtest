
EXE?= hpslibtest
STATIC_EXE ?= $(EXE)

INCL := -I./include/
LIB_NAME := ./lib/hpslib.a

all: app

DEBUG = -g -ggdb

app: 
	$(CC) main.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE)

clean:
	rm -rf $(STATIC_EXE)
