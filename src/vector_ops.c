/**
 * @file vector_ops.c
 * @brief Implementation of vector operations and I/O
 */

#include "vector_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double dot(double* u, double* v, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += u[i] * v[i];
    }
    return sum;
}

double dot_allreduce(double local) {
    double global;
    MPI_Allreduce(&local, &global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return global;
}

double* read_vector(const char* filename, int n, int rank) {
    double* vec = malloc(n * sizeof(double));
    if (vec == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate vector\n", rank);
        return NULL;
    }
    
    if (rank == 0) {
        FILE* f = fopen(filename, "r");
        if (f == NULL) {
            fprintf(stderr, "Rank 0: Failed to open %s\n", filename);
            free(vec);
            return NULL;
        }
        for (int i = 0; i < n; i++) {
            if (fscanf(f, "%lf", &vec[i]) != 1) {
                fprintf(stderr, "Rank 0: Failed to read %d values from %s\n", n, filename);
                free(vec);
                fclose(f);
                return NULL;
            }
        }
        fclose(f);
    }
    MPI_Bcast(vec, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return vec;
}

void write_vector(const char* filename, double* x, int n, int rank) {
    if (rank == 0) {
        FILE* f = fopen(filename, "w");
        if (f == NULL) {
            fprintf(stderr, "Rank 0: Failed to open %s for writing\n", filename);
            return;
        }
        for (int i = 0; i < n; i++) {
            fprintf(f, "%.12g\n", x[i]);
        }
        fclose(f);
    }
}

