#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

void teste_igual(Poly *a, Poly *b){
	printf("size a: %i\n", a->size);
	printf("size b: %i\n", b->size);

	printf("[");
	for (int i = 0; i < a->size; ++i){
		if(i == a->size - 1){
			if(a->coeff[i] == b->coeff[i]){
				printf("S");
			}else {
				printf("N");
			}
		}else {
			if(a->coeff[i] == b->coeff[i]){
				printf("S ");
			}else {
				printf("N ");
			}
		}
	}
	printf("]\n");
}

void poly_gen_msg(int n, Poly *result){
	sleep(1);
	srand(time(NULL));

	result->coeff = calloc(n, sizeof(int));
	result->size = n;

	for (int i = 0; i < n-1; i++){
		result->coeff[i] = rand() % 2;
	}

	result->coeff[n-1] = 1;
}

int main(){
	Kyber *kyber =  kyber_init();
	Poly *msg = poly_init();
	Poly *msgA = poly_init();
	Poly *dec = poly_init();

	poly_gen_msg(N, msg);
	poly_copy(msgA, msg);

	printf("MSG: ");
	poly_println(msg);

	kyber_keygen(kyber);

	// printf("A: ");
	// polyvec_print(kyber->a);
	// printf("S: ");
	// polyvec_print(kyber->s);
	// printf("E: ");
	// polyvec_print(kyber->e);
	// printf("T: ");
	// polyvec_print(kyber->t);

	kyber_encrypt(kyber, msg);
	kyber_decrypt(kyber, dec);
	teste_igual(msgA, dec);

	return 0;
}