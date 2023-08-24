#include <stdlib.h>

/* Retorna o maior entre dois elementos.
 *
 * input: dois inteiros.
 * output: um inteiro.
 */
int max(int a, int b){
	if(a >= b){
		return a;
	
	}else {
		return b;
	}
}

/* Realiza a operação de módulo em um inteiro.
 *
 * input: inteiro a ser realizado a operação e o módulo.
 * output: resultado da operação n (mod m).
 */
int mod(int n, int m){
	int remainder = n % m;
	
	if(remainder < 0){
		return remainder + m;
	
	}else {
		return remainder;
	} 
}