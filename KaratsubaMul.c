#include <stdio.h>
#include <malloc.h>
#include "KaratsubaMul.h"

const int MIN_NUM_TO_KARATSUBA_MUL = 64;

void karatsuba_multiply(int *coef_ans, int *coef_a, int *coef_b, int n) {
    int m = n / 2;
    int *coef_a1b1 = NULL;
    int *coef_a2b2 = NULL;
    int *coef_a1pa2 = NULL; //auxiliary variable for (a1 + a2) * (b1 + b2)
    int *coef_b1pb2 = NULL; //auxiliary variable for (a1 + a2) * (b1 + b2)
    int *coef_a1pa2_b1pb2 = NULL;

    if (n <= MIN_NUM_TO_KARATSUBA_MUL) {
        multiply_polynomials(coef_ans, coef_a, coef_b, n);

        return;
    }

    coef_a1b1 = (int *) calloc(n, sizeof(int));
    coef_a2b2 = (int *) calloc(n, sizeof(int));

    karatsuba_multiply(coef_a1b1, coef_a, coef_b, m);
    karatsuba_multiply(coef_a2b2, &coef_a[m], &coef_b[m], m);

    for (int i = 0; i < n - 1; i++) {
        coef_ans[i] += coef_a1b1[i];
        coef_ans[i + m] -= coef_a2b2[i];
        coef_ans[i + m] -= coef_a1b1[i];
        coef_ans[i + n] += coef_a2b2[i];
    }

    free(coef_a1b1);
    free(coef_a2b2);

    coef_a1pa2 = (int *) calloc(m, sizeof(int));
    coef_b1pb2 = (int *) calloc(m, sizeof(int));
    coef_a1pa2_b1pb2 = (int *) calloc(n, sizeof(int));

    for (int i = 0; i < m; i++) {
        coef_a1pa2[i] = coef_a[i] + coef_a[i + m];
        coef_b1pb2[i] = coef_b[i] + coef_b[i + m];
    }

    karatsuba_multiply(coef_a1pa2_b1pb2, coef_a1pa2, coef_b1pb2, m);

    for (int i = 0; i < n - 1; i++) 
        coef_ans[i + m] += coef_a1pa2_b1pb2[i];
    
    free(coef_a1pa2);
    free(coef_b1pb2);
    free(coef_a1pa2_b1pb2);
}

void multiply_polynomials(int *coef_ans, int *coef_a, int *coef_b, int n) {
    for (int i = n - 1; i >= 0; i--)
            for (int j = n - 1; j >= 0; j--)
                coef_ans[i + j] += coef_a[i] * coef_b[j];
    return;
}
