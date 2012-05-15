#vars
CC=gcc
CFLAGS=-c -Wall

#targets
all: mit

mit:  mit.o sha1.o
	$(CC) sha1.o mit.o -o mit

sha1.o: sha1.c
	$(CC) $(CFLAGS) sha1.c	

mit.o: mit.c
	$(CC) $(CFLAGS) mit.c

clean:
	rm -rf mit *.o