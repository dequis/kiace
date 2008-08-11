
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g --std=c99

main: msn-socket.o msn.c

all:
	main

dist:
	cd .. && tar cvzf dxdx.tar.gz src/*.c src/*.h src/Makefile 

clean:
	rm -f *.o
	rm -f a.out
	rm -f core
