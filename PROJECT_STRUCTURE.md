# Project Structure

This document describes the organization of the Parallel CG Solver repository.

## Directory Layout

```
parallel-cg-solver/
│
├── src/                          # Source code files
│   ├── main.c                    # Main program and CLI
│   ├── cg_solver.c               # CG algorithm implementation
│   ├── csr_io.c                  # CSR matrix I/O operations
│   ├── sparse_ops.c              # Sparse matrix operations (SpMV)
│   └── vector_ops.c              # Vector operations and I/O
│
├── include/                      # Header files
│   ├── cg_solver.h               # CG solver interface
│   ├── csr_io.h                  # CSR I/O interface
│   ├── sparse_ops.h              # Sparse operations interface
│   └── vector_ops.h              # Vector operations interface
│
├── examples/                     # Example input files
│   ├── matrix.csr                # Example CSR matrix
│   ├── b.txt                     # Example RHS vector
│   └── README.md                 # Examples documentation
│
├── scripts/                      # Helper scripts
│   ├── qsub_job.sh               # PBS/Torque job submission
│   └── run_local.sh              # Local testing script
│
├── build/                        # Build artifacts (created by make)
│   └── *.o                       # Object files
│
├── bin/                          # Compiled executables (created by make)
│   └── cg_solver                 # Main executable
│
├── Makefile                      # Build configuration
├── README.md                     # Main documentation
├── LICENSE                       # MIT License
├── .gitignore                    # Git ignore rules
├── CONTRIBUTING.md               # Contribution guidelines
└── PROJECT_STRUCTURE.md          # This file

```

## Module Descriptions

### Core Solver Components

#### `main.c` / CLI
- Command-line argument parsing
- Initialization and cleanup
- MPI setup and teardown
- Orchestrates the solver workflow

#### `cg_solver.c` / `cg_solver.h`
- Implements the Conjugate Gradient algorithm
- Manages iteration loop
- Handles convergence checking
- Coordinates MPI communication

#### `csr_io.c` / `csr_io.h`
- Parallel CSR matrix reading using MPI-IO
- Block row distribution across processes
- Binary file format handling

#### `sparse_ops.c` / `sparse_ops.h`
- Sparse matrix-vector multiplication (SpMV)
- CSR format operations
- Core computational kernel

#### `vector_ops.c` / `vector_ops.h`
- Vector dot products
- MPI collective operations (Allreduce)
- Vector I/O (read/write)

## Build System

The project uses GNU Make for building:

- **`make`** - Build the solver
- **`make clean`** - Remove build artifacts
- **`make run`** - Test with example data
- **`make install`** - Install to system (requires sudo)

## Design Principles

1. **Modularity**: Each module has a single, well-defined responsibility
2. **Separation of Concerns**: I/O, computation, and communication are separated
3. **Clear Interfaces**: Header files provide clean API boundaries
4. **Documentation**: Doxygen-style comments throughout
5. **Portability**: Standard C99 and MPI for cross-platform compatibility

## Code Organization

### Header Guards
All header files use include guards:
```c
#ifndef MODULE_NAME_H
#define MODULE_NAME_H
// ... declarations ...
#endif
```

### Naming Conventions
- **Functions**: `snake_case` (e.g., `read_csr_parallel`)
- **Files**: `snake_case.c` / `snake_case.h`
- **Constants**: `UPPER_CASE`
- **Types**: `PascalCase`

### Dependencies
```
main.c
  ├── csr_io.h
  ├── cg_solver.h
  └── vector_ops.h

cg_solver.c
  ├── cg_solver.h
  ├── sparse_ops.h
  └── vector_ops.h

csr_io.c
  └── csr_io.h

sparse_ops.c
  └── sparse_ops.h

vector_ops.c
  └── vector_ops.h
```

## Communication Pattern

The solver uses the following MPI operations:

1. **MPI_File_open/read_at_all** - Parallel matrix reading
2. **MPI_Bcast** - Broadcasting scalar values
3. **MPI_Allgatherv** - Gathering distributed vectors
4. **MPI_Allreduce** - Global dot product reduction
5. **MPI_Gatherv** - Collecting final solution

## Memory Layout

Each process stores:
- **Local matrix rows**: `local_n` rows in CSR format
- **Local vectors**: `local_n` elements (x, r, d, q)
- **Global vector**: `global_n` elements (for communication)

Block distribution: Process `i` owns rows `[i*rows_per_proc, (i+1)*rows_per_proc)`

## Future Extensions

Possible additions without breaking the current structure:

- **Preconditioning**: Add `precond_ops.h/.c`
- **Alternative solvers**: Add `bicgstab_solver.h/.c`
- **GPU support**: Add `gpu_ops.h/.c`
- **Matrix generators**: Add to `scripts/`
- **Benchmarking**: Add `benchmark/` directory

## Building on Different Systems

### Linux/macOS
```bash
make
```

### Windows (with MinGW/MSYS2)
```bash
mingw32-make
```

### Windows (with Visual Studio)
Use the Developer Command Prompt or create a Visual Studio project.

### HPC Clusters
Load appropriate modules first:
```bash
module load gcc openmpi
make
```

## Version Control

The repository is set up for Git with appropriate `.gitignore` rules:
- Build artifacts excluded
- Output files excluded
- Editor-specific files excluded
- Platform-specific files excluded

## Getting Started

1. **Clone the repository**
2. **Navigate to project root**
3. **Run `make`** to build
4. **Run `make run`** to test
5. **Read `README.md`** for detailed usage

For contributing, see `CONTRIBUTING.md`.

