#include <stdlib.h>
#include <time.h>

#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "k_pke.h"
#include "ml_kem.h"

ML_KEM* ml_kem_init(){
    ML_KEM *ml_kem = malloc(sizeof(ML_KEM));

	ml_kem->ek_pke_t  = polyvec_init();
	ml_kem->ek_pke_a  = polyvec_init();
	ml_kem->dk_pke_s  = polyvec_init();
	ml_kem->c_u       = polyvec_init();
	ml_kem->c_v = poly_init();
	ml_kem->k   = poly_init();

	polyvec_poly_init(ml_kem->ek_pke_a, K, K);
	polyvec_poly_init(ml_kem->dk_pke_s, K, 1);
	polyvec_poly_init(ml_kem->ek_pke_t, K, 1);
	polyvec_poly_init(ml_kem->c_u, K, 1);

    return ml_kem;
}

void ml_kem_free(ML_KEM *ml_kem){
    polyvec_free(ml_kem->ek_pke_t);
	polyvec_free(ml_kem->ek_pke_a);
	polyvec_free(ml_kem->dk_pke_s);
	polyvec_free(ml_kem->c_u);
	poly_free(ml_kem->c_v);
	poly_free(ml_kem->k);

	free(ml_kem);
}

void ml_kem_keygen(ML_KEM *ml_kem){

	K_PKE *k_pke = k_pke_init();

	k_pke_keygen(k_pke);

	polyvec_copy(ml_kem->ek_pke_a, k_pke->a);
	polyvec_copy(ml_kem->ek_pke_t, k_pke->t);
	polyvec_copy(ml_kem->dk_pke_s, k_pke->s);

	k_pke_free(k_pke);

}

void ml_kem_encapsulate(ML_KEM *ml_kem){

	K_PKE *k_pke = k_pke_init();

	ml_kem->k->coeff = malloc(sizeof(int) * N);
	ml_kem->k->size  = N;

	for (int i = 0; i < ml_kem->k->size; i++){
		ml_kem->k->coeff[i] = rand() % 2;
	}

	polyvec_copy(k_pke->a, ml_kem->ek_pke_a);
	polyvec_copy(k_pke->t, ml_kem->ek_pke_t);

	k_pke_encrypt(k_pke, ml_kem->k);

	polyvec_copy(ml_kem->c_u, k_pke->u);
	poly_copy(ml_kem->c_v, k_pke->v);

	k_pke_free(k_pke);
}

void ml_kem_decapsulate(ML_KEM *ml_kem, Poly *k){
	K_PKE *k_pke = k_pke_init();

	polyvec_copy(k_pke->s, ml_kem->dk_pke_s);
	polyvec_copy(k_pke->u, ml_kem->c_u);
	poly_copy(k_pke->v, ml_kem->c_v);

	Poly *k_temp = poly_init();
	k_pke_decrypt(k_pke, k_temp);

	poly_copy(k, k_temp);

	poly_free(k_temp);
	k_pke_free(k_pke);
}