#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 20
#define DIM 3
    
void init(const int dim, int * const m) {
    for (int i = 0; i < dim * dim; i++) {
        m[i] = rand() % (MAX_VALUE + 1);
    }
}

void multiply(const int dim, const int * const a, int * const b, int * const c) {
    
    /*
     * MATRIX MULTIPLICATION
     * cij = SUM k = 1->N of aik + bkj
     *
        7,2,13,9,   X  7,20,20,8,
        8,6,0,0,    X  1,2,20,5,     
        16,4,18,15, X  4,10,19,14,
        1,6,14,4,   X  3,9,16,17,
     *
     * =
     *  c[i,j] = a[i,0]b[0,j] = a[i,1]b[1,j] ...
     *  c[0,0] = a[0,0]b[0,0] + a[0,1]b[1,0] + a[0,2]b[2,0] = a[0,3]b[3,0]
     *  c[0,0] = (7)(7) + 2(1) + 13(4) + 9(3) = ...
     *
     */

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            c[dim * i + j] = 0;
            for (int x = 0; x < dim; x++) {
                c[dim * i + j] += a[dim*i + x] * b[dim * x + j];
            }
        }
    }
}

void transpose(const int dim, int * const m) {

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < i; j++) {
            int temp = m[i*dim + j];
            m[i*dim + j] = m[j*dim + i];
            m[j*dim + i] = temp;   
        }
    }
}

void multiply_transpose(const int dim, const int * const a, const int * const b_t, int * const c) {

     /*
     * MATRIX MULTIPLICATION BY ROWS 
     * cij = SUM k = 1->N of aik + bjk
     *
        7,2,13,9,   X  7,20,20,8,
        8,6,0,0,    X  1,2,20,5,     
        16,4,18,15, X  4,10,19,14,
        1,6,14,4,   X  3,9,16,17,
     *
     * =
     *  c[i,j] = a[i,0]b[j,0] = a[i,1]b[j,1] ...
     *  c[0,0] = a[0,0]b[0,0] + a[0,1]b[0,1] + a[0,2]b[0,2] = a[0,3]b[0,3]
     *
     */

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            c[dim * i + j] = 0;
            for (int x = 0; x < dim; x++) {
                c[dim * i + j] += a[dim * i + x] * b_t[dim * j + x];
            }
        }
    } 
}

void transpose_and_multiply(const int dim, const int * const a, int * const b, int * const c) {
    transpose(dim, b);
    multiply_transpose(dim, a, b, c);   
}

struct timeval run_and_time(
    void (* mult_func)(const int, const int * const, int * const, int * const), 
    const int dim,
    const int * const a,
    int * const b,
    int * const c
);

int verify(const int dim, const int * const c1, const int * const c2) {
    int TRUE = 1;
    int FALSE = 0;
    
    for (int i = 0; i < dim * dim; i++) {
        if (c1[i] != c2[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

void print(const int dim, const int * const m) {

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d,", m[dim * i + j]);
        }
        printf("\n");
    }
}

void run_test(const int dim) {
    int * a = (int*) calloc(dim * dim, sizeof(int));
    int * b = (int*) calloc(dim * dim, sizeof(int));
    int * c = (int*) calloc(dim * dim, sizeof(int));
    
    init(dim, a);
    init(dim, b);

    printf("OPERAND MATRIX A\n"); 
    print(dim, a);
    printf("OPERAND MATRIX B\n"); 
    print(dim, b);

    printf("ORIGINAL MULTIPLICATION MATRIX \n");
    multiply(dim, a, b, c);
    print(dim, c);
    int * temp_c = c;

    printf("TRANSPOSED MULTIPLICATION MATRIX \n");
    transpose_and_multiply(dim, a, b, c);
    print(dim, c);
    int verif = verify(dim, temp_c, c);
    if (verif == 1) {
        printf("MATRICES MATCH!");
    } else {
        printf("MATRICES DO NOT MATCH!");
    }

    free(a);
    free(b);
    free(c);
}

int main() {
    run_test(DIM);
    return EXIT_SUCCESS;    
}
