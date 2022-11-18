definitiva: main.o definitiva.o
	gcc -o definitiva main.o definitiva.o

main.o: main.c definitiva.h
	gcc -c main.c

definitiva.o: definitiva.c definitiva.h
	gcc -c definitiva.c

clean:	
	rm -f *.o definitiva




