/**
 * @file sparse_ops.h
 * @brief Sparse matrix operations
 * 
 * Operations for sparse matrices stored in CSR format,
 * including matrix-vector multiplication.
 */

#ifndef SPARSE_OPS_H
#define SPARSE_OPS_H

/**
 * @brief Sparse matrix-vector multiplication in CSR format
 * 
 * Computes y_local = A_local * x_global, where A_local is the local
 * portion of the sparse matrix stored in CSR format.
 * 
 * @param ptr Row pointer array (size: local_n + 1)
 * @param cols Column indices array (size: local_nnz)
 * @param vals Non-zero values array (size: local_nnz)
 * @param x_global Global input vector (size: global_n)
 * @param y_local Local output vector (size: local_n)
 * @param local_n Number of local rows
 * @param global_n Total number of rows
 */
void mat_vec_csr(int* ptr, int* cols, double* vals, 
                 double* x_global, double* y_local,
                 int local_n, int global_n);

#endif // SPARSE_OPS_H

