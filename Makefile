CC=gcc 
CFLAGS= -Wall -Wextra -std=c99 -g
SOURCES= Galaxie.c CircularDoublyLinkedList.c
build:
	$(CC) $(CFLAGS) $(SOURCES) -o main

clean:
	rm -rf main

pack:
	zip -FSr 312CA_Spirache_Eduard-Mihai.zip README Makefile *.c *.h

.PHONY: clean pack
