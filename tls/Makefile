# Makefile
CC = gcc
CFLAGS = -Wall
CRYPTO = -lssl -lcrypto
ALL = client server

all: $(ALL)

client: client.o
	$(CC) $(CFLAGS) -o client client.c $(CRYPTO)

server: server.c
	$(CC) $(CFLAGS) -o server server.c $(CRYPTO)

clean:
	rm -f *.c $(ALL)
