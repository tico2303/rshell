CC=g++
CFLAGS =-c -Wall -Werror -ansi -pedantic
CFLAGS2 = -Wall -Werror -ansi -pedantic

OBJS = Command.o tree.o main.o

all: rshell

rshell: $(OBJS)
	mkdir bin
	$(CC) $(CFLAGS2) $(OBJS) -o bin/rshell

Command.o: Command.h Command.cc
	$(CC) $(CFLAGS) Command.h Command.cc

tree.o: tree.cc tree.h
	$(CC) $(CFLAGS) tree.h tree.cc

main.o: main.cc
	$(CC) $(CFLAGS) main.cc

clean:
	rm *.o
	rm *.h.gch
	rm -rf bin

