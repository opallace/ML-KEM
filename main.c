#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"

int main(){

	Polyvec *pv = polyvec_init(2, 2);

	poly_gen_tern(256, 5, 5, pv->poly[0][0]);
	poly_gen_tern(256, 5, 5, pv->poly[0][1]);
	poly_gen_tern(256, 5, 5, pv->poly[1][0]);
	poly_gen_tern(256, 5, 5, pv->poly[1][1]);

	polyvec_print(pv);

	return 0;
}