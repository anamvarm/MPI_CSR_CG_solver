/**
 * @file csr_io.c
 * @brief Implementation of CSR matrix I/O operations
 */

#include "csr_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void read_csr_parallel(const char* filename, int** ptr, int** cols, double** vals,
                       int* local_n, int* local_nnz, int* global_n, 
                       int rank, int p) {
    MPI_File fh;
    int err = MPI_File_open(MPI_COMM_WORLD, (char*)filename, MPI_MODE_RDONLY, 
                            MPI_INFO_NULL, &fh);
    if (err != MPI_SUCCESS) {
        char err_str[MPI_MAX_ERROR_STRING];
        int len;
        MPI_Error_string(err, err_str, &len);
        fprintf(stderr, "Rank %d: Failed to open %s: %s\n", rank, filename, err_str);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int n, nnz;
    if (rank == 0) {
        MPI_File_read(fh, &n, 1, MPI_INT, MPI_STATUS_IGNORE);
        MPI_File_read(fh, &nnz, 1, MPI_INT, MPI_STATUS_IGNORE);
        if (n <= 0 || nnz <= 0) {
            fprintf(stderr, "Rank 0: Invalid matrix dimensions: n=%d, nnz=%d\n", n, nnz);
            MPI_File_close(&fh);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nnz, 1, MPI_INT, 0, MPI_COMM_WORLD);
    *global_n = n;

    int* full_ptr = malloc((n + 1) * sizeof(int));
    if (full_ptr == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate full_ptr\n", rank);
        MPI_File_close(&fh);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_File_read_at_all(fh, 2 * sizeof(int), full_ptr, n + 1, MPI_INT, MPI_STATUS_IGNORE);

    // Simple block distribution 
    int rows_per_proc = (n + p - 1) / p;
    int row_start = rank * rows_per_proc;
    int row_end = (rank == p - 1) ? n : (rank + 1) * rows_per_proc;

    *local_n = row_end - row_start;
    *local_nnz = full_ptr[row_end] - full_ptr[row_start];

    *ptr = malloc((*local_n + 1) * sizeof(int));
    if (*ptr == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate ptr\n", rank);
        free(full_ptr);
        MPI_File_close(&fh);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    for (int i = 0; i <= *local_n; i++) {
        (*ptr)[i] = full_ptr[row_start + i] - full_ptr[row_start];
    }

    *cols = malloc(*local_nnz * sizeof(int));
    *vals = malloc(*local_nnz * sizeof(double));
    if (*cols == NULL || *vals == NULL) {
        fprintf(stderr, "Rank %d: Failed to allocate cols or vals\n", rank);
        free(*ptr); 
        free(full_ptr); 
        MPI_File_close(&fh); 
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    size_t offset_cols = 2 * sizeof(int) + (n + 1) * sizeof(int);
    size_t offset_vals = offset_cols + nnz * sizeof(int);

    MPI_File_read_at_all(fh, offset_cols + full_ptr[row_start] * sizeof(int),
                         *cols, *local_nnz, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_read_at_all(fh, offset_vals + full_ptr[row_start] * sizeof(double),
                         *vals, *local_nnz, MPI_DOUBLE, MPI_STATUS_IGNORE);

    free(full_ptr);
    MPI_File_close(&fh);
}

