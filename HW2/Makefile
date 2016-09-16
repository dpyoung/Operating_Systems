#
# Makefile for Chapter 02
#
# Type  make    to compile all the programs
# in the chapter 
#
CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -Wpointer-arith

#all: who1 who2 who3 who3a cp1
all: who3a who3b cp1a

clean:
#	rm -f who1 who2 who3 cp1
	rm -f who3a who3b cp1a

#who1: who1.c
#	$(CC) $(CFLAGS) -o who1 who1.c
#
#who2: who2.c
#	$(CC) $(CFLAGS) -o who2 who2.c
#
#who3: who3.c utmplib.c
#	$(CC) $(CFLAGS) -o who3 who3.c utmplib.c
#
who3a: who3a.c utmplib2.c
	$(CC) $(CFLAGS) -o who3a who3a.c utmplib2.c

who3b: who3b.c utmplib.c
	$(CC) $(CFLAGS) -o who3b who3b.c utmplib.c

cp1a: cp1a.c
	$(CC) $(CFLAGS) -o cp1a cp1a.c

#
#cp1: cp1.c
#	$(CC) $(CFLAGS) -o cp1 cp1.c 

