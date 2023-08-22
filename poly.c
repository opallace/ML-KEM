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


void poly_free(Poly *a){
	free(a->coeff);
	free(a);
}

void poly_trunc(Poly *a){
	for (int i = 0; i < a->size; ++i){
		a->coeff[i] = mod(a->coeff[i], Q);
	}
}

void poly_fill(Poly *a, int new_size, int value){

	a->coeff = realloc(a->coeff, new_size * sizeof(int));
	
	for (int i = a->size; i < new_size; i++){
		a->coeff[i] = value;	
	}

	a->size = new_size;
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
	int half_q = (Q / 2) + 0.5;

    for (int i = 0; i < a->size; i++) {
		int dist_center = abs(half_q - a->coeff[i]);
		int dist_bound = a->coeff[i] < Q - a->coeff[i] ? a->coeff[i] : Q - a->coeff[i];
        result->coeff[i] = dist_center < dist_bound ? half_q : 0;
		result->coeff[i] = result->coeff[i] / half_q;
    }
}

void poly_decompress(Poly *a, Poly *result){
	double decompress_float = Q / 2.0;

	for (size_t i = 0; i < a->size; i++){
		result->coeff[i] = (int)ceil(decompress_float * a->coeff[i]);
	}
}

void poly_sum(Poly *aa, Poly *bb, Poly *result){
	Poly *temp_result = poly_init();
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	poly_trunc(a);
	poly_trunc(b);

	temp_result->size  = N;
	temp_result->coeff = calloc(N, sizeof(int));

	poly_fill(a, N, 0);
	poly_fill(b, N, 0);

	for (int i = 0; i < temp_result->size; i++){
		temp_result->coeff[i] = a->coeff[i] + b->coeff[i];
	}

	poly_trunc(temp_result);
	poly_copy(result, temp_result);

	poly_free(temp_result);
	poly_free(a);
	poly_free(b);
}

void poly_sub(Poly *aa, Poly *bb, Poly *result){
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	poly_trunc(a);
	poly_trunc(b);

	result->size  = max(a->size, b->size);
	result->coeff = calloc(result->size, sizeof(int));

	poly_fill(a, N, 0);
	poly_fill(b, N, 0);

	for (int i = 0; i < result->size; i++){
		result->coeff[i] = a->coeff[i] - b->coeff[i];
	}

	poly_trunc(result);

	poly_free(a);
	poly_free(b);
}

void poly_mul(Poly *aa, Poly *bb, Poly *result){
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	result->coeff = calloc(N, sizeof(int));
	result->size  = N;

	for (int i = 0; i < result->size; i++){
		for (int j = 0; j < result->size; j++){
			if(i-j >= 0){
				result->coeff[i] += a->coeff[i-j] * b->coeff[j];
			}else {
				result->coeff[i] += -a->coeff[N+i-j] * b->coeff[j];
			}
		}
	}
	
	poly_trunc(result);
	poly_free(a);
	poly_free(b);
}