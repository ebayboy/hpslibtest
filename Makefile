
EXE?= test                                                                                   
STATIC_EXE ?= static_$(EXE)

INCL := -I./include/
LIB  := -L./lib/
LIB_NAME := test

all: static_app

static_app: 
	$(CC) main.c $(INCL) $(LIB) -l$(LIB_NAME) -o $(STATIC_EXE)

clean:
	rm -rf $(STATIC_EXE)
