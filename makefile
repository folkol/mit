# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc

# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: mit

mit: mit.o
	$(CC) mit.o -o mit

mit_core.o: mit.c
	$(CC) $(CFLAGS) mit.c

clean:
	rm -rf mit *.o