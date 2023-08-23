#ifndef POLYVEC_H_INCLUDED
#define POLYVEC_H_INCLUDED

typedef struct{
	Poly ***poly;

	int size_i;
	int size_j;
} Polyvec;

/* Inicializa um Polyvec.
 *
 * Input: 
 * Output: Ponteiro para um tipo Polyvec.
 */
Polyvec* polyvec_init();

/* Inicializa os elementos de um Polyvec com base
 * na ordem da matriz.
 *
 * Input: -polyvec: ponteiro para um tipo Polyvec.
 *        -lines: números de linhas.
 *        -colums: número de colunas.
 * Output:
 */
void polyvec_poly_init(Polyvec *polyvec, int lines, int colums);

/* Copia uma matriz de polinômios para outra.
 *
 * Input: Duas matrizes de polinômios.
 *        -a destino.
*         -b origem.
 * Output:
 */
void polyvec_copy(Polyvec *a, Polyvec *b);

/* Libera memória alocada por um tipo Polyvec.
 *
 * Input: Uma matriz de polinômios.
 * Output:
 */
void polyvec_free(Polyvec *polyvec);

/* Realiza a operação de transposição matricial.
 *
 * Input: Uma matriz de polinômios.
 * Output: Uma matriz de polinômios.
 */
void polyvec_transpose(Polyvec *polyvec, Polyvec *transpose);

/* Realiza a operação de soma matricial.
 *
 * Input: Duas matrizes de polinômios.
 * Output: Uma matriz de polinômios.
 */
void polyvec_sum(Polyvec *a, Polyvec *b, Polyvec *result);

/* Realiza a operação de multiplicação matricial.
 *
 * Input: Duas matrizes de polinômios.
 * Output: Uma matriz de polinômios.
 */
void polyvec_mul(Polyvec *a, Polyvec *b, Polyvec *result);

#endif