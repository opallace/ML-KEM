#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

Kyber* kyber_init(){
	Kyber *kyber = malloc(sizeof(Kyber));

	kyber->a = polyvec_init();
	kyber->s = polyvec_init();
	kyber->e = polyvec_init();
	kyber->t = polyvec_init();

	kyber->r  = polyvec_init();
	kyber->e1 = polyvec_init();
	kyber->e2 = poly_init();
	kyber->u  = polyvec_init();
	kyber->v  = poly_init();

	return kyber;
}

void kyber_params_set(int n, int k, int q, int n1, int n2, Kyber *kyber){
	kyber->n = n;
	kyber->k = k;
	kyber->q = q;
	kyber->n1 = n1;
	kyber->n2 = n2;
}

void kyber_encrypt(Kyber *kyber, Poly *msg, Poly *result){
	// Poly *enc = poly_init();

	// poly_mul(ntru->b, ntru->h, ntru->q, enc);
	// poly_sum(enc, msg, ntru->q, enc);
	// poly_mod(enc, ntru->r, ntru->q, enc);

	// poly_copy(result, enc);
	// poly_free(enc);
}

void kyber_decrypt(Kyber *kyber, Poly *msg, Poly *result){
	// Poly *dec = poly_init();

	// poly_mul(ntru->f, msg, ntru->q, dec);
	// poly_mod(dec, ntru->r, ntru->q, dec);
	// poly_mod_center(dec, ntru->q, dec);

	// poly_mul(ntru->f_p, dec, ntru->p, dec);
	// poly_mod(dec, ntru->r, ntru->p, dec);
	// poly_mod_center(dec, ntru->p, dec);

	// poly_copy(result, dec);
	// poly_free(dec);
}


