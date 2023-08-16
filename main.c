#include <stdio.h>
#include <stdlib.h>

#include "poly.h"
#include "polyvec.h"
#include "kyber.h"

int main(){

	Kyber *kyber =  kyber_init();
	Poly *msg = poly_init();
	Poly *dec = poly_init();

	kyber_keygen(kyber);
	poly_gen(N, msg);
	

	kyber_encrypt(kyber, msg);
	kyber_decrypt(kyber, dec);

	return 0;
}