#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "k_pke.h"
#include "ml_kem.h"

int main(){
    srand(time(NULL));
	
    ML_KEM *ml_kem = ml_kem_init();

    ml_kem_keygen(ml_kem);

    printf("EK_PKE_T: ");
    polyvec_println(ml_kem->ek_pke_t);

    printf("EK_PKE_A: ");
    polyvec_println(ml_kem->ek_pke_a);

    printf("DK_PKE_S: ");
    polyvec_println(ml_kem->dk_pke_s);

    ml_kem_encapsulate(ml_kem);

    printf("K: ");
    poly_compress(ml_kem->k, ml_kem->k);
    poly_println(ml_kem->k);

    printf("C_U: ");
    polyvec_println(ml_kem->c_u);

    printf("C_V: ");
    poly_println(ml_kem->c_v);

    ml_kem_decapsulate(ml_kem, ml_kem->k);

    printf("K: ");
    poly_println(ml_kem->k);

    return 0;
}