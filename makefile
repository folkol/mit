# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc

# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: mit

mit:  mit_opts.o mit.o
	$(CC) mit.o mit_opts.o -o mit

mit.o: mit.c
	$(CC) $(CFLAGS) mit.c

mit_opts.o: mit_opts.c
	$(CC) $(CFLAGS) mit_opts.c

clean:
	rm -rf mit *.o