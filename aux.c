#include <stdlib.h>

int* bytes_to_bits(int *bytes, int size){
	int *bits = malloc(sizeof(int) * size * 8);

	for (int i = 0; i < size; i++){
		bits[(i*8)+0] = (bytes[i] & 0x80) >> 7;
		bits[(i*8)+1] = (bytes[i] & 0x40) >> 6;
		bits[(i*8)+2] = (bytes[i] & 0x20) >> 5;
		bits[(i*8)+3] = (bytes[i] & 0x10) >> 4;
		bits[(i*8)+4] = (bytes[i] & 0x08) >> 3;
		bits[(i*8)+5] = (bytes[i] & 0x04) >> 2;
		bits[(i*8)+6] = (bytes[i] & 0x02) >> 1;
		bits[(i*8)+7] = (bytes[i] & 0x01) >> 0;
	}

	return bits;
}


int max(int a, int b){
	if(a >= b){
		return a;
	
	}else {
		return b;
	}
}

int mod(int n, int modulus){
	int remainder = n % modulus;
	
	if(remainder < 0){
		return remainder + modulus;
	
	}else {
		return remainder;
	} 
}

int ntruprime_inv_int(int n, int modulus){
    int new_t = 0;
    int t = 1;
    int b = modulus;

    while (b != 0) {
        int quotient = n / b;

        int temp = n;
        n = b;
        b = temp % b;

        temp = new_t;
        new_t = t - quotient*new_t;
        t = temp;
    }

    if (t < 0){
        t += modulus;
    }

    return t;
}
