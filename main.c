#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

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
	Poly *dec = poly_init();

	// poly_gen_msg(N, msg);
	// poly_println(msg);
	msg->coeff = malloc(4 * sizeof(int));
	msg->size = 4;

	msg->coeff[0] = 1;
	msg->coeff[1] = 1;
	msg->coeff[2] = 0;
	msg->coeff[3] = 1;
	
	poly_println(msg);

	kyber_keygen(kyber);
	kyber_encrypt(kyber, msg);
	kyber_decrypt(kyber, dec);
	poly_println(dec);

	return 0;
}