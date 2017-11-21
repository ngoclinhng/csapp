CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3
INCLUDE=-I../include

all: waitpid1 waitpid2

waitpid1: waitpid1.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

waitpid2: waitpid2.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

run: waitpid1 waitpid2
	./waitpid1
	./waitpid2

clean:
	$(RM) *.o waitpid1 waitpid2
