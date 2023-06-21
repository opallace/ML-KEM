#ifndef POLYVEC_H_INCLUDED
#define POLYVEC_H_INCLUDED

typedef struct{
	Poly ***poly;

	int size_i;
	int size_j;
} Polyvec;

Polyvec* polyvec_init();
void polyvec_print(Polyvec *polyvec);

#endif