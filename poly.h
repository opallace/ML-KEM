#ifndef POLY_H_INCLUDED
#define POLY_H_INCLUDED

typedef struct{
	int *coeff;
	int size;
} Poly;

/* Inicializa um polinômio.
 *
 * Input:
 * Output: Um ponteiro para um tipo Poly.
 */
Poly* poly_init();

/* Copia um polinômio para outra.
 *
 * Input: Dois polinômios.
 *        -a destino.
 *        -b origem.
 * Output:
 */
void poly_copy(Poly *a, Poly *b);

/* Libera memória alocada por um tipo Poly.
 *
 * Input: Um polinômio.
 * Output:
 */
void poly_free(Poly *a);

/* Aumenta o grau de um polinômio inserindo um valor
 * para os coeficientes novos.
 *
 * Input: -a: polinômio que será aumentado o grau.
 *        -new_size: novo grau do polinômio.
 *        -value: valor que será inserido nos novos coeficientes.
 * Output:
 */
void poly_fill(Poly *a, int new_size, int value);

/* Gera um polinômio em Z_q[x]/<x^n + 1>.
 *
 * Input: -n: grau do polinômio.
 *        -result: polinômio gerado.
 * Output:
 */
void poly_gen(int n, Poly *result);

/* A partir de um polinômio com coeficientes aleatórios em q,
 * retorna um polinômio com coeficientes pertencentes a uma 
 * distribuição de probabilidade binomial centrada em 0. Cada
 * coeficiente do polinômio resultante estará no intervalo [-eta,eta]. 
 *
 * Input: -a: polinômio com coeficientes aleatórios em q.
 *        -eta: parâmetro da distribuição binomial.
 *        -result: polinômio resultante.
 * Output:
 */
void poly_cbd(Poly *a, int eta, Poly *result);

/* Para cada coeficiente de um polinômio, altera para 1 se
 * este for mais próximo de q/2 do que de 0, e altera para 0
 * caso contrario.
 *
 * Input: Um polinômio a ser comprimido.
 * Output:
 */
void poly_compress(Poly *a, Poly *result);

/* Para cada coeficiente de um polinômio, altera para 0
 * se for 0, e [q/2] se for 1.
 *
 * Input: Um polinômio a ser descomprimido.
 * Output:
 */
void poly_decompress(Poly *a, Poly *result);

/* Realiza a soma de polinômios em Z_q[x]/<x^n + 1>.
 *
 * Input: Dois polinômios a serem somados.
 * Output:
 */
void poly_sum(Poly *a, Poly *b, Poly *result);

/* Realiza a subtração de polinômios em Z_q[x]/<x^n + 1>.
 *
 * Input: Dois polinômios a serem subtraidos.
 *        a(x) - b(x)
 * Output:
 */
void poly_sub(Poly *a, Poly *b, Poly *result);

/* Realiza a multiplicação de polinômios em Z_q[x]/<x^n + 1>.
 * Esta função se aproveita da seguinte propriedade da multiplicação
 * de polinômios em Z_q[x]/<x^n + 1>:
 *
 *	(a + bx + cx² + dx³)(x_0 + x_1x + x_2x² + x_3x³)(mod x⁴ + 1) =
 *	a -d -c -b   x_0
 *	b  a -d -c   x_1
 *	c  b  a -d * x_2
 *	d  c  b  a   x_3
 *
 * Input: Dois polinômios a serem multiplicados.
 * Output:
 */
void poly_mul(Poly *a, Poly *b, Poly *result);

#endif