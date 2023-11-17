#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"
#include "k_pke.h"

/* Inicializa um Polyvec.
 *
 * Input: 
 * Output: Ponteiro para um tipo Polyvec.
 */
Polyvec* polyvec_init(){
	return malloc(sizeof(Polyvec));
}

/* Inicializa os elementos de um Polyvec com base
 * na ordem da matriz.
 *
 * Input: -polyvec: ponteiro para um tipo Polyvec.
 *        -lines: números de linhas.
 *        -colums: número de colunas.
 * Output:
 */
void polyvec_poly_init(Polyvec *polyvec, int lines, int colums){
	polyvec->poly = malloc(sizeof(Poly**) * lines);

	for(int i = 0; i < lines; ++i){
		polyvec->poly[i] = malloc(sizeof(Poly*) * colums);

		for (int j = 0; j < colums; ++j){
			polyvec->poly[i][j] = poly_init();
		}
	}

	polyvec->size_i = lines;
	polyvec->size_j = colums;
}

/* Printa uma matriz de polinômios.
 *
 * input: uma matriz de polinômios.
 * output:
 * 
 */
void polyvec_println(Polyvec *polyvec){
	printf("[");
	for (int i = 0; i < polyvec->size_i; ++i){
		printf("[");
		for (int j = 0; j < polyvec->size_j; ++j){
			poly_print(polyvec->poly[i][j]);
			if(j < polyvec->size_j - 1){
				printf(",");
			}
		}
		printf("]");
		if(i < polyvec->size_i - 1){
			printf(",");
		}
	}
	printf("]\n\n");
}

/* Copia uma matriz de polinômios para outra.
 *
 * Input: Duas matrizes de polinômios.
 *        -a destino.
*         -b origem.
 * Output:
 */
void polyvec_copy(Polyvec *a, Polyvec *b){
	polyvec_poly_init(a, b->size_i, b->size_j);

	for(int i = 0; i < a->size_i; ++i){
		for(int j = 0; j < a->size_j; ++j){
			poly_copy(a->poly[i][j], b->poly[i][j]);
		}
	}
}

/* Libera memória alocada por um tipo Polyvec.
 *
 * Input: Uma matriz de polinômios.
 * Output:
 */
void polyvec_free(Polyvec *polyvec){
	for (int i = 0; i < polyvec->size_i; ++i){
		for (int j = 0; j < polyvec->size_j; ++j){
			poly_free(polyvec->poly[i][j]);
		}

		free(polyvec->poly[i]);
	}

	free(polyvec);
}

/* Realiza a operação de transposição matricial.
 *
 * Input: Uma matriz de polinômios.
 * Output: Uma matriz de polinômios.
 */
void polyvec_transpose(Polyvec *aa, Polyvec *result){
	Polyvec *a = polyvec_init();
	polyvec_copy(a, aa);

	for (int i = 0; i < a->size_i; i++){
		for (int j = 0; j < a->size_j; j++){
			poly_copy(result->poly[j][i], a->poly[i][j]);
		}
	}

	polyvec_free(a);
}

/* Realiza a operação de soma matricial.
 *
 * Input: Duas matrizes de polinômios.
 * Output: Uma matriz de polinômios.
 */
void polyvec_sum(Polyvec *aa, Polyvec *bb, Polyvec *result){
	Polyvec *a = polyvec_init();
	Polyvec *b = polyvec_init();

	polyvec_copy(a, aa);
	polyvec_copy(b, bb);

	Polyvec *temp_result = polyvec_init();
	polyvec_poly_init(temp_result, a->size_i, a->size_j);

	for(int i = 0; i < a->size_i; i++){
		for(int j = 0; j < a->size_j; j++){
			poly_sum(a->poly[i][j], b->poly[i][j], temp_result->poly[i][j]);
		}
	}

	polyvec_copy(result, temp_result);

	polyvec_free(temp_result);
	polyvec_free(a);
	polyvec_free(b);
}

/* Realiza a operação de multiplicação matricial.
 *
 * Input: Duas matrizes de polinômios.
 * Output: Uma matriz de polinômios.
 */
void polyvec_mul(Polyvec *aa, Polyvec *bb, Polyvec *result){
	Polyvec *a = polyvec_init();
	Polyvec *b = polyvec_init();

	polyvec_copy(a, aa);
	polyvec_copy(b, bb);

	Polyvec *temp_result = polyvec_init();
	polyvec_poly_init(temp_result, a->size_i, b->size_j);

	for (int u = 0; u < a->size_i; u++){
        for (int v = 0; v < b->size_j; v++){
			Poly *somatorio = poly_init();

            for (int k = 0; k < a->size_j; k++){
				Poly *mul  = poly_init();

				poly_mul(a->poly[u][k], b->poly[k][v], mul);
				poly_sum(somatorio, mul, somatorio);
			}

			poly_copy(temp_result->poly[u][v], somatorio);
			poly_free(somatorio);
        }
    }

	polyvec_copy(result, temp_result);
	
	polyvec_free(temp_result);
	polyvec_free(a);
	polyvec_free(b);
}