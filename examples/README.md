# Example Input Files

This directory contains example input files for the Parallel CG Solver.

## Files

- **matrix.csr**: Example sparse matrix in CSR binary format
- **b.txt**: Example right-hand side vector (text format)

## Creating Your Own Input Files

### CSR Matrix Format

The CSR matrix file is a binary file with the following structure:

```c
struct CSRMatrix {
    int n;              // Number of rows/columns
    int nnz;            // Number of non-zero elements
    int ptr[n+1];       // Row pointers
    int cols[nnz];      // Column indices
    double vals[nnz];   // Values
};
```

### Example: Creating a CSR Matrix in Python

```python
import numpy as np
from scipy.sparse import csr_matrix

# Create a sparse matrix (example: 5x5 tridiagonal)
n = 5
diag = np.full(n, 2.0)
off_diag = np.full(n-1, -1.0)
A = csr_matrix(
    (np.concatenate([diag, off_diag, off_diag]),
     (np.concatenate([np.arange(n), np.arange(n-1), np.arange(1,n)]),
      np.concatenate([np.arange(n), np.arange(1,n), np.arange(n-1)])))
)

# Write to binary file
with open('matrix.csr', 'wb') as f:
    np.array([n], dtype=np.int32).tofile(f)
    np.array([A.nnz], dtype=np.int32).tofile(f)
    A.indptr.astype(np.int32).tofile(f)
    A.indices.astype(np.int32).tofile(f)
    A.data.astype(np.float64).tofile(f)
```

### Example: Creating a Vector File

Simply create a text file with one value per line:

```bash
# Using bash
for i in {1..5}; do echo "1.0"; done > b.txt

# Or in Python
import numpy as np
b = np.ones(5)
np.savetxt('b.txt', b)
```

## Testing

To test with the example files:

```bash
# From the project root directory
mpirun -np 4 bin/cg_solver \
    -matrix examples/matrix.csr \
    -b examples/b.txt \
    -output solution.txt
```

## Matrix Requirements

For the CG solver to work correctly:

1. **Symmetric**: A = Aᵀ
2. **Positive Definite**: xᵀAx > 0 for all x ≠ 0
3. **Sparse**: Efficient representation benefits from sparsity

Common examples:
- Discretized Laplacian operators
- Finite element stiffness matrices
- Graph Laplacian matrices

