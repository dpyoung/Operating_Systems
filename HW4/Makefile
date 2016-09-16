#
# Makefile for Chapter 03
#
# Type  make    to compile all the programs
# in the chapter 
#   
CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -Wpointer-arith

all: ls5 mv

clean:
	rm -f ls5 mv

ls5: ls5.c 
	$(CC) $(CFLAGS) -o ls5 ls5.c 

mv: mv.c 
	$(CC) $(CFLAGS) -o mv mv.c 

