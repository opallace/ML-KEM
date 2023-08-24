#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    srand(time(NULL));
	
    Kyber *kyber         = kyber_init();
    Poly  *msg           = poly_init();
    Poly  *msg_original  = poly_init();
    Poly  *msg_decifrada = poly_init();

    poly_gen_msg(N, msg);
    poly_copy(msg_original, msg);

    kyber_keygen(kyber);
    kyber_encrypt(kyber, msg);
    kyber_decrypt(kyber, msg_decifrada);

    printf("MSG ORIGINAL: ");
    poly_println(msg_original);
    
    printf("A: ");
    polyvec_println(kyber->a);

    printf("S: ");
    polyvec_println(kyber->s);

    printf("T: ");
    polyvec_println(kyber->t);

    printf("U: ");
    polyvec_println(kyber->u);

    printf("V: ");
    poly_println(kyber->v);

    printf("MSG DECIFRADA: ");
    poly_println(msg_decifrada);

    kyber_free(kyber);
    poly_free(msg);
    poly_free(msg_original);
    poly_free(msg_decifrada);

    return 0;
}