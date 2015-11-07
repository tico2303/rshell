
MKDIR = mkdir
.PHONY: directories
all: directories rshell
directories: ${OUT_DIR}
${OUT_DIR}:
	${MKDIR} ${OUT_DIR}

CC=g++
CFLAGS =-c -Wall -Werror -ansi -pendantic

all: rshell

rshell: rshell.cc
	$(CC) rshell.cc -o rshell
rshell.o:
	$(CC) $(CFLAGS) rshell.cc
clean:
	rm *o hello


