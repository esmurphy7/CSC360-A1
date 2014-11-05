# variables
CC=gcc
CFLAGS=-c -Wall

# target "all" executes other targets until its dependencies are met
all: shell.exe

# use -o option instead of CFLAGS to specify that we're using object files
shell.exe: shell.o parser.o linkedlist.o utilities.o
	$(CC) shell.o parser.o linkedlist.o utilities.o -o shell.exe

objects: shell.c parser.c linkedlist.c utilities.c
	$(CC) $(CFLAGS) shell.c shell.c parser.c linkedlist.c utilities.c

# remove executable and all .o files to start fresh
clean:
	rm -rf *o shell.exe