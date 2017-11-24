CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3 -g3
INCLUDE=-I../../include

all: unix_shell

common.o: ../../src/common.c ../../include/common.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

unix_shell: unix_shell.o common.o
	$(CC) -o $@ $^
unix_shell.o: unix_shell.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

run: unix_shell
	./unix_shell

clean:
	$(RM) *.o unix_shell
