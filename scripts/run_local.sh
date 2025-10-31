#!/bin/bash

# Local test script for the Parallel CG Solver
# =============================================
# Simple script to test the solver on your local machine

# Number of MPI processes to use
NPROCS=${1:-4}

# Configuration
MATRIX_FILE="examples/matrix.csr"
B_FILE="examples/b.txt"
OUTPUT_FILE="solution.txt"
MAX_ITER=500
TOL=1e-6

echo "Running CG Solver locally with $NPROCS processes"
echo "================================================"

# Check if binary exists
if [ ! -f "bin/cg_solver" ]; then
    echo "Error: bin/cg_solver not found. Please run 'make' first."
    exit 1
fi

# Check if matrix file exists
if [ ! -f "$MATRIX_FILE" ]; then
    echo "Warning: $MATRIX_FILE not found."
    echo "Running without -b argument (using vector of ones)"
    mpirun -np $NPROCS bin/cg_solver \
        -matrix $MATRIX_FILE \
        -output $OUTPUT_FILE \
        -max_iter $MAX_ITER \
        -tol $TOL
else
    mpirun -np $NPROCS bin/cg_solver \
        -matrix $MATRIX_FILE \
        -b $B_FILE \
        -output $OUTPUT_FILE \
        -max_iter $MAX_ITER \
        -tol $TOL
fi

# Check result
if [ $? -eq 0 ]; then
    echo ""
    echo "Success! Solution written to $OUTPUT_FILE"
else
    echo ""
    echo "Failed to solve system"
    exit 1
fi

