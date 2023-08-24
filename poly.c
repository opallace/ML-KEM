#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "aux.h"
#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

/* Inicializa um polinômio.
 *
 * Input:
 * Output: Um ponteiro para um tipo Poly.
 */
Poly* poly_init(){
	Poly* result = malloc(sizeof(Poly));
	result->size = 0;
	return result;
}

/* Printa um polinômio sem quebra de linha.
 *
 * input: Um polinômio.
 * output:
 */
void poly_print(Poly *a){
	printf("[");
	for (int i = 0; i < a->size; ++i){
		if(i == a->size - 1){
			printf("%i", a->coeff[i]);
		}else {
			printf("%i ", a->coeff[i]);
		}
	}
	printf("]");
}

/* Printa um polinômio com quebra de linha.
 *
 * input: Um polinômio.
 * output:
 */
void poly_println(Poly *a){
	printf("[");
	for (int i = 0; i < a->size; ++i){
		if(i == a->size - 1){
			printf("%i", a->coeff[i]);
		}else {
			printf("%i,", a->coeff[i]);
		}
	}
	printf("]\n\n");
}

/* Copia um polinômio para outra.
 *
 * Input: Dois polinômios.
 *        -a destino.
 *        -b origem.
 * Output:
 */
void poly_copy(Poly *a, Poly *b){

	a->coeff = malloc(b->size * sizeof(int));
	a->size  = b->size;

	memcpy(a->coeff, b->coeff, b->size * sizeof(int));
}

/* Libera memória alocada por um tipo Poly.
 *
 * Input: Um polinômio.
 * Output:
 */
void poly_free(Poly *a){
	free(a->coeff);
	free(a);
}

/* Aumenta o grau de um polinômio inserindo um valor
 * para os coeficientes novos.
 *
 * Input: -a: polinômio que será aumentado o grau.
 *        -new_size: novo grau do polinômio.
 *        -value: valor que será inserido nos novos coeficientes.
 * Output:
 */
void poly_fill(Poly *a, int new_size, int value){
	a->coeff = realloc(a->coeff, new_size * sizeof(int));
	
	for (int i = a->size; i < new_size; i++){
		a->coeff[i] = value;	
	}

	a->size = new_size;
}

/* Gera um polinômio em Z_q[x]/<x^n + 1>.
 *
 * Input: -n: grau do polinômio.
 *        -result: polinômio gerado.
 * Output:
 */
void poly_gen(int n, Poly *result){
	result->coeff = calloc(n, sizeof(int));
	result->size  = n;

	for (int i = 0; i < n-1; i++){
		result->coeff[i] = rand() % Q;
	}

	result->coeff[n-1] = (rand() % (Q-1)) + 1;
}

/* Gera um polinômio com coeficientes pertencentes a uma 
 * distribuição de probabilidade binomial centrada em 0. Cada
 * coeficiente do polinômio resultante estará no intervalo [-eta,eta]. 
 *
 * Input: -eta: parâmetro da distribuição binomial.
 *        -result: polinômio resultante.
 * Output:
 */
void poly_cbd(int eta, Poly *result){
	result->coeff = malloc(N * sizeof(int));
	result->size  = N;

	for (int i = 0; i < N; i++){
		int a = random() % (eta + 1);
		int b = random() % (eta + 1);

		result->coeff[i] =  a - b;
	}
}

/* Para cada coeficiente de um polinômio, altera para 1 se
 * este for mais próximo de q/2 do que de 0, e altera para 0
 * caso contrario.
 *
 * Input: Um polinômio a ser comprimido.
 * Output:
 */
void poly_compress(Poly *a, Poly *result){
	int half_q = (Q / 2.0) + 0.5;

    for (int i = 0; i < a->size; i++) {
		int dist_center = abs(half_q - a->coeff[i]);
		int dist_bound = a->coeff[i] < Q - a->coeff[i] ? a->coeff[i] : Q - a->coeff[i];
        result->coeff[i] = dist_center < dist_bound ? half_q : 0;
		result->coeff[i] = result->coeff[i] / half_q;
    }
}

/* Para cada coeficiente de um polinômio, altera para 0
 * se for 0, e [q/2] se for 1.
 *
 * Input: Um polinômio a ser descomprimido.
 * Output:
 */
void poly_decompress(Poly *a, Poly *result){
	for (size_t i = 0; i < a->size; i++){
		result->coeff[i] = (int)ceil((Q / 2.0) * a->coeff[i]);
	}
}

/* Realiza a soma de polinômios em Z_q[x]/<x^n + 1>.
 *
 * Input: Dois polinômios a serem somados.
 * Output:
 */
void poly_sum(Poly *aa, Poly *bb, Poly *result){
	Poly *temp_result = poly_init();
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	temp_result->size  = N;
	temp_result->coeff = calloc(N, sizeof(int));

	poly_fill(a, N, 0);
	poly_fill(b, N, 0);

	for (int i = 0; i < temp_result->size; i++){
		temp_result->coeff[i] = mod(a->coeff[i] + b->coeff[i], Q);
	}

	poly_copy(result, temp_result);

	poly_free(temp_result);
	poly_free(a);
	poly_free(b);
}

/* Realiza a subtração de polinômios em Z_q[x]/<x^n + 1>.
 *
 * Input: Dois polinômios a serem subtraidos.
 *        a(x) - b(x)
 * Output:
 */
void poly_sub(Poly *aa, Poly *bb, Poly *result){
	Poly *temp_result = poly_init();
	Poly *a = poly_init();
	Poly *b = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	temp_result->size  = N;
	temp_result->coeff = calloc(N, sizeof(int));

	poly_fill(a, N, 0);
	poly_fill(b, N, 0);

	for (int i = 0; i < temp_result->size; i++){
		temp_result->coeff[i] = mod(a->coeff[i] - b->coeff[i], Q);
	}

	poly_copy(result, temp_result);

	poly_free(temp_result);
	poly_free(a);
	poly_free(b);
}

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
void poly_mul(Poly *aa, Poly *bb, Poly *result){
	Poly *temp_result = poly_init();
	Poly *a           = poly_init();
	Poly *b           = poly_init();

	poly_copy(a, aa);
	poly_copy(b, bb);

	temp_result->size  = N;
	temp_result->coeff = calloc(N, sizeof(int));

	for (int i = 0; i < temp_result->size; i++){
		for (int j = 0; j < temp_result->size; j++){
			if(i-j >= 0){
				temp_result->coeff[i] = mod(temp_result->coeff[i] + mod(a->coeff[i-j] * b->coeff[j], Q), Q);
			}else {
				temp_result->coeff[i] = mod(temp_result->coeff[i] - mod(a->coeff[N+i-j] * b->coeff[j], Q), Q);
			}
		}
	}
	
	poly_copy(result, temp_result);

	poly_free(temp_result);
	poly_free(a);
	poly_free(b);
}