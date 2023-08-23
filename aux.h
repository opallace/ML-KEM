#ifndef AUX_H_INCLUDED
#define AUX_H_INCLUDED

/* Converte um vetor de bytes em um vetor de bits.
 *
 * input: vetor de bytes e seu tamanho.
 * output: um vetor de bits.
 */
int* bytes_to_bits(int *bytes, int size);

/* Retorna o maior entre dois elementos.
 *
 * input: dois inteiros.
 * output: um inteiro.
 */
int max(int a, int b);

/* Realiza a operação de módulo em um inteiro.
 *
 * input: inteiro a ser realizado a operação e o módulo.
 * output: resultado da operação n (mod m).
 */
int mod(int n, int m);

#endif