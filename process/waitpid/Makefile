CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3
INCLUDE=-I../../include

SRC_DIR=../../src
INCLUDE_DIR=../../include

all: waitpid1 waitpid2

common.o: $(SRC_DIR)/common.c $(INCLUDE_DIR)/common.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

waitpid1: waitpid1.o common.o
	$(CC)  -o $@ $^
waitpid1.o: waitpid1.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

waitpid2: waitpid2.o common.o
	$(CC)  -o $@ $^
waitpid2.o: waitpid2.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

run: waitpid1 waitpid2
	./waitpid1
	./waitpid2

clean:
	$(RM) *.o waitpid1 waitpid2
