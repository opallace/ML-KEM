#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

Poly* poly_init(){
	Poly* result = malloc(sizeof(Poly));
	result->size = 0;
	return result;
}

void poly_print(Poly *a){
	for (int i = 0; i < a->size; ++i){
		if(a->coeff[i] != 0){
			printf("%ix^%i ", a->coeff[i], i);
		}
	}
}

void poly_println(Poly *a){
	for (int i = 0; i < a->size; ++i){
		if(a->coeff[i] != 0){
			printf("%ix^%i ", a->coeff[i], i);
		}
	}

	printf("\n\n");
}

void poly_copy(Poly *a, Poly *b){

	a->coeff = malloc(b->size * sizeof(int));
	a->size  = b->size;

	memcpy(a->coeff, b->coeff, b->size * sizeof(int));
}

/* se a > b  = 1
 * se a == b = 0
 * se a < b  = -1
 */
int poly_cmp(Poly *a, Poly *b){
	if(a->size == b->size){

		if(a->coeff[a->size - 1] == b->coeff[b->size - 1]){
			return 0;

		}else {
			if(a->coeff[a->size - 1] > b->coeff[b->size - 1]){
				return 1;

			}else {
				return -1;

			}
		}

	}else {
		if(a->size > b->size){
			return 1;
		}else {
			return -1;
		}
	}
}

void poly_rem_null_terms(Poly *a){
	for (int i = a->size - 1; a->coeff[i] == 0 && i > 0; i--){
		a->coeff = realloc(a->coeff, --a->size * sizeof(int));
	}
}

void poly_free(Poly *a){
	free(a->coeff);
	free(a);
}

void poly_trunc(Poly *a, int q){
	for (int i = 0; i < a->size; ++i){
		a->coeff[i] = mod(a->coeff[i], q);
	}

	poly_rem_null_terms(a);
}

void poly_fill(Poly *a, int new_size, int value){

	a->coeff = realloc(a->coeff, new_size * sizeof(int));
	
	for (int i = a->size; i < new_size; i++){
		a->coeff[i] = value;	
	}

	a->size = new_size;
}

int poly_is_zero(Poly *aa){
	Poly *a = poly_init();
	poly_copy(a, aa);
	
	poly_rem_null_terms(a);

	if(a->size == 1 && a->coeff[0] == 0){
		poly_free(a);
		return 1;
	
	}else {
		poly_free(a);
		return 0;
	
	}
}

int poly_degree(Poly *aa){
	Poly *a = poly_init();
	poly_copy(a, aa);
	
	poly_rem_null_terms(a);

	int degree = a->size - 1;

	poly_free(a);
	return degree;
}


void poly_mod_center(Poly *aa, int q, Poly *result){
	Poly *a = poly_init();
	poly_copy(a, aa);

	for (int i = 0; i < a->size; i++){
		if(a->coeff[i] > (int)(q / 2)){
			a->coeff[i] -= q;
		}
	}

	poly_copy(result, a);
	poly_free(a);
}

void poly_gen_tern(int n, int d1, int d0, Poly *result){
	srand(time(NULL));

	result->coeff = calloc(n + 1, sizeof(int));
	result->size = n + 1;

	for(int i = 0; i < d1;){
		int degree = rand() % (n + 1);

		if(result->coeff[degree] == 0){
			result->coeff[degree] = 1;
			i++;
		}
	}

	for(int i = 0; i < d0;){
		int degree = rand() % (n + 1);

		if(result->coeff[degree] == 0){
			result->coeff[degree] = -1;
			i++;
		}
	}

	poly_rem_null_terms(result);
}

void poly_gen(int n, Poly *result){
	srand(time(NULL));

	result->coeff = calloc(n + 1, sizeof(int));
	result->size = n + 1;

	for (int i = 0; i < n; i++){
		result->coeff[i] = rand() % Q;
	}

	result->coeff[n] = (rand() % (Q-1)) + 1;
}

void poly_gen_small(int n, Poly *result){
	srand(time(NULL));

	result->coeff = calloc(n + 1, sizeof(int));
	result->size = n + 1;

	for (int i = 0; i < n; i++){
		result->coeff[i] = (rand() % 3) - 1;
	}

	result->coeff[n] = 1;
}

void poly_sum(Poly *aa, Poly *bb, int modulus, Poly *result){
	
	Poly *temp_result = poly_init();
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	poly_trunc(a, modulus);
	poly_trunc(b, modulus);

	temp_result->size = max(a->size, b->size);
	temp_result->coeff = calloc(temp_result->size, sizeof(int));

	poly_fill(a, temp_result->size, 0);
	poly_fill(b, temp_result->size, 0);

	for (int i = 0; i < temp_result->size; i++){
		temp_result->coeff[i] = a->coeff[i] + b->coeff[i];
	}

	poly_trunc(temp_result, modulus);
	poly_rem_null_terms(temp_result);
	poly_copy(result, temp_result);

	poly_free(temp_result);
	poly_free(a);
	poly_free(b);
}

void poly_sub(Poly *aa, Poly *bb, int modulus, Poly *result){
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	poly_trunc(a, modulus);
	poly_trunc(b, modulus);

	result->size = max(a->size, b->size);
	result->coeff = calloc(result->size, sizeof(int));

	poly_fill(a, result->size, 0);
	poly_fill(b, result->size, 0);

	for (int i = 0; i < result->size; i++){
		result->coeff[i] = a->coeff[i] - b->coeff[i];
	}

	poly_trunc(result, modulus);
	poly_rem_null_terms(result);

	poly_free(a);
	poly_free(b);
}

void poly_int_mul(Poly *aa, int num, int modulus, Poly *result){
	Poly *a = poly_init();

	poly_copy(a, aa);
	poly_trunc(a, modulus);

	for (int i = 0; i < a->size; i++){
		a->coeff[i] *= num;
	}

	poly_trunc(a, modulus);
	poly_rem_null_terms(a);

	poly_copy(result, a);

	poly_free(a);
}

void poly_mul(Poly *aa, Poly *bb, int modulus, Poly *result){
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	poly_trunc(a, modulus);
	poly_trunc(b, modulus);

	result->coeff = calloc(a->size + b->size - 1, sizeof(int));
	result->size  = a->size + b->size - 1;

	for (int i = 0; i < a->size; i++){
		for (int j = 0; j < b->size; j++){
			result->coeff[i+j] += a->coeff[i] * b->coeff[j];
		}
	}

	poly_trunc(result, modulus);
	poly_rem_null_terms(result);

	poly_free(a);
	poly_free(b);

}

void poly_div(Poly *aa, Poly *bb, int modulus, Poly *result){
	if(!poly_is_zero(bb)){
		Poly *quotient = poly_init();
		quotient->coeff = malloc(sizeof(int));
		quotient->coeff[0] = 0;
		quotient->size = 1;

		Poly *remainder = poly_init();
		Poly *b = poly_init();

		poly_copy(remainder, aa);
		poly_copy(b, bb);

		poly_trunc(remainder, modulus);
		poly_trunc(b, modulus);

		while(!poly_is_zero(remainder) && poly_degree(remainder) >= poly_degree(b)){
			Poly *temp = poly_init();
			temp->coeff = calloc(((remainder->size - b->size) + 1), sizeof(int));
			temp->coeff[remainder->size - b->size] = remainder->coeff[remainder->size - 1] * ntruprime_inv_int(b->coeff[b->size - 1], modulus);
			temp->size = (remainder->size - b->size) + 1;

			poly_sum(quotient, temp, modulus, quotient);

			Poly *mul = poly_init();

			poly_mul(temp, b, modulus, mul);
			poly_sub(remainder, mul, modulus, remainder);

			poly_trunc(remainder, modulus);
			poly_trunc(quotient, modulus);

			poly_free(temp);
			poly_free(mul);
		}

		poly_rem_null_terms(quotient);
		poly_copy(result, quotient);

		poly_free(quotient);
		poly_free(remainder);
		poly_free(b);

	}else {
		printf("Error: division by zero.\n");

	}
}

void poly_mod(Poly *aa, Poly *bb, int modulus, Poly *result){
	if(!poly_is_zero(bb)){
		Poly *quotient = poly_init();
		quotient->coeff = malloc(sizeof(int));
		quotient->coeff[0] = 0;
		quotient->size = 1;

		Poly *remainder = poly_init();
		Poly *divisor = poly_init();

		poly_copy(remainder, aa);
		poly_copy(divisor, bb);

		poly_trunc(remainder, modulus);
		poly_trunc(divisor, modulus);

		while(!poly_is_zero(remainder) && poly_degree(remainder) >= poly_degree(divisor)){
			Poly *temp = poly_init();
			temp->coeff = calloc(((remainder->size - divisor->size) + 1), sizeof(int));
			temp->coeff[remainder->size - divisor->size] = remainder->coeff[remainder->size - 1] * ntruprime_inv_int(divisor->coeff[divisor->size - 1], modulus);
			temp->size = (remainder->size - divisor->size) + 1;

			poly_sum(quotient, temp, modulus, quotient);

			Poly *mul = poly_init();

			poly_mul(temp, divisor, modulus, mul);
			poly_sub(remainder, mul, modulus, remainder);

			poly_trunc(remainder, modulus);
			poly_trunc(quotient, modulus);

			poly_free(temp);
			poly_free(mul);
		}

		poly_rem_null_terms(remainder);
		poly_copy(result, remainder);

		poly_free(quotient);
		poly_free(remainder);
		poly_free(divisor);

	}else {
		printf("Error: division by zero.\n");

	}
}

/*  calculates the greatest common divisor between two big numbers 
 *	with the euclidean algorithm
 *
 *  Input: two big number pointers to calculate the mdc between 
 *         them, a big number pointer
 *  Output:
 *
 *	Pseudocode:  
 * 				while (y != 0)
 *	       			r = x % y
 *	       			x = y
 *	       			y = r
 *				return x;
 * 
 */
void poly_gdc(Poly *aa, Poly *bb, int modulus, Poly *result){
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	poly_trunc(a, modulus);
	poly_trunc(b, modulus);
	
	while(!poly_is_zero(b)){
		Poly *r = poly_init();

		poly_mod(a, b, modulus, r);

		poly_copy(a, b);
		poly_copy(b, r);

		poly_free(r);
	}

	poly_copy(result, a);

	poly_free(a);
	poly_free(b);
}

void poly_gdce(Poly *a, Poly *b, int modulus, Poly *result){

    Poly *r     = poly_init();
    Poly *new_r = poly_init();

    Poly *t     = poly_init();
    Poly *new_t = poly_init();
   
    poly_copy(new_r, a);
    poly_copy(r, b);

    poly_trunc(new_r, modulus);
    poly_trunc(r, modulus);
   
    t->coeff = malloc(sizeof(double));
    t->size  = 1;
    t->coeff[0] = 0;

    new_t->coeff = malloc(sizeof(double));
    new_t->size  = 1;
    new_t->coeff[0] = 1;
   
    while(!poly_is_zero(new_r)){
        Poly *quo = poly_init();
        poly_div(r, new_r, modulus, quo);
        
        Poly *mul = poly_init();

        Poly *temp_new_r = poly_init();
        poly_copy(temp_new_r, new_r);

        poly_mul(quo, new_r, modulus, mul);

        poly_sub(r, mul, modulus, new_r);
        poly_copy(r, temp_new_r);

        Poly *temp_new_t = poly_init();
        poly_copy(temp_new_t, new_t);

        poly_mul(quo, new_t, modulus, mul);
        poly_sub(t, mul, modulus, new_t);
        poly_copy(t, temp_new_t);

        poly_free(quo);
        poly_free(mul);
        poly_free(temp_new_r);
        poly_free(temp_new_t);
    }

    poly_div(t, r, modulus, result);

    poly_free(r);
    poly_free(t);
    poly_free(new_r);
    poly_free(new_t);
}

int poly_is_invertible(Poly *aa, Poly *ring, int modulus){
	Poly *gcd = poly_init();
	Poly *a   = poly_init();

	int is_invertible;

	poly_copy(a, aa);

	poly_gdc(a, ring, modulus, gcd);

	if(poly_degree(gcd) == 0){
		is_invertible = 1;
	}else {
		is_invertible = 0;
	}

	poly_free(gcd);
    poly_free(a);

    return is_invertible;
}

int poly_invert(Poly *aa, Poly *ring, int modulus, Poly *result){
	Poly *a = poly_init();

	poly_copy(a, aa);
	poly_trunc(a, modulus);

	if(is_prime(modulus) && poly_is_invertible(a, ring, modulus)){

		poly_gdce(a, ring, modulus, result);

	}else if(is_2_power(modulus) && poly_is_invertible(a, ring, 2)){

		poly_gdce(a, ring, 2, result);

		for (int i = 1; i < (int)log2(modulus); i++){
			
			Poly *mul1 = poly_init();
			poly_int_mul(result, 2, modulus, mul1);

			Poly *pow = poly_init();
			poly_mul(result, result, modulus, pow);

			Poly *mul2 = poly_init();
			poly_mul(a, pow, modulus, mul2);

			Poly *sub = poly_init();
			poly_sub(mul1, mul2, modulus, sub);

			poly_mod(sub, ring, modulus, result);

			poly_free(mul1);
        	poly_free(pow);
        	poly_free(mul2);
        	poly_free(sub);
		}
	}

    poly_free(a);

    return 1;
}