# Quick Start Guide

Get up and running with the Parallel CG Solver in minutes!

## Prerequisites

Before you begin, ensure you have:
- ✅ MPI implementation (OpenMPI or MPICH)
- ✅ C compiler with MPI support
- ✅ GNU Make

### Installing Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential libopenmpi-dev openmpi-bin
```

**CentOS/RHEL:**
```bash
sudo yum groupinstall "Development Tools"
sudo yum install openmpi openmpi-devel
module load mpi/openmpi-x86_64
```

**macOS (with Homebrew):**
```bash
brew install open-mpi
```

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/parallel-cg-solver.git
cd parallel-cg-solver
```

### 2. Build the Solver

```bash
make
```

This creates the executable at `bin/cg_solver`.

### 3. Verify Installation

```bash
bin/cg_solver --help
```

You should see the usage information.

## Running Your First Solve

### Using the Example Files

The repository includes example files in the `examples/` directory:

```bash
# Run with 4 MPI processes
mpirun -np 4 bin/cg_solver \
    -matrix examples/matrix.csr \
    -b examples/b.txt \
    -output solution.txt
```

### Using the Test Script

```bash
# Make the script executable (Linux/macOS)
chmod +x scripts/run_local.sh

# Run with default settings (4 processes)
./scripts/run_local.sh

# Or specify number of processes
./scripts/run_local.sh 8
```

## Basic Usage

### Minimal Command

```bash
mpirun -np 4 bin/cg_solver -matrix input.csr -output solution.txt
```

This uses:
- A vector of ones as the right-hand side
- Default tolerance: 1e-6
- Default max iterations: 1000

### Full Command

```bash
mpirun -np 4 bin/cg_solver \
    -matrix input.csr \
    -b rhs.txt \
    -output solution.txt \
    -max_iter 500 \
    -tol 1e-8
```

## Common Workflows

### 1. Testing with Different Process Counts

```bash
# Single process (for debugging)
mpirun -np 1 bin/cg_solver -matrix examples/matrix.csr -output sol1.txt

# Multiple processes (for performance)
for np in 2 4 8 16; do
    echo "Running with $np processes..."
    mpirun -np $np bin/cg_solver \
        -matrix examples/matrix.csr \
        -output sol_$np.txt
done
```

### 2. Adjusting Convergence Parameters

```bash
# Tighter tolerance (more accurate)
mpirun -np 4 bin/cg_solver \
    -matrix examples/matrix.csr \
    -output solution.txt \
    -tol 1e-10

# More iterations (for difficult problems)
mpirun -np 4 bin/cg_solver \
    -matrix examples/matrix.csr \
    -output solution.txt \
    -max_iter 5000
```

### 3. Running on a Cluster

On HPC systems with PBS/Torque:

```bash
# Edit scripts/qsub_job.sh to set your parameters
qsub scripts/qsub_job.sh
```

On SLURM systems, create a batch script:

```bash
#!/bin/bash
#SBATCH --job-name=cg_solver
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=16
#SBATCH --time=01:00:00

module load openmpi

mpirun bin/cg_solver \
    -matrix matrix.csr \
    -output solution.txt
```

## Creating Your Own Input

### From Python (using SciPy)

```python
import numpy as np
from scipy.sparse import csr_matrix

# Create a test matrix (example: 2D Laplacian)
n = 100
diag = np.full(n, 4.0)
off1 = np.full(n-1, -1.0)
off10 = np.full(n-10, -1.0)

A = csr_matrix(...)  # Your matrix here

# Write to CSR binary format
with open('my_matrix.csr', 'wb') as f:
    np.array([n], dtype=np.int32).tofile(f)
    np.array([A.nnz], dtype=np.int32).tofile(f)
    A.indptr.astype(np.int32).tofile(f)
    A.indices.astype(np.int32).tofile(f)
    A.data.astype(np.float64).tofile(f)

# Create RHS vector
b = np.ones(n)
np.savetxt('my_b.txt', b)

# Now solve
# mpirun -np 4 bin/cg_solver -matrix my_matrix.csr -b my_b.txt -output my_solution.txt
```

### From MATLAB

```matlab
% Create or load your sparse matrix
A = sprand(1000, 1000, 0.01);
A = A + A' + speye(1000) * 10;  % Make symmetric positive definite

% Save in CSR format
[n, ~] = size(A);
[i, j, v] = find(A);
A_csr = sparse(i, j, v, n, n);

% Write binary file (requires custom function)
write_csr_binary('matrix.csr', A_csr);

% Create RHS
b = ones(n, 1);
save('b.txt', 'b', '-ascii');
```

## Troubleshooting

### "Failed to open matrix file"
- Check file path is correct
- Ensure file is in CSR binary format
- Verify MPI-IO is enabled in your MPI installation

### "CG breakdown"
- Matrix may not be symmetric positive definite
- Try a different matrix
- Check matrix construction

### Slow convergence
- Matrix may be ill-conditioned
- Increase max_iter
- Consider using a preconditioner (future feature)

### Build errors
```bash
# Clean and rebuild
make clean
make

# Check MPI compiler
mpicc --version
which mpicc
```

## Performance Tips

1. **Process Count**: Use powers of 2 for best performance (2, 4, 8, 16, ...)
2. **Matrix Size**: Ensure n ≫ p (matrix size much larger than process count)
3. **Network**: On clusters, use high-performance interconnect (InfiniBand)
4. **Binding**: Bind MPI processes to cores:
   ```bash
   mpirun --bind-to core -np 4 bin/cg_solver ...
   ```

## Next Steps

- 📖 Read the full [README.md](README.md) for detailed documentation
- 🔧 Check [examples/README.md](examples/README.md) for creating input files
- 🤝 See [CONTRIBUTING.md](CONTRIBUTING.md) to contribute
- 📂 Review [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) to understand the code

## Getting Help

- 🐛 Found a bug? [Open an issue](https://github.com/yourusername/parallel-cg-solver/issues)
- 💡 Have a question? Check existing issues or open a new one
- 🚀 Want to contribute? See CONTRIBUTING.md

Happy solving! 🎯

