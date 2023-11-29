#include <stdlib.h>
#include <time.h>

#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "k_pke.h"

/* Inicializa o K_PKE.
 *
 * Input:
 * Output: Um ponteiro para o tipo K_PKE.
 */
K_PKE* k_pke_init(){
	K_PKE *k_pke = malloc(sizeof(K_PKE));

	k_pke->a  = polyvec_init();
	k_pke->s  = polyvec_init();
	k_pke->e  = polyvec_init();
	k_pke->t  = polyvec_init();
	k_pke->r  = polyvec_init();
	k_pke->e1 = polyvec_init();
	k_pke->u  = polyvec_init();

	k_pke->e2 = poly_init();
	k_pke->v  = poly_init();

	polyvec_poly_init(k_pke->a, K, K);
	polyvec_poly_init(k_pke->s, K, 1);
	polyvec_poly_init(k_pke->e, K, 1);
	polyvec_poly_init(k_pke->t, K, 1);
	polyvec_poly_init(k_pke->r, K, 1);
	polyvec_poly_init(k_pke->e1, K, 1);
	polyvec_poly_init(k_pke->u, K, 1);

	return k_pke;
}

/* Libera memória alocada pelo k_pke.
 *
 * Input: Um ponteiro para K_PKE.
 * Output: 
 */
void k_pke_free(K_PKE *k_pke){
	poly_free(k_pke->e2);
	poly_free(k_pke->v);

	polyvec_free(k_pke->a);
	polyvec_free(k_pke->s);
	polyvec_free(k_pke->e);
	polyvec_free(k_pke->t);
	polyvec_free(k_pke->r);
	polyvec_free(k_pke->e1);
	polyvec_free(k_pke->u);

	free(k_pke);
}

/* Gera as chaves públicas e privadas do k_pke.
 *
 * Input: Um ponteiro para K_PKE
 * Output: 
 */
void k_pke_keygen(K_PKE *k_pke){
	for (int i = 0; i < k_pke->a->size_i; i++){
		for (int j = 0; j < k_pke->a->size_j; j++){
			poly_gen(N, k_pke->a->poly[i][j]);
		}
	}

	for (int i = 0; i < k_pke->s->size_i; i++){
		for (int j = 0; j < k_pke->s->size_j; j++){
			poly_cbd(N1, k_pke->s->poly[i][j]);
		}
	}

	for (int i = 0; i < k_pke->e->size_i; i++){
		for (int j = 0; j < k_pke->e->size_j; j++){
			poly_cbd(N1, k_pke->e->poly[i][j]);
		}
	}

	polyvec_mul(k_pke->a, k_pke->s, k_pke->t);
	polyvec_sum(k_pke->t, k_pke->e, k_pke->t);
}

/* Cifra uma mensagem usando as chaves públicas
 * do k_pke.
 *
 * Input: Um ponteiro para K_PKE e uma mensagem.
 * Output: 
 */
void k_pke_encrypt(K_PKE *k_pke, Poly *msg){
	Polyvec *At  = polyvec_init();
	Polyvec *tt  = polyvec_init();
	Polyvec *mul = polyvec_init();

	polyvec_poly_init(At, K, K);
	polyvec_poly_init(tt, 1, K);
	polyvec_poly_init(mul, 1, 1);

	polyvec_transpose(k_pke->a, At);
	polyvec_transpose(k_pke->t, tt);

	for (int i = 0; i < k_pke->r->size_i; i++){
		for (int j = 0; j < k_pke->r->size_j; j++){
			poly_cbd(N1, k_pke->r->poly[i][j]);
		}
	}

	for (int i = 0; i < k_pke->e1->size_i; i++){
		for (int j = 0; j < k_pke->e1->size_j; j++){
			poly_cbd(N2, k_pke->e1->poly[i][j]);
		}
	}

	poly_cbd(N2, k_pke->e2);

	polyvec_mul(At, k_pke->r, k_pke->u);
	polyvec_sum(k_pke->u, k_pke->e1, k_pke->u);

	polyvec_mul(tt, k_pke->r, mul);
	poly_sum(mul->poly[0][0], k_pke->e2, k_pke->v);
	poly_decompress(msg, msg);

	poly_sum(k_pke->v, msg, k_pke->v);

	polyvec_free(At);
	polyvec_free(tt);
	polyvec_free(mul);
}

/* Decifra uma mensagem usando as chaves privadas
 * do k_pke.
 *
 * Input: Um ponteiro para K_PKE e uma mensagem cifrada.
 * Output: 
 */
void k_pke_decrypt(K_PKE *k_pke, Poly *msg){
	Polyvec *st  = polyvec_init();
	Polyvec *mul = polyvec_init();

	polyvec_poly_init(st, 1, K);
	polyvec_poly_init(mul, 1, 1);

	polyvec_transpose(k_pke->s, st);
	polyvec_mul(st, k_pke->u, mul);
	poly_copy(msg, mul->poly[0][0]);

	poly_sub(k_pke->v, msg, msg);
	poly_compress(msg, msg);

	polyvec_free(st);
	polyvec_free(mul);
}