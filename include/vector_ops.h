/**
 * @file vector_ops.h
 * @brief Vector operations and I/O
 * 
 * Basic vector operations including dot products, norms,
 * and parallel vector I/O.
 */

#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

/**
 * @brief Compute local dot product of two vectors
 * 
 * Computes the dot product u^T * v for local vector segments.
 * Note: This only computes the local contribution. Use dot_allreduce()
 * to get the global result.
 * 
 * @param u First vector
 * @param v Second vector
 * @param n Length of vectors
 * @return Local dot product sum
 */
double dot(double* u, double* v, int n);

/**
 * @brief Perform MPI_Allreduce to sum local dot product contributions
 * 
 * @param local Local contribution to the dot product
 * @return Global dot product result
 */
double dot_allreduce(double local);

/**
 * @brief Read a vector from file and broadcast to all processes
 * 
 * Rank 0 reads the vector from file and broadcasts to all processes.
 * 
 * @param filename Path to vector file (one value per line)
 * @param n Length of the vector
 * @param rank MPI rank of the calling process
 * @return Pointer to allocated vector, or NULL on failure
 */
double* read_vector(const char* filename, int n, int rank);

/**
 * @brief Write a vector to file
 * 
 * Only rank 0 writes the vector to the file.
 * 
 * @param filename Path to output file
 * @param x Vector to write
 * @param n Length of the vector
 * @param rank MPI rank of the calling process
 */
void write_vector(const char* filename, double* x, int n, int rank);

#endif // VECTOR_OPS_H

