CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O3
INCLUDE=-I../../include

SRC_DIR=../../src
INCLUDE_DIR=../../include

all: sigint kill_exp1 signal signal2 signalprob0 \
waitforsignal

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

signal: signal.o common.o
	$(CC)  -o $@ $^
signal.o: signal.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

signal2: signal2.o common.o
	$(CC)  -o $@ $^
signal2.o: signal2.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

signalprob0: signalprob0.o common.o
	$(CC)  -o $@ $^
signalprob0.o: signalprob0.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

waitforsignal: waitforsignal.o common.o
	$(CC)  -o $@ $^
waitforsignal.o: waitforsignal.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

run: sigint kill_exp1 signal signal2 signalprob0 waitforsignal
	./sigint
	./kill_exp1
	./signal
	./signal2
	./signalprob0
	./waitforsignal
clean:
	$(RM) *.o sigint kill_exp1 signal signal2 signalprob0 \
	waitforsignal
