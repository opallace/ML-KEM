all: main

main: aux.o poly.o polyvec.o kyber.o
	gcc main.c -o main aux.o poly.o polyvec.o kyber.o -lm

benchmark: aux.o poly.o polyvec.o kyber.o
	gcc benchmark.c -o benchmark aux.o poly.o polyvec.o kyber.o -lm

test: aux.o poly.o polyvec.o kyber.o
	gcc test.c -o test aux.o poly.o polyvec.o kyber.o -lm

aux.o: aux.c aux.h
	gcc -c aux.c

poly.o: poly.c poly.h
	gcc -c poly.c -lm

polyvec.o: polyvec.c polyvec.h
	gcc -c polyvec.c

kyber.o: kyber.c kyber.h
	gcc -c kyber.c

clean:
	rm -rf *.o
