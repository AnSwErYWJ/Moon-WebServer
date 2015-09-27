CC = gcc

Moon: Moon.o function.o
	$(CC) -o Moon Moon.o function.o -lpthread
Moon.o: Moon.c function.h 
	$(CC) -c Moon.c -lpthread
function.o: function.c function.h
	$(CC) -c function.c
clean:
	rm -f *.o 
