#ifndef POLYVEC_H_INCLUDED
#define POLYVEC_H_INCLUDED

typedef struct{
	Poly ***poly;

	int size_i;
	int size_j;
} Polyvec;

Polyvec* polyvec_init(int lines, int colums);
void polyvec_print(Polyvec *polyvec);
void polyvec_free(Polyvec *polyvec);

void polyvec_transpose(Polyvec *polyvec, Polyvec *transpose);
void polyvec_sum(Polyvec *a, Polyvec *b, Polyvec *result);
void polyvec_mul(Polyvec *a, Polyvec *b, Polyvec *result);

#endif