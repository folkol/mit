#vars
CC=gcc
CFLAGS=-c -Wall

#targets
all: mit

mit:  mit_object_store.o mit_commands.o mit.o sha1.o
	$(CC) mit_commands.o mit_object_store.o sha1.o mit.o -o mit

sha1.o: sha1.c
	$(CC) $(CFLAGS) sha1.c	

mit.o: mit.c
	$(CC) $(CFLAGS) mit.c

mit_commands.o: mit_commands.c
	$(CC) $(CFLAGS) mit_commands.c

mit_object_store.o: mit_object_store.c
	$(CC) $(CFLAGS) mit_object_store.c

clean:
	rm -rf mit *.o