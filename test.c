#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "k_pke.h"

int poly_cmp(Poly *a, Poly *b){
    if(a->size == b->size){
        for (int i = 0; i < a->size; i++){
            if(a->coeff[i] != b->coeff[i]){
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

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

    int count = 0;
	
    for (int i = 0; i < 1000; i++){

        Kyber *kyber         = kyber_init();
        Poly  *msg           = poly_init();
        Poly  *msg_original  = poly_init();
        Poly  *msg_decifrada = poly_init();

        poly_gen_msg(N, msg);
        poly_copy(msg_original, msg);

        kyber_keygen(kyber);
        kyber_encrypt(kyber, msg);
        kyber_decrypt(kyber, msg_decifrada);

        if(poly_cmp(msg_original, msg_decifrada)){
            count++;
        }

        kyber_free(kyber);
        poly_free(msg);
        poly_free(msg_original);
        poly_free(msg_decifrada);

    }

    printf("%i/1000 sucesso.\n", count);

	return 0;
}
