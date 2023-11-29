#ifndef K_PKE_H_INCLUDED
#define K_PKE_H_INCLUDED

typedef struct{
	Polyvec *a;
	Polyvec *s;
	Polyvec *e;
	Polyvec *t;
	Polyvec *r;
	Polyvec *e1;
	Polyvec *u;

	Poly *e2;
	Poly *v;
} K_PKE;

/* Inicializa o K_PKE.
 *
 * Input:
 * Output: Um ponteiro para o tipo K_PKE.
 */
K_PKE* k_pke_init();

/* Libera memória alocada pelo k_pke.
 *
 * Input: Um ponteiro para K_PKE.
 * Output: 
 */
void k_pke_free(K_PKE *k_pke);

/* Gera as chaves públicas e privadas do k_pke.
 *
 * Input: Um ponteiro para K_PKE
 * Output: 
 */
void k_pke_keygen(K_PKE *k_pke);

/* Cifra uma mensagem usando as chaves públicas
 * do k_pke.
 *
 * Input: Um ponteiro para K_PKE e uma mensagem.
 * Output: 
 */
void k_pke_encrypt(K_PKE *k_pke, Poly *msg);

/* Decifra uma mensagem usando as chaves privadas
 * do k_pke.
 *
 * Input: Um ponteiro para K_PKE e uma mensagem cifrada.
 * Output: 
 */
void k_pke_decrypt(K_PKE *k_pke, Poly *msg);

#endif