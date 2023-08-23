#ifndef KYBER_H_INCLUDED
#define KYBER_H_INCLUDED

#define N 256
#define K 4
#define Q 3329
#define N1 2
#define N2 2

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
} Kyber;

/* Inicializa o Kyber.
 *
 * Input:
 * Output: Um ponteiro para o tipo Kyber.
 */
Kyber* kyber_init();

/* Libera memória alocada pelo kyber.
 *
 * Input: Um ponteiro para Kyber.
 * Output: 
 */
void kyber_free(Kyber *kyber);

/* Gera as chaves públicas e privadas do kyber.
 *
 * Input: Um ponteiro para Kyber
 * Output: 
 */
void kyber_keygen(Kyber *kyber);

/* Cifra uma mensagem usando as chaves públicas
 * do kyber.
 *
 * Input: Um ponteiro para Kyber e uma mensagem.
 * Output: 
 */
void kyber_encrypt(Kyber *kyber, Poly *msg);

/* Decifra uma mensagem usando as chaves privadas
 * do kyber.
 *
 * Input: Um ponteiro para Kyber e uma mensagem cifrada.
 * Output: 
 */
void kyber_decrypt(Kyber *kyber, Poly *msg);

#endif