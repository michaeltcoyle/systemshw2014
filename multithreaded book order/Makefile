CC = gcc
CFLAGS = -Wall -g

all: order

order: src/bookorder.c src/structs.c src/structs.h src/node.c src/node.h src/queue.c src/queue.h
	$(CC) $(CFLAGS) -lpthread -o bookorder src/*.c

clean:
	rm -f *.o
	rm -f bookorder