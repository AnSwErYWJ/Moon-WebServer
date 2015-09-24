
CC = gcc

moon: moon.o
	$(CC) -o moon moon.o
moon.o: moon.c configure.h
	$(CC) -c moon.c

clean:
	rm -f *.o moon
