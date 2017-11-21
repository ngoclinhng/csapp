CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3
INCLUDE=-I../include

all: shell

shell: shell.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

run: shell
	./shell

clean:
	$(RM) *.o shell
