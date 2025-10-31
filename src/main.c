/**
 * @file main.c
 * @brief Main program for parallel CG solver
 * 
 * Command-line interface for solving sparse linear systems using
 * the Conjugate Gradient method with MPI parallelization.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "csr_io.h"
#include "cg_solver.h"
#include "vector_ops.h"

/**
 * @brief Print usage information
 */
void print_usage(const char* prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -matrix <file>    CSR matrix file (required)\n");
    printf("  -b <file>         Right-hand side vector file (optional, default: ones)\n");
    printf("  -output <file>    Output solution file (required)\n");
    printf("  -max_iter <n>     Maximum iterations (default: 1000)\n");
    printf("  -tol <value>      Convergence tolerance (default: 1e-6)\n");
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    const char* matrix_file = NULL;
    const char* b_file = NULL;
    const char* x_file = NULL;
    int max_iter = 1000;
    double tol = 1e-6;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-matrix") == 0) {
            if (++i < argc) {
                matrix_file = argv[i];
            } else {
                if (rank == 0) fprintf(stderr, "Error: -matrix requires a filename\n");
                MPI_Finalize();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-b") == 0) {
            if (++i < argc) {
                b_file = argv[i];
            } else {
                if (rank == 0) fprintf(stderr, "Error: -b requires a filename\n");
                MPI_Finalize();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-output") == 0) {
            if (++i < argc) {
                x_file = argv[i];
            } else {
                if (rank == 0) fprintf(stderr, "Error: -output requires a filename\n");
                MPI_Finalize();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-max_iter") == 0) {
            if (++i < argc) {
                max_iter = atoi(argv[i]);
            } else {
                if (rank == 0) fprintf(stderr, "Error: -max_iter requires a value\n");
                MPI_Finalize();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-tol") == 0) {
            if (++i < argc) {
                tol = atof(argv[i]);
            } else {
                if (rank == 0) fprintf(stderr, "Error: -tol requires a value\n");
                MPI_Finalize();
                return 1;
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            if (rank == 0) print_usage(argv[0]);
            MPI_Finalize();
            return 0;
        }
    }

    // Validate required arguments
    if (matrix_file == NULL || x_file == NULL) {
        if (rank == 0) {
            fprintf(stderr, "Error: Missing required arguments\n\n");
            print_usage(argv[0]);
            if (matrix_file == NULL) fprintf(stderr, "\n  Missing: -matrix argument\n");
            if (x_file == NULL) fprintf(stderr, "  Missing: -output argument\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Read matrix
    if (rank == 0) printf("Reading matrix from %s\n", matrix_file);
    int local_n, local_nnz, global_n;
    int* ptr = NULL;
    int* cols = NULL;
    double* vals = NULL;
    read_csr_parallel(matrix_file, &ptr, &cols, &vals, 
                      &local_n, &local_nnz, &global_n, rank, p);
    if (rank == 0) printf("Matrix read complete: global_n=%d\n", global_n);

    // Allocate and initialize right-hand side vector
    double* b = malloc(local_n * sizeof(double));
    if (b == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate b\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    if (b_file) {
        if (rank == 0) printf("Reading vector from %s\n", b_file);
        double* full_b = read_vector(b_file, global_n, rank);
        if (full_b == NULL) {
            fprintf(stderr, "Rank %d: Failed to read vector from %s\n", rank, b_file);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        memcpy(b, full_b + rank * local_n, local_n * sizeof(double));
        free(full_b);
    } else {
        if (rank == 0) printf("No b file specified, using vector of ones\n");
        for (int i = 0; i < local_n; i++) {
            b[i] = 1.0;
        }
    }

    // Allocate solution vector (initial guess: zero)
    double* x_local = calloc(local_n, sizeof(double));
    if (x_local == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate x_local\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Solve the system
    if (rank == 0) printf("Starting CG solver\n");
    double start = MPI_Wtime();
    cg_solver(ptr, cols, vals, b, x_local, local_n, local_nnz, global_n, 
              rank, p, max_iter, tol);
    double elapsed = MPI_Wtime() - start;
    if (rank == 0) {
        printf("CG solver complete\n");
        fflush(stdout);
    }

    // Gather solution on rank 0
    double* x_global = NULL;
    int* recvcounts = NULL;
    int* displs = NULL;
    if (rank == 0) {
        x_global = malloc(global_n * sizeof(double));
        recvcounts = malloc(p * sizeof(int));
        displs = malloc(p * sizeof(int));
        if (x_global == NULL || recvcounts == NULL || displs == NULL) {
            fprintf(stderr, "Rank 0: Failed to allocate x_global or gather arrays\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    
    int rows_per_proc = (global_n + p - 1) / p;
    if (rank == 0) {
        for (int i = 0; i < p; i++) {
            int start = i * rows_per_proc;
            int end = (start + rows_per_proc > global_n) ? global_n : start + rows_per_proc;
            recvcounts[i] = end - start;
            displs[i] = start;
        }
    }
    MPI_Gatherv(x_local, local_n, MPI_DOUBLE, x_global, recvcounts, displs, 
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Write solution and cleanup
    if (rank == 0) {
        printf("Solved system in %.3fs\n", elapsed);
        fflush(stdout);
        write_vector(x_file, x_global, global_n, rank);
        free(x_global);
        free(recvcounts);
        free(displs);
    }

    free(ptr);
    free(cols);
    free(vals);
    free(b);
    free(x_local);
    
    MPI_Finalize();
    return 0;
}

