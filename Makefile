
CC=g++
CFLAGS =-c -Wall -Werror -ansi -pendantic

all: rshell

rshell: rshell.cc
	$(CC) rshell.cc -o rshell
rshell.o:
	$(CC) $(CFLAGS) rshell.cc
clean:
	rm *o hello


