#ifndef KYBER_H_INCLUDED
#define KYBER_H_INCLUDED

#define N 4
#define K 4
#define Q 17
#define N1 2
#define N2 2

typedef struct{
	Polyvec *a;
	Polyvec *s;
	Polyvec *e;
	Polyvec *t;

	Polyvec *r;
	Polyvec *e1;
	Poly *e2;
	Polyvec *u;
	Poly *v;
} Kyber;

Kyber* kyber_init();
void kyber_keygen(Kyber *kyber);
void kyber_encrypt(Kyber *kyber, Poly *msg);
void kyber_decrypt(Kyber *kyber, Poly *msg);

#endif