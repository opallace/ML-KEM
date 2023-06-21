#ifndef KYBER_H_INCLUDED
#define KYBER_H_INCLUDED

typedef struct{
	int n;
	int k;
	int q;
	int n1;
	int n2;

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
void kyber_params_set(int n, int k, int q, int n1, int n2, Kyber *kyber);
void kyber_encrypt(Kyber *kyber, Poly *msg, Poly *result);
void kyber_decrypt(Kyber *kyber, Poly *msg, Poly *result);

#endif