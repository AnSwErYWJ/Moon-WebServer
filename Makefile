CC = gcc

Moon: Moon.o function.o fastCGI.o
	$(CC) -o Moon Moon.o function.o fastCGI.o -lpthread
Moon.o: Moon.c function.h 
	$(CC) -c Moon.c -lpthread
function.o: function.c function.h fastCGI.h
	$(CC) -c function.c -lpthread
fastCGI.o: fastCGI.c fastCGI.h
	$(CC) -c fastCGI.c 

clean:
	rm -f *.o 
