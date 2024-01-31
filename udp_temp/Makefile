# Makefile
CC = gcc
CFLAGS = -Wall
ALL = client server

all: $(ALL)

client: client.o
	$(CC) $(CFLAGS) -o client client.c

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f *.c $(ALL)
