CC = gcc -m64 -g -no-pie
CFLAGS = -Wall -m64

all: main.o function.o
	$(CC) $(CFLAGS) -o example main.o function.o

function.o: function.s
	nasm -f elf64 -o function.o function.s

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

clean:
	rm -f *.o example
