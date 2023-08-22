#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

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
	Kyber *kyber =  kyber_init();
	Poly *msg    = poly_init();
	Poly *dec    = poly_init();

	poly_gen_msg(N, msg);

	printf("MSG: ");
	poly_println(msg);
	printf("\n");

	kyber_keygen(kyber);

	printf("A: ");
	polyvec_print(kyber->a);
	printf("\n");

	printf("S: ");
	polyvec_print(kyber->s);
	printf("\n");

	printf("T: ");
	polyvec_print(kyber->t);
	printf("\n");
	
	kyber_encrypt(kyber, msg);

	printf("U: ");
	polyvec_print(kyber->u);
	printf("\n");

	printf("V: ");
	poly_println(kyber->v);
	printf("\n");

	kyber_decrypt(kyber, dec);

	printf("MSG: ");
	poly_println(dec);

	return 0;
}