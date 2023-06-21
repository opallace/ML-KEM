#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"

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
			printf(" | ");
		}
		printf("\n");
	}
}