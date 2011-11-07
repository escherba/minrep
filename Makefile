CC=gcc
CFLAGS=-std=c99 -O2 -Wall -m64 -Wall -Wextra -pedantic -DNDEBUG
CFLAGS_DEBUG=-Wall -m64 -ggdb -DDEBUG -Wextra -pedantic -std=c99
LD=$(CC)
LDFLAGS=-lm
PROGRAM_NAME=canon

$(PROGRAM_NAME): $(PROGRAM_NAME).o minrep.o
	$(LD) -o $(PROGRAM_NAME) $(PROGRAM_NAME).o minrep.o $(LDFLAGS) 

$(PROGRAM_NAME).o: canon.c
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME).o -c canon.c

minrep.o: minrep.c
	$(CC) $(CFLAGS) -o minrep.o -c minrep.c

minrep.debug.o: minrep.c
	$(CC) $(CFLAGS_DEBUG) -o minrep.debug.o -c minrep.c

$(PROGRAM_NAME).debug.o: canon.c
	$(CC) $(CFLAGS_DEBUG) -o $(PROGRAM_NAME).debug.o -c canon.c

debug: $(PROGRAM_NAME).debug.o minrep.debug.o
	$(LD) -o $(PROGRAM_NAME) $(PROGRAM_NAME).debug.o minrep.debug.o $(LDFLAGS)

all: $(PROGRAM_NAME)

install:
	cp $(PROGRAM_NAME) ~/programs/bin/

check:
	./test_canon.pl

clean:
	rm $(PROGRAM_NAME) $(PROGRAM_NAME).o $(PROGRAM_NAME).debug.o minrep.o minrep.debug.o
