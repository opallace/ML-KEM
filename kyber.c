#include <stdlib.h>
#include <time.h>

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

void kyber_keygen(Kyber *kyber){
	srand(time(NULL));

	for (int i = 0; i < kyber->a->size_i; i++){
		for (int j = 0; j < kyber->a->size_j; j++){
			poly_gen(N, kyber->a->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->s->size_i; i++){
		for (int j = 0; j < kyber->s->size_j; j++){
			poly_gen(N, kyber->s->poly[i][j]);
			poly_cbd(kyber->s->poly[i][j], N1, kyber->s->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->e->size_i; i++){
		for (int j = 0; j < kyber->e->size_j; j++){
			poly_gen(N, kyber->e->poly[i][j]);
			poly_cbd(kyber->e->poly[i][j], N1, kyber->e->poly[i][j]);
		}
	}

	polyvec_mul(kyber->a, kyber->s, kyber->t);
	polyvec_sum(kyber->t, kyber->e, kyber->t);
}

void kyber_encrypt(Kyber *kyber, Poly *msg){
	srand(time(NULL));

	Polyvec *At  = polyvec_init(K, K);
	Polyvec *tt  = polyvec_init(1, K);
	Polyvec *mul = polyvec_init(1, 1);

	polyvec_transpose(kyber->a, At);
	polyvec_transpose(kyber->t, tt);

	for (int i = 0; i < kyber->r->size_i; i++){
		for (int j = 0; j < kyber->r->size_j; j++){
			poly_gen(N, kyber->r->poly[i][j]);
			poly_cbd(kyber->r->poly[i][j], N1, kyber->r->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->e1->size_i; i++){
		for (int j = 0; j < kyber->e1->size_j; j++){
			poly_gen(N, kyber->e1->poly[i][j]);
			poly_cbd(kyber->e1->poly[i][j], N2, kyber->e1->poly[i][j]);
		}
	}

	poly_gen(N, kyber->e2);
	poly_cbd(kyber->e2, N2, kyber->e2);

	polyvec_mul(At, kyber->r, kyber->u);
	polyvec_sum(kyber->u, kyber->e1, kyber->u);

	polyvec_mul(tt, kyber->r, mul);
	poly_sum(mul->poly[0][0], kyber->e2, kyber->v);
	poly_decompress(msg, msg);

	poly_sum(kyber->v, msg, kyber->v);
}

void kyber_decrypt(Kyber *kyber, Poly *msg){
	Polyvec *st  = polyvec_init(1, K);
	Polyvec *mul = polyvec_init(1, 1);

	polyvec_transpose(kyber->s, st);
	polyvec_mul(st, kyber->u, mul);
	poly_copy(msg, mul->poly[0][0]);

	poly_sub(kyber->v, msg, msg);
	poly_compress(msg, msg);
}