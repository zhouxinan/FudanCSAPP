CC = gcc
CFLAGS = -pthread -O2

all: proxy.c csapp.c
	$(CC) $(CFLAGS) -o proxy proxy.c csapp.c
clean:
	rm -f proxy *~ *.o tmp/*

