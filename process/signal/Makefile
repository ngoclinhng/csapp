CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3
INCLUDE=-I../../include

SRC_DIR=../../src
INCLUDE_DIR=../../include

all: sigint kill_exp1

common.o: $(SRC_DIR)/common.c $(INCLUDE_DIR)/common.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

sigint: sigint.o common.o
	$(CC)  -o $@ $^
sigint.o: sigint.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

kill_exp1: kill_exp1.o common.o
	$(CC)  -o $@ $^
kill_exp1.o: kill_exp1.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

run: sigint kill_exp1
	./sigint
	./kill_exp1

clean:
	$(RM) *.o sigint kill_exp1
