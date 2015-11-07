CC=g++
CFLAGS =-c -Wall -Werror -ansi -pendantic

all: rshell

rshell: rshell.cc
	mkdir bin
	$(CC) rshell.cc -o bin/rshell
rshell.o:
	$(CC) $(CFLAGS) rshell.cc
clean:
	rm *o rshell


