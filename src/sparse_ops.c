/**
 * @file sparse_ops.c
 * @brief Implementation of sparse matrix operations
 */

#include "sparse_ops.h"

void mat_vec_csr(int* ptr, int* cols, double* vals, 
                 double* x_global, double* y_local,
                 int local_n, int global_n) {
    for (int i = 0; i < local_n; i++) {
        y_local[i] = 0.0;
        for (int j = ptr[i]; j < ptr[i + 1]; j++) {
            y_local[i] += vals[j] * x_global[cols[j]];
        }
    }
}

