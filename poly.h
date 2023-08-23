#ifndef POLY_H_INCLUDED
#define POLY_H_INCLUDED

typedef struct{
	int *coeff;
	int size;
} Poly;

Poly* poly_init();

void poly_copy(Poly *a, Poly *b);
void poly_free(Poly *a);
void poly_fill(Poly *a, int new_size, int value);

void poly_gen(int n, Poly *result);
void poly_cbd(Poly *a, int eta, Poly *result);
void poly_compress(Poly *a, Poly *result);
void poly_decompress(Poly *a, Poly *result);

void poly_sum(Poly *a, Poly *b, Poly *result);
void poly_sub(Poly *a, Poly *b, Poly *result);
void poly_mul(Poly *a, Poly *b, Poly *result);

#endif