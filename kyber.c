#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

Kyber* kyber_init(){
	Kyber *kyber = malloc(sizeof(Kyber));

	kyber->a = polyvec_init(K, K);
	kyber->s = polyvec_init(K, 1);
	kyber->e = polyvec_init(K, 1);
	kyber->t = polyvec_init(K, 1);

	kyber->r  = polyvec_init(K, 1);
	kyber->e1 = polyvec_init(K, 1);
	kyber->e2 = poly_init();
	kyber->u  = polyvec_init(K, 1);
	kyber->v  = poly_init();

	return kyber;
}

void kyber_gen_ring(int p, Poly *result){
	Poly *ring = poly_init();

	ring->coeff = calloc((p + 1), sizeof(int));
	ring->size = p + 1;

	ring->coeff[0] = 1;
	ring->coeff[p] = 1;

	poly_copy(result, ring);
	poly_free(ring);
}

void kyber_keygen(Kyber *kyber){
	for (int i = 0; i < kyber->a->size_i; i++){
		for (int j = 0; j < kyber->a->size_j; j++){
			poly_gen(N, kyber->a->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->s->size_i; i++){
		for (int j = 0; j < kyber->s->size_j; j++){
			poly_gen_small(N, kyber->s->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->e->size_i; i++){
		for (int j = 0; j < kyber->e->size_j; j++){
			poly_gen_small(N, kyber->e->poly[i][j]);
		}
	}

	polyvec_mul(kyber->a, kyber->s, kyber->t);
	polyvec_sum(kyber->t, kyber->e, kyber->t);
}

void kyber_encrypt(Kyber *kyber, Poly *msg){
	Polyvec *At  = polyvec_init(K, K);
	Polyvec *tt  = polyvec_init(1, K);
	Polyvec *mul = polyvec_init(1, 1);

	polyvec_transpose(kyber->a, At);

	for (int i = 0; i < kyber->r->size_i; i++){
		for (int j = 0; j < kyber->r->size_j; j++){
			poly_gen_small(N, kyber->r->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->e1->size_i; i++){
		for (int j = 0; j < kyber->e1->size_j; j++){
			poly_gen_small(N, kyber->e1->poly[i][j]);
		}
	}

	poly_gen_small(N, kyber->e2);

	polyvec_mul(At, kyber->r, kyber->u);
	polyvec_sum(kyber->u, kyber->e1, kyber->u);

	polyvec_transpose(kyber->t, tt);
	polyvec_mul(tt, kyber->r, mul);
	poly_copy(kyber->v, mul->poly[0][0]);
	poly_sum(kyber->v, kyber->e2, Q, kyber->v);
	poly_int_mul(msg, Q/2, Q, msg);
	poly_sum(kyber->v, msg, Q, kyber->v);
}

void kyber_decrypt(Kyber *kyber, Poly *msg){
	Polyvec *st  = polyvec_init(1, K);
	Polyvec *mul = polyvec_init(1, 1);

	polyvec_transpose(kyber->s, st);
	polyvec_mul(st, kyber->u, mul);
	poly_copy(msg, mul->poly[0][0]);

	poly_sub(kyber->v, msg, Q, msg);
	
	//problema aqui
	poly_int_mul(msg, 2/Q, Q, msg);
	poly_trunc(msg, 2);
}


