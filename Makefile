
EXE?= hpslibtest
STATIC_EXE ?= static_$(EXE)

INCL := -I./include/
LIB_NAME := ./lib/hpslib.a

all: static_app

static_app: 
	$(CC) main.c $(INCL) $(LIB_NAME) -o $(STATIC_EXE)

clean:
	rm -rf $(STATIC_EXE)
