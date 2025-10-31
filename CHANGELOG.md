# Changelog

All notable changes to the Parallel CG Solver project.

## [2.0.0] - 2025-10-31

### Major Refactoring: Professional Repository Structure

This release completely restructures the codebase from a single monolithic file into a professional, modular project suitable for GitHub.

### Added

#### Project Structure
- **Source Organization**: Split into `src/` and `include/` directories
- **Modular Design**: 
  - `csr_io.c/h` - CSR matrix I/O operations
  - `cg_solver.c/h` - Conjugate Gradient algorithm
  - `sparse_ops.c/h` - Sparse matrix operations
  - `vector_ops.c/h` - Vector operations and utilities
  - `main.c` - Clean CLI interface

#### Build System
- **Makefile**: Professional build system with multiple targets
  - `make` - Build the project
  - `make clean` - Clean build artifacts
  - `make run` - Run test example
  - `make install` - System installation
  - `make help` - Display help

#### Documentation
- **README.md**: Comprehensive project documentation
  - Features overview
  - Installation instructions
  - Usage examples
  - File format specifications
  - Algorithm description
- **QUICKSTART.md**: Get started in minutes
- **CONTRIBUTING.md**: Contribution guidelines
- **PROJECT_STRUCTURE.md**: Detailed code organization
- **CHANGELOG.md**: This file
- **examples/README.md**: Example files documentation

#### Scripts
- **scripts/run_local.sh**: Local testing script
- **scripts/qsub_job.sh**: Improved PBS job submission script

#### Configuration Files
- **.gitignore**: Comprehensive ignore rules for C/MPI projects
- **LICENSE**: MIT License

### Changed

#### Code Improvements
- **Better Documentation**: Doxygen-style comments throughout
- **Improved Error Handling**: More descriptive error messages
- **Enhanced Logging**: Better progress reporting during CG iterations
- **Code Style**: Consistent formatting and naming conventions

#### CLI Enhancements
- Added `--help` / `-h` flag
- Better error messages for missing arguments
- Improved usage information

### Improved

#### Algorithm
- Fixed convergence criterion (using tolerance squared)
- Added iteration progress reporting
- Better handling of edge cases

#### Performance
- Pre-allocated communication buffers (moved outside CG loop)
- More efficient MPI collective operations
- Reduced memory allocations

### Technical Details

#### From Monolithic to Modular

**Before:**
```
op_cg.c (337 lines - everything in one file)
```

**After:**
```
src/
  ├── main.c (185 lines) - CLI and orchestration
  ├── cg_solver.c (104 lines) - CG algorithm
  ├── csr_io.c (91 lines) - Matrix I/O
  ├── sparse_ops.c (14 lines) - SpMV operation
  └── vector_ops.c (61 lines) - Vector operations

include/
  ├── cg_solver.h
  ├── csr_io.h
  ├── sparse_ops.h
  └── vector_ops.h
```

#### Benefits of Refactoring
1. **Maintainability**: Each module has a single responsibility
2. **Testability**: Individual components can be tested separately
3. **Reusability**: Modules can be used in other projects
4. **Clarity**: Clear separation between I/O, computation, and communication
5. **Documentation**: Better organized with focused documentation per module

### Repository Structure

```
parallel-cg-solver/
├── src/                    # Source files
├── include/                # Header files
├── examples/               # Example data
├── scripts/                # Helper scripts
├── build/                  # Build artifacts (generated)
├── bin/                    # Executables (generated)
├── Makefile               # Build system
├── README.md              # Main documentation
├── QUICKSTART.md          # Quick start guide
├── CONTRIBUTING.md        # Contribution guidelines
├── PROJECT_STRUCTURE.md   # Code organization
├── CHANGELOG.md           # This file
├── LICENSE                # MIT License
└── .gitignore            # Git ignore rules
```

### Migration Notes

If you're upgrading from the old monolithic version:

1. **Building**: Use `make` instead of manual compilation
2. **Running**: Executable is now at `bin/cg_solver` instead of `./op_cg`
3. **Examples**: Example files moved to `examples/` directory
4. **Scripts**: Job scripts moved to `scripts/` directory

### Breaking Changes

- **Executable name**: Changed from `op_cg` to `bin/cg_solver`
- **Location**: Executable now in `bin/` subdirectory
- **Build process**: Now uses Makefile instead of direct mpicc command

### Backwards Compatibility

The command-line interface remains unchanged:
```bash
# Old (still works with new executable path)
mpirun -np 4 bin/cg_solver -matrix matrix.csr -b b.txt -output solution.txt

# File formats unchanged
# Input CSR format: same
# Vector format: same
# Output format: same
```

### Statistics

- **Files Created**: 20+ files
- **Lines of Code**: ~600 LOC (including documentation)
- **Documentation**: 400+ lines
- **Modules**: 5 distinct modules
- **Test Scripts**: 2

### Contributors

- Initial refactoring and documentation

### Acknowledgments

Thanks to the MPI community for best practices and patterns used in this refactoring.

---

## [1.0.0] - Previous

### Initial Release

- Single-file implementation (`op_cg.c`)
- Basic CG solver with MPI parallelization
- CSR matrix format support
- Command-line interface
- PBS job submission script

---

## Future Plans

### [2.1.0] - Planned
- [ ] Preconditioning support (Jacobi, ILU)
- [ ] Unit tests
- [ ] Performance benchmarks
- [ ] Continuous Integration

### [3.0.0] - Planned
- [ ] GPU acceleration
- [ ] Alternative solvers (BiCGSTAB, GMRES)
- [ ] Python bindings
- [ ] Matrix format conversion tools

---

For more details, see the [README.md](README.md) or visit the [GitHub repository](https://github.com/yourusername/parallel-cg-solver).

