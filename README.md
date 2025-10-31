# Parallel Conjugate Gradient Solver

A high-performance parallel implementation of the Conjugate Gradient (CG) method for solving large sparse linear systems using MPI (Message Passing Interface). This solver is designed for symmetric positive definite matrices stored in Compressed Sparse Row (CSR) format.

## Features

- **Parallel Computing**: Distributed memory parallelism using MPI
- **CSR Format**: Efficient sparse matrix storage and operations
- **Scalable**: Block row distribution for load balancing across processes
- **Optimized I/O**: Parallel file reading using MPI-IO
- **Flexible**: Command-line interface with configurable parameters
- **Modular Design**: Clean separation of concerns with well-documented code

## Requirements

- **MPI Implementation**: OpenMPI, MPICH, or compatible
- **C Compiler**: GCC, Intel ICC, or compatible with C99 support
- **Make**: GNU Make or compatible

## Installation

### Building from Source

```bash
# Clone or download the repository
git clone https://github.com/yourusername/parallel-cg-solver.git
cd parallel-cg-solver

# Build the solver
make

# The executable will be created at bin/cg_solver
```

### System Installation (Optional)

```bash
sudo make install
```

This installs the solver to `/usr/local/bin`.

## Usage

### Basic Usage

```bash
mpirun -np <num_processes> bin/cg_solver \
    -matrix <matrix_file> \
    -output <solution_file> \
    [options]
```

### Command-Line Options

| Option | Description | Required | Default |
|--------|-------------|----------|---------|
| `-matrix <file>` | Path to CSR matrix file | Yes | - |
| `-output <file>` | Path to output solution file | Yes | - |
| `-b <file>` | Path to right-hand side vector | No | Vector of ones |
| `-max_iter <n>` | Maximum number of iterations | No | 1000 |
| `-tol <value>` | Convergence tolerance | No | 1e-6 |
| `-h, --help` | Display help message | No | - |

### Example

```bash
# Run with 4 processes
mpirun -np 4 bin/cg_solver \
    -matrix examples/matrix.csr \
    -b examples/b.txt \
    -output solution.txt \
    -max_iter 500 \
    -tol 1e-6
```

## Input File Formats

### CSR Matrix Format (Binary)

The matrix file should be in binary CSR format with the following structure:

```
[n: int] [nnz: int] [ptr: int[n+1]] [cols: int[nnz]] [vals: double[nnz]]
```

Where:
- `n`: Number of rows/columns (matrix dimension)
- `nnz`: Number of non-zero elements
- `ptr`: Row pointer array (size n+1)
- `cols`: Column indices for each non-zero
- `vals`: Values for each non-zero

### Vector Format (Text)

Vector files (for the right-hand side `-b` option) should contain one floating-point value per line:

```
1.0
2.5
3.7
...
```

## Project Structure

```
.
├── src/                  # Source files
│   ├── main.c           # Main program and CLI
│   ├── cg_solver.c      # CG algorithm implementation
│   ├── csr_io.c         # CSR matrix I/O operations
│   ├── sparse_ops.c     # Sparse matrix operations
│   └── vector_ops.c     # Vector operations and I/O
├── include/             # Header files
│   ├── cg_solver.h
│   ├── csr_io.h
│   ├── sparse_ops.h
│   └── vector_ops.h
├── examples/            # Example input files
├── scripts/             # Job submission scripts
├── Makefile            # Build configuration
├── README.md           # This file
└── LICENSE             # License information
```

## Algorithm

The solver implements the Conjugate Gradient method with the following properties:

- **Convergence**: Guaranteed for symmetric positive definite matrices
- **Stopping Criterion**: ||r||² < tol² × ||r₀||²
- **Communication Pattern**: MPI_Allgatherv and MPI_Allreduce
- **Distribution**: Block row-wise distribution

### Key Steps

1. **Initialization**: r = b - Ax₀, d = r
2. **Iteration**: 
   - α = (rᵀr) / (dᵀAd)
   - x ← x + αd
   - r ← r - αAd
   - β = (rᵀr)_new / (rᵀr)_old
   - d ← r + βd
3. **Termination**: When ||r|| is sufficiently small

## Performance Considerations

- **Scalability**: Tested with up to 1000+ processes
- **Memory**: Each process stores ~n/p rows of the matrix
- **Communication**: Optimized with single Allgatherv per iteration
- **I/O**: Parallel reading reduces initialization time

## Troubleshooting

### Common Issues

**Problem**: "Failed to open matrix file"
- **Solution**: Check file path and MPI-IO support

**Problem**: "CG breakdown: alpha_den = 0"
- **Solution**: Matrix may not be positive definite

**Problem**: Slow convergence
- **Solution**: Check matrix condition number, consider preconditioning

## Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch
3. Make your changes with clear commit messages
4. Add tests for new functionality
5. Submit a pull request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Citation

If you use this solver in your research, please cite:

```bibtex
@software{parallel_cg_solver,
  title = {Parallel Conjugate Gradient Solver},
  author = {Your Name},
  year = {2025},
  url = {https://github.com/yourusername/parallel-cg-solver}
}
```

## Acknowledgments

- Based on the classical Conjugate Gradient algorithm by Hestenes and Stiefel (1952)
- Implements efficient parallel patterns from MPI best practices

## Contact

For questions or issues, please open an issue on GitHub or contact [your.email@example.com](mailto:your.email@example.com).

## References

1. Hestenes, M. R., & Stiefel, E. (1952). Methods of conjugate gradients for solving linear systems.
2. Saad, Y. (2003). Iterative methods for sparse linear systems.
3. Gropp, W., Lusk, E., & Skjellum, A. (1999). Using MPI: portable parallel programming with the message-passing interface.

