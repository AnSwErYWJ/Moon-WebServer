CC = gcc

Moon: Moon.o function.o
	$(CC) -o Moon Moon.o function.o
Moon.o: Moon.c configure.h function.h configure.h
	$(CC) -c Moon.c
function.o: function.c function.h
	$(CC) -c function.c
clean:
	rm -f *.o 
