#
# Makefile for Chapter 03
#
# Type  make    to compile all the programs
# in the chapter 
#   
CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -Wpointer-arith

all: ls3 ls4 cp2 touch

clean:
	rm -f ls3 ls4 cp2 touch

ls3: ls3.c 
	$(CC) $(CFLAGS) -o ls3 ls3.c 

ls4: ls4.c 
	$(CC) $(CFLAGS) -o ls4 ls4.c 

cp2: cp2.c 
	$(CC) $(CFLAGS) -o cp2 cp2.c 

touch: touch.c 
	$(CC) $(CFLAGS) -o touch touch.c 

