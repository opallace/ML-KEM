all: main benchmark test

main: aux.o poly.o polyvec.o k_pke.o ml_kem.o
	gcc main.c -o main aux.o poly.o polyvec.o k_pke.o ml_kem.o -lm

benchmark: aux.o poly.o polyvec.o k_pke.o ml_kem.o
	gcc benchmark.c -o benchmark aux.o poly.o polyvec.o k_pke.o ml_kem.o -lm

test: aux.o poly.o polyvec.o k_pke.o ml_kem.o
	gcc test.c -o test aux.o poly.o polyvec.o k_pke.o ml_kem.o -lm

aux.o: aux.c aux.h
	gcc -c aux.c

poly.o: poly.c poly.h
	gcc -c poly.c -lm

polyvec.o: polyvec.c polyvec.h
	gcc -c polyvec.c

k_pke.o: k_pke.c k_pke.h
	gcc -c k_pke.c

ml_kem.o: ml_kem.c ml_kem.h
	gcc -c ml_kem.c

clean:
	rm -rf *.o ; rm test ; rm benchmark ; rm main
