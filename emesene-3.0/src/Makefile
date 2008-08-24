
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g --std=c99

all: msn-socket.c msn.c param-parser.c

clean:
	find . -name \*.o -print | xargs rm -f
	find . -name main -print | xargs rm -f
	find . -name core -print | xargs rm -f
