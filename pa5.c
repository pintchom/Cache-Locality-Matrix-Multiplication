#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_VALUE 20
#define DIM 3
#define MIN_DIM_POWER 3
#define MAX_DIM_POWER 10    

void init(const int dim, int * const m) {
    for (int i = 0; i < dim * dim; i++) {
        m[i] = rand() % (MAX_VALUE + 1);
    }
}

void multiply(const int dim, const int * const a, int * const b, int * const c) { 
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
) {
    struct timeval start, end, elapsed;

    gettimeofday(&start, NULL);
    mult_func(dim, a, b, c);
    gettimeofday(&end, NULL);

    elapsed.tv_sec = end.tv_sec - start.tv_sec;
    elapsed.tv_usec = end.tv_usec - start.tv_usec;

    if (elapsed.tv_usec < 0) {
        elapsed.tv_usec += 1000000;
        elapsed.tv_sec -= 1;
    }

    return elapsed;
}

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
    int * c2 = (int*) calloc(dim * dim, sizeof(int));
    
    init(dim, a);
    init(dim, b);
    
    struct timeval time_multiply = run_and_time(multiply, dim, a, b, c);
    struct timeval time_transpose_multiply = run_and_time(transpose_and_multiply, dim, a, b, c2);

    int verif = verify(dim, c, c2);

    printf("Standard multiplication: %ld seconds, %d microseconds\n", (long)time_multiply.tv_sec, (int)time_multiply.tv_usec);
    printf("Multiplication with transpose: %ld seconds, %d microseconds\n", (long)time_transpose_multiply.tv_sec, (int)time_transpose_multiply.tv_usec);

    if (verif == 1) {
        printf("Results agree.\n");
    } else {
        printf("Results do not agree!\n");
    }

    free(a);
    free(b);
    free(c);
    free(c2);

    /*printf("OPERAND MATRIX A\n");
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
    free(temp_c);
    */
}

int main() {
    for (int power = MIN_DIM_POWER; power <= MAX_DIM_POWER; power++) {
        int dim = 1 << power;
        run_test(dim);
    }
    return EXIT_SUCCESS;
}
