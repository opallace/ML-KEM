#ifndef POLY_H_INCLUDED
#define POLY_H_INCLUDED

typedef struct{
	int *coeff;
	int size;
} Poly;

Poly* poly_init();

void poly_print(Poly *a);
void poly_println(Poly *a);
void poly_copy(Poly *a, Poly *b);
int poly_cmp(Poly *a, Poly *b);
void poly_rem_null_terms(Poly *a);
void poly_free(Poly *a);
void poly_trunc(Poly *a, int modulus);
void poly_fill(Poly *a, int new_size, int value);
int poly_is_zero(Poly *a);
int poly_degree(Poly *a);

void poly_mod_center(Poly *aa, int q, Poly *result);

void poly_gen_tern(int n, int d1, int d0, Poly *result);

void poly_sum(Poly *a, Poly *b, int modulus, Poly *result);
void poly_sub(Poly *a, Poly *b, int modulus, Poly *result);
void poly_int_mul(Poly *a, int num, int modulus, Poly *result);
void poly_mul(Poly *a, Poly *b, int modulus, Poly *result);
void poly_div(Poly *a, Poly *b, int modulus, Poly *result);
void poly_mod(Poly *a, Poly *b, int modulus, Poly *result);

void poly_gdc(Poly *a, Poly *b, int modulus, Poly *result);
void poly_gdce(Poly *a, Poly *b, int modulus, Poly *result);

int poly_is_invertible(Poly *a, Poly *ring, int modulus);
int poly_invert(Poly *a, Poly *ring, int modulus, Poly *result);

#endif