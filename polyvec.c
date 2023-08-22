#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

Polyvec* polyvec_init(int lines, int colums){
	Polyvec* result = malloc(sizeof(Polyvec));

	result->poly = malloc(sizeof(Poly**) * lines);

	for (int i = 0; i < lines; ++i){
		result->poly[i] = malloc(sizeof(Poly*) * colums);

		for (int j = 0; j < colums; ++j){
			result->poly[i][j] = poly_init();
		}
	}

	result->size_i = lines;
	result->size_j = colums;

	return result;
}

void polyvec_print(Polyvec *polyvec){
	for (int i = 0; i < polyvec->size_i; ++i){
		for (int j = 0; j < polyvec->size_j; ++j){
			poly_print(polyvec->poly[i][j]);
			if(j < polyvec->size_j - 1){
				printf(" | ");
			}
		}
		printf("\n");
	}
}

void polyvec_free(Polyvec *polyvec){
	for (int i = 0; i < polyvec->size_i; ++i){
		for (int j = 0; j < polyvec->size_j; ++j){
			 poly_free(polyvec->poly[i][j]);
		}

		free(polyvec->poly[i]);
	}

	free(polyvec);
}

void polyvec_transpose(Polyvec *polyvec, Polyvec *transpose){
	for (int i = 0; i < polyvec->size_i; i++){
		for (int j = 0; j < polyvec->size_j; j++){
			poly_copy(transpose->poly[j][i], polyvec->poly[i][j]);
		}
	}
}

void polyvec_sum(Polyvec *a, Polyvec *b, Polyvec *result){
	for(int i = 0; i < a->size_i; i++){
		for(int j = 0; j < a->size_j; j++){
			poly_sum(a->poly[i][j], b->poly[i][j], result->poly[i][j]);
		}
	}
}


void polyvec_mul(Polyvec *a, Polyvec *b, Polyvec *result){
	for (int u = 0; u < a->size_i; u++){
        for (int v = 0; v < b->size_j; v++){
            for (int k = 0; k < a->size_j; k++){
				Poly *mul  = poly_init();

				poly_mul(a->poly[u][k], b->poly[k][v], mul);
				poly_sum(result->poly[u][v], mul, result->poly[u][v]);

				poly_free(mul);
			}
        }
    }
}