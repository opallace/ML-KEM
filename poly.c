#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

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
	printf("[");
	for (int i = 0; i < a->size; ++i){
		if(i == a->size - 1){
			printf("%i", a->coeff[i]);
		}else {
			printf("%i ", a->coeff[i]);
		}
	}
	printf("]");
}

void poly_println(Poly *a){
	printf("[");
	for (int i = 0; i < a->size; ++i){
		if(i == a->size - 1){
			printf("%i", a->coeff[i]);
		}else {
			printf("%i,", a->coeff[i]);
		}
	}
	printf("]\n");
}

void poly_copy(Poly *a, Poly *b){

	a->coeff = malloc(b->size * sizeof(int));
	a->size  = b->size;

	memcpy(a->coeff, b->coeff, b->size * sizeof(int));
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

void poly_gen(int n, Poly *result){
	result->coeff = calloc(n, sizeof(int));
	result->size = n;

	for (int i = 0; i < n-1; i++){
		result->coeff[i] = rand() % Q;
	}

	result->coeff[n-1] = (rand() % (Q-1)) + 1;
}

void poly_cbd(Poly *a, int eta, Poly *result){
	Poly *input_bytes = poly_init();
	poly_copy(input_bytes, a);

	int *bits = bytes_to_bits(input_bytes->coeff, input_bytes->size);
	result->coeff = malloc(N * sizeof(int));
	result->size  = N;

	for (int i = 0; i < N; i++){
		int sum_a = 0;
		int sum_b = 0;

		for (int j = 0; j < eta; j++){
			sum_a += bits[(2 * i * eta) + j];
		}

		for (int j = 0; j < eta; j++){
			sum_b += bits[(2 * i * eta) + eta + j];
		}

		result->coeff[i] = sum_a - sum_b;
	}
}

void poly_compress(Poly *a, Poly *result){
	int round_c(int val, int center, int bound) {
		int dist_center = abs(center - val);
		int dist_bound = val < bound - val ? val : bound - val;
		return dist_center < dist_bound ? center : 0;
	}

	int half_q = (Q / 2) + 0.5;

    for (int i = 0; i < a->size; i++) {
        result->coeff[i] = round_c(a->coeff[i], half_q, Q);
    }

    for (int i = 0; i < a->size; i++) {
        result->coeff[i] = result->coeff[i] / half_q;
    }
}

void poly_decompress(Poly *a, Poly *result){
	double decompress_float = Q / 2.0;

	for (size_t i = 0; i < a->size; i++){
		result->coeff[i] = (int)ceil(decompress_float * a->coeff[i]);
	}
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