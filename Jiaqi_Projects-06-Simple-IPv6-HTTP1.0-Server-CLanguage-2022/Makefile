server: server.o utility.o
	gcc -pthread server.o utility.o -o server

server.o: server.c utility.h
	gcc -pthread server.c -c -Wall -g -o server.o

utility.o: utility.c utility.h 
	gcc utility.c -c -Wall -g -o utility.o

clean:
	rm -f server *.o
