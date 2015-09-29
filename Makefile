CC = gcc

Moon: Moon.o fastCGI.o
	$(CC) -o Moon Moon.o fastCGI.o -lpthread
Moon.o: Moon.c Moon.h 
	$(CC) -c Moon.c -lpthread
fastCGI.o: fastCGI.c fastCGI.h
	$(CC) -c fastCGI.c 

clean:
	rm -f *.o 
