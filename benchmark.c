#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aux.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "k_pke.h"

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
	
	K_PKE *k_pke        =  k_pke_init();
	Poly *msg           = poly_init();
	Poly *msg_decifrada = poly_init();

	poly_gen_msg(N, msg);
	
	clock_t t;
	int f;
  	
	t = clock();
	for(int i = 0; i < 1000; i++){
		k_pke_keygen(k_pke);
	}
	t = clock() - t;
  	printf ("k_pke_keygen() %f segundos em 1000 execuções.\n",((float)t)/CLOCKS_PER_SEC);
	
	t = clock();
	for(int i = 0; i < 1000; i++){
		k_pke_encrypt(k_pke, msg);
	}
	t = clock() - t;
  	printf ("k_pke_encrypt() %f segundos em 1000 execuções.\n",((float)t)/CLOCKS_PER_SEC);

	t = clock();
	for(int i = 0; i < 1000; i++){
		k_pke_decrypt(k_pke, msg_decifrada);
	}
	t = clock() - t;
  	printf ("k_pke_decrypt() %f segundos em 1000 execuções.\n",((float)t)/CLOCKS_PER_SEC);

	k_pke_free(k_pke);
    poly_free(msg);
    poly_free(msg_decifrada);

	return 0;
}
