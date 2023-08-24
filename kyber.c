#include <stdlib.h>
#include <time.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

/* Inicializa o Kyber.
 *
 * Input:
 * Output: Um ponteiro para o tipo Kyber.
 */
Kyber* kyber_init(){
	Kyber *kyber = malloc(sizeof(Kyber));

	kyber->a  = polyvec_init();
	kyber->s  = polyvec_init();
	kyber->e  = polyvec_init();
	kyber->t  = polyvec_init();
	kyber->r  = polyvec_init();
	kyber->e1 = polyvec_init();
	kyber->u  = polyvec_init();

	kyber->e2 = poly_init();
	kyber->v  = poly_init();

	polyvec_poly_init(kyber->a, K, K);
	polyvec_poly_init(kyber->s, K, 1);
	polyvec_poly_init(kyber->e, K, 1);
	polyvec_poly_init(kyber->t, K, 1);
	polyvec_poly_init(kyber->r, K, 1);
	polyvec_poly_init(kyber->e1, K, 1);
	polyvec_poly_init(kyber->u, K, 1);

	return kyber;
}

/* Libera memória alocada pelo kyber.
 *
 * Input: Um ponteiro para Kyber.
 * Output: 
 */
void kyber_free(Kyber *kyber){
	poly_free(kyber->e2);
	poly_free(kyber->v);

	polyvec_free(kyber->a);
	polyvec_free(kyber->s);
	polyvec_free(kyber->e);
	polyvec_free(kyber->t);
	polyvec_free(kyber->r);
	polyvec_free(kyber->e1);
	polyvec_free(kyber->u);

	free(kyber);
}

/* Gera as chaves públicas e privadas do kyber.
 *
 * Input: Um ponteiro para Kyber
 * Output: 
 */
void kyber_keygen(Kyber *kyber){
	for (int i = 0; i < kyber->a->size_i; i++){
		for (int j = 0; j < kyber->a->size_j; j++){
			poly_gen(N, kyber->a->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->s->size_i; i++){
		for (int j = 0; j < kyber->s->size_j; j++){
			poly_cbd(N1, kyber->s->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->e->size_i; i++){
		for (int j = 0; j < kyber->e->size_j; j++){
			poly_cbd(N1, kyber->e->poly[i][j]);
		}
	}

	polyvec_mul(kyber->a, kyber->s, kyber->t);
	polyvec_sum(kyber->t, kyber->e, kyber->t);
}

/* Cifra uma mensagem usando as chaves públicas
 * do kyber.
 *
 * Input: Um ponteiro para Kyber e uma mensagem.
 * Output: 
 */
void kyber_encrypt(Kyber *kyber, Poly *msg){
	Polyvec *At  = polyvec_init();
	Polyvec *tt  = polyvec_init();
	Polyvec *mul = polyvec_init();

	polyvec_poly_init(At, K, K);
	polyvec_poly_init(tt, 1, K);
	polyvec_poly_init(mul, 1, 1);

	polyvec_transpose(kyber->a, At);
	polyvec_transpose(kyber->t, tt);

	for (int i = 0; i < kyber->r->size_i; i++){
		for (int j = 0; j < kyber->r->size_j; j++){
			poly_cbd(N1, kyber->r->poly[i][j]);
		}
	}

	for (int i = 0; i < kyber->e1->size_i; i++){
		for (int j = 0; j < kyber->e1->size_j; j++){
			poly_cbd(N2, kyber->e1->poly[i][j]);
		}
	}

	poly_cbd(N2, kyber->e2);

	polyvec_mul(At, kyber->r, kyber->u);
	polyvec_sum(kyber->u, kyber->e1, kyber->u);

	polyvec_mul(tt, kyber->r, mul);
	poly_sum(mul->poly[0][0], kyber->e2, kyber->v);
	poly_decompress(msg, msg);

	poly_sum(kyber->v, msg, kyber->v);

	polyvec_free(At);
	polyvec_free(tt);
	polyvec_free(mul);
}

/* Decifra uma mensagem usando as chaves privadas
 * do kyber.
 *
 * Input: Um ponteiro para Kyber e uma mensagem cifrada.
 * Output: 
 */
void kyber_decrypt(Kyber *kyber, Poly *msg){
	Polyvec *st  = polyvec_init();
	Polyvec *mul = polyvec_init();

	polyvec_poly_init(st, 1, K);
	polyvec_poly_init(mul, 1, 1);

	polyvec_transpose(kyber->s, st);
	polyvec_mul(st, kyber->u, mul);
	poly_copy(msg, mul->poly[0][0]);

	poly_sub(kyber->v, msg, msg);
	poly_compress(msg, msg);

	polyvec_free(st);
	polyvec_free(mul);
}