CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3
INCLUDE=-I../include

all: kill_exp1

kill_exp1: kill_exp1.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

run: kill_exp1
	./kill_exp1

clean:
	$(RM) *.o kill_exp1
