#ifndef ML_KEM_H_INCLUDED
#define ML_KEM_H_INCLUDED

typedef struct{
    Polyvec *ek_pke_t;
    Polyvec *ek_pke_a;

    Polyvec *dk_pke_s;

    Polyvec *c_u;
    Poly *c_v;

    Poly *k;
} ML_KEM;

void H(char *in, Poly *out);

ML_KEM* ml_kem_init();
void ml_kem_free(ML_KEM *ml_kem);
void ml_kem_keygen(ML_KEM *ml_kem);
void ml_kem_encapsulate(ML_KEM *ml_kem);
void ml_kem_decapsulate(ML_KEM *ml_kem, Poly *k);

#endif