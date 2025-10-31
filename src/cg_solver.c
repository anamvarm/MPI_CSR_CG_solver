/**
 * @file cg_solver.c
 * @brief Implementation of the Conjugate Gradient solver
 */

#include "cg_solver.h"
#include "sparse_ops.h"
#include "vector_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void cg_solver(int* ptr, int* cols, double* vals, double* b, double* x,
               int local_n, int local_nnz, int global_n, 
               int rank, int p, int max_iter, double tol) {
    double* r = calloc(local_n, sizeof(double));
    double* d = calloc(local_n, sizeof(double));
    double* q = calloc(local_n, sizeof(double));
    double* x_global = calloc(global_n, sizeof(double));
    if (r == NULL || d == NULL || q == NULL || x_global == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate CG vectors\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Pre-allocate communication arrays once, outside the loop
    int* recvcounts = malloc(p * sizeof(int));
    int* displs = malloc(p * sizeof(int));
    if (recvcounts == NULL || displs == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate recvcounts or displs\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Calculate displacements and counts once
    int rows_per_proc = (global_n + p - 1) / p;
    for (int i = 0; i < p; i++) {
        int start = i * rows_per_proc;
        int end = (start + rows_per_proc > global_n) ? global_n : start + rows_per_proc;
        recvcounts[i] = end - start;
        displs[i] = start;
    }

    // Initial residual calculation
    mat_vec_csr(ptr, cols, vals, x_global, q, local_n, global_n);
    for (int i = 0; i < local_n; i++) {
        r[i] = b[i] - q[i];
        d[i] = r[i];
    }

    double delta = dot_allreduce(dot(r, r, local_n));
    double delta0 = delta;

    if (rank == 0) {
        printf("Starting CG iterations\n");
    }

    for (int iter = 0; iter < max_iter && delta > tol * tol * delta0; iter++) {
        // Single collective communication instead of Gatherv+Bcast
        MPI_Allgatherv(d, local_n, MPI_DOUBLE, x_global, recvcounts, displs,
                       MPI_DOUBLE, MPI_COMM_WORLD);

        mat_vec_csr(ptr, cols, vals, x_global, q, local_n, global_n);

        double alpha_num = delta;
        double alpha_den = dot_allreduce(dot(d, q, local_n));

        if (alpha_den == 0.0) {
            if (rank == 0) {
                fprintf(stderr, "CG breakdown: alpha_den = 0\n");
            }
            break;
        }

        double alpha = alpha_num / alpha_den;

        // Update solution and residual
        for (int i = 0; i < local_n; i++) {
            x[i] += alpha * d[i];
            r[i] -= alpha * q[i];
        }

        double delta_new = dot_allreduce(dot(r, r, local_n));
        double beta = delta_new / delta;

        for (int i = 0; i < local_n; i++) {
            d[i] = r[i] + beta * d[i];
        }

        delta = delta_new;

        // Optional: Print progress every 10 iterations
        if (rank == 0 && (iter + 1) % 10 == 0) {
            printf("  Iteration %d: residual = %.6e\n", iter + 1, delta / delta0);
        }
    }

    free(r);
    free(d);
    free(q);
    free(x_global);
    free(recvcounts);
    free(displs);
}

