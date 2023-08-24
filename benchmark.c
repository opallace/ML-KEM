#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

void poly_gen_msg(int n, Poly *result){
	result->coeff = calloc(n, sizeof(int));
	result->size  = n;

	for (int i = 0; i < n-1; i++){
		result->coeff[i] = rand() % 2;
	}

	result->coeff[n-1] = 1;
}

int main(){
	srand(time(NULL));
	
	Kyber *kyber =  kyber_init();
	Poly *msg    = poly_init();
	Poly *dec    = poly_init();

	poly_gen_msg(N, msg);
	
	clock_t t;
	int f;
  	
	t = clock();
	for(int i = 0; i < 1000; i++){
		kyber_keygen(kyber);
	}
	t = clock() - t;
  	printf ("keygen() %f segundos em 1000 execuções.\n",((float)t)/CLOCKS_PER_SEC);
	
	t = clock();
	for(int i = 0; i < 1000; i++){
		kyber_encrypt(kyber, msg);
	}
	t = clock() - t;
  	printf ("kyber_encrypt() %f segundos em 1000 execuções.\n",((float)t)/CLOCKS_PER_SEC);

	t = clock();
	for(int i = 0; i < 1000; i++){
		kyber_decrypt(kyber, dec);
	}
	t = clock() - t;
  	printf ("kyber_decrypt() %f segundos em 1000 execuções.\n",((float)t)/CLOCKS_PER_SEC);

	return 0;
}
