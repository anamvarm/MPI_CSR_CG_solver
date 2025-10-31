/**
 * @file cg_solver.h
 * @brief Conjugate Gradient iterative solver for sparse linear systems
 * 
 * Implements the parallel Conjugate Gradient algorithm for solving
 * symmetric positive definite linear systems Ax = b.
 */

#ifndef CG_SOLVER_H
#define CG_SOLVER_H

/**
 * @brief Solve a sparse linear system using the Conjugate Gradient method
 * 
 * Solves Ax = b where A is a sparse symmetric positive definite matrix
 * stored in CSR format. The matrix is distributed across processes by rows.
 * 
 * @param ptr Row pointer array for local CSR matrix
 * @param cols Column indices for local CSR matrix
 * @param vals Non-zero values for local CSR matrix
 * @param b Right-hand side vector (local portion)
 * @param x Solution vector (local portion, initial guess on input)
 * @param local_n Number of rows assigned to this process
 * @param local_nnz Number of non-zeros assigned to this process
 * @param global_n Total number of rows in the matrix
 * @param rank MPI rank of the calling process
 * @param p Total number of MPI processes
 * @param max_iter Maximum number of CG iterations
 * @param tol Relative convergence tolerance
 */
void cg_solver(int* ptr, int* cols, double* vals, double* b, double* x,
               int local_n, int local_nnz, int global_n, 
               int rank, int p, int max_iter, double tol);

#endif // CG_SOLVER_H

