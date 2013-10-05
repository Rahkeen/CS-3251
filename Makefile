#################################################################
##
## FILE:	Makefile
## PROJECT:	CS 3251 Project 1 - Professor Traynor
## DESCRIPTION: Compile Project 1
##
#################################################################

CC=gcc

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
    endif

all: client server 

client: client.c
	$(CC) -lssl -lcrypto listdir.h listdir.c sync.h client.c -o client

server: server.c
	$(CC) -lcrypto -pthread -lssl -lcrypto listdir.h listdir.c sync.h server.c -o server

clean:
	    rm -f client server *.o

