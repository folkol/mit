# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc

# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: mit

mit: mit_core.o
	$(CC) mit_core.o -o mit

mit_core.o: mit_core.c
	$(CC) $(CFLAGS) mit_core.c

clean:
	rm -rf mit *.o