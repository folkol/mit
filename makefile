#vars
CC=gcc
CFLAGS=-c -Wall

#targets
all: mit

mit:  mit.o
	$(CC) mit.o -o mit

mit.o: mit.c
	$(CC) $(CFLAGS) mit.c

clean:
	rm -rf mit *.o