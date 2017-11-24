CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3 -g3
INCLUDE=-I../../include

SRC_DIR=../../src
INCLUDE_DIR=../../include

all: shell

common.o: $(SRC_DIR)/common.c $(INCLUDE_DIR)/common.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

shell: shell.o common.o
	$(CC) -o $@ $^
shell.o: shell.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

run: shell
	./shell

clean:
	$(RM) *.o shell
