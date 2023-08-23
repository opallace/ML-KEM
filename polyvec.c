#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

Polyvec* polyvec_init(){
	return malloc(sizeof(Polyvec));
}

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

void polyvec_copy(Polyvec *a, Polyvec *b){
	polyvec_poly_init(a, b->size_i, b->size_j);

	for(int i = 0; i < a->size_i; ++i){
		for(int j = 0; j < a->size_j; ++j){
			poly_copy(a->poly[i][j], b->poly[i][j]);
		}
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