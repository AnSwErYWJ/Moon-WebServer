
CC = gcc

moon: moon.o function.o
	$(CC) -o moon moon.o function.o
moon.o: moon.c configure.h function.h configure.h
	$(CC) -c moon.c
function.o: function.c function.h
	$(CC) -c function.c
clean:
	rm -f *.o 
