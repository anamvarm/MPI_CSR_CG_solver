# Contributing to Parallel CG Solver

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/yourusername/parallel-cg-solver.git
   cd parallel-cg-solver
   ```
3. **Create a branch** for your feature or fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Development Setup

### Prerequisites

- MPI implementation (OpenMPI or MPICH)
- GCC or compatible C compiler
- GNU Make
- Git

### Building

```bash
make clean
make
```

### Testing

Run the example to ensure everything works:

```bash
make run
```

## Code Style Guidelines

### C Code Style

- **Indentation**: 4 spaces (no tabs)
- **Naming Conventions**:
  - Functions: `snake_case`
  - Variables: `snake_case`
  - Constants: `UPPER_CASE`
  - Struct types: `PascalCase`
- **Comments**: Use Doxygen-style comments for functions
- **Line Length**: Maximum 100 characters

### Example

```c
/**
 * @brief Compute the dot product of two vectors
 * 
 * @param u First vector
 * @param v Second vector
 * @param n Length of vectors
 * @return Dot product result
 */
double dot(double* u, double* v, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += u[i] * v[i];
    }
    return sum;
}
```

## Contribution Areas

We welcome contributions in the following areas:

### High Priority

- [ ] Preconditioning support (Jacobi, ILU)
- [ ] Additional sparse matrix formats (COO, ELL)
- [ ] Performance benchmarks
- [ ] Unit tests
- [ ] More comprehensive examples

### Medium Priority

- [ ] GPU acceleration support
- [ ] Alternative iterative solvers (BiCGSTAB, GMRES)
- [ ] Python/Julia bindings
- [ ] Improved error handling
- [ ] Better convergence diagnostics

### Documentation

- [ ] Tutorial examples
- [ ] Performance tuning guide
- [ ] API documentation
- [ ] Video tutorials

## Submitting Changes

1. **Commit your changes** with clear messages:
   ```bash
   git add .
   git commit -m "Add feature: brief description"
   ```

2. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

3. **Create a Pull Request** on GitHub with:
   - Clear title and description
   - Reference to related issues (if any)
   - List of changes made
   - Test results

### Pull Request Checklist

- [ ] Code follows style guidelines
- [ ] Comments and documentation updated
- [ ] All tests pass
- [ ] No new compiler warnings
- [ ] Commit messages are clear
- [ ] Branch is up-to-date with main

## Reporting Bugs

When reporting bugs, please include:

1. **Environment details**:
   - OS and version
   - MPI implementation and version
   - Compiler and version

2. **Steps to reproduce**:
   - Exact commands run
   - Input files (if possible)
   - Expected vs actual behavior

3. **Error messages**:
   - Full error output
   - Stack traces (if available)

### Example Bug Report

```markdown
**Environment:**
- OS: Ubuntu 22.04
- MPI: OpenMPI 4.1.1
- Compiler: GCC 11.2

**Steps to Reproduce:**
1. Run `make`
2. Run `mpirun -np 8 bin/cg_solver -matrix large.csr -output sol.txt`
3. Observe error

**Error:**
```
Rank 5: Failed to allocate memory
MPI_Abort called
```

**Expected:** Should run successfully with 8 processes
```

## Feature Requests

We welcome feature suggestions! Please:

1. Check if the feature already exists or is planned
2. Describe the use case clearly
3. Provide examples of desired behavior
4. Indicate if you're willing to implement it

## Code Review Process

- All submissions require review
- Reviewers check:
  - Correctness
  - Performance
  - Code style
  - Documentation
- Changes may be requested
- Once approved, maintainers will merge

## Community Guidelines

- Be respectful and constructive
- Help others when possible
- Follow the code of conduct
- Keep discussions on-topic

## Questions?

- Open an issue for questions
- Tag with `question` label
- Check existing issues first

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

Thank you for contributing! 🚀

