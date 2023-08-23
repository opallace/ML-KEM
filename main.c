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
	
	Kyber *kyber   =  kyber_init();
	Poly *msg      = poly_init();
	Poly *dec      = poly_init();

	poly_gen_msg(N, msg);
	
	clock_t t;
	int f;
  	
	t = clock();
	for(int i = 0; i < 1000; i++){
		kyber_keygen(kyber);
	}
	t = clock() - t;
  	printf ("keygen() %f seconds\n",((float)t)/CLOCKS_PER_SEC);
	
	t = clock();
	for(int i = 0; i < 1000; i++){
		kyber_encrypt(kyber, msg);
	}
	t = clock() - t;
  	printf ("kyber_encrypt() %f seconds\n",((float)t)/CLOCKS_PER_SEC);

	t = clock();
	for(int i = 0; i < 1000; i++){
		kyber_decrypt(kyber, dec);
	}
	t = clock() - t;
  	printf ("kyber_decrypt() %f seconds\n",((float)t)/CLOCKS_PER_SEC);

	return 0;
}
