
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g --std=c99

main: msn-socket.c msn.c param-parser.c

all:
	main

dist:
	cd .. && tar cvzf dxdx.tar.gz src/*.c src/*.h src/Makefile 

clean:
	rm -f *.o
	rm -f main
	rm -f a.out
	rm -f core
