/**
 * @file csr_io.h
 * @brief CSR (Compressed Sparse Row) matrix I/O operations
 * 
 * Functions for parallel reading and writing of sparse matrices
 * in CSR format using MPI-IO.
 */

#ifndef CSR_IO_H
#define CSR_IO_H

#include <mpi.h>

/**
 * @brief Read a sparse matrix in CSR format using parallel MPI-IO
 * 
 * @param filename Path to the CSR matrix file
 * @param ptr Output: row pointer array (size: local_n + 1)
 * @param cols Output: column indices array (size: local_nnz)
 * @param vals Output: non-zero values array (size: local_nnz)
 * @param local_n Output: number of rows assigned to this process
 * @param local_nnz Output: number of non-zeros assigned to this process
 * @param global_n Output: total number of rows in the matrix
 * @param rank MPI rank of the calling process
 * @param p Total number of MPI processes
 */
void read_csr_parallel(const char* filename, int** ptr, int** cols, double** vals,
                       int* local_n, int* local_nnz, int* global_n, 
                       int rank, int p);

#endif // CSR_IO_H

