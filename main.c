#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

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
	printf("]\n\n");
}


void polyvec_print(Polyvec *polyvec){
	printf("[");
	for (int i = 0; i < polyvec->size_i; ++i){
		printf("[");
		for (int j = 0; j < polyvec->size_j; ++j){
			poly_print(polyvec->poly[i][j]);
			if(j < polyvec->size_j - 1){
				printf(",");
			}
		}
		printf("]");
		if(i < polyvec->size_i - 1){
			printf(",");
		}
	}
	printf("]\n\n");
}

void poly_gen_msg(int n, Poly *result){
	srand(time(NULL));
	
	result->coeff = calloc(n, sizeof(int));
	result->size  = n;

	for (int i = 0; i < n-1; i++){
		result->coeff[i] = rand() % 2;
	}

	result->coeff[n-1] = 1;
}

int main(){
	srand(time(NULL));
	
	Kyber *kyber  =  kyber_init();
	Poly *msg     = poly_init();
	Poly *msgg    = poly_init();
	Poly *dec     = poly_init();

	poly_gen_msg(N, msg);
	poly_copy(msgg, msg);

	printf("MSG: ");
	poly_println(msgg);
	printf("\n");

	kyber_keygen(kyber);

	printf("A: ");
	polyvec_print(kyber->a);

	printf("S: ");
	polyvec_print(kyber->s);

	printf("T: ");
	polyvec_print(kyber->t);
	
	kyber_encrypt(kyber, msg);

	printf("U: ");
	polyvec_print(kyber->u);

	printf("V: ");
	poly_println(kyber->v);

	kyber_decrypt(kyber, dec);

	printf("MSG: ");
	poly_println(dec);

	kyber_free(kyber);

	return 0;
}
