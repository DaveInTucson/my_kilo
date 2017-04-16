OBJ=kilo.o
CFLAGS=-Wall -Wextra -pedantic -std=c99

kilo: ${OBJ}
	$(CC) ${OBJ} -o kilo

kilo.o: kilo.c

clean:
	rm -f *~ *.o kilo
