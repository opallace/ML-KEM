#ifndef AUX_H_INCLUDED
#define AUX_H_INCLUDED

int* bytes_to_bits(int *bytes, int size);
int max(int a, int b);
int mod(int n, int modulus);
int ntruprime_inv_int(int n, int modulus);
int is_prime(int n);
int is_2_power(int n);

#endif