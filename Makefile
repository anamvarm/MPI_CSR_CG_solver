# Makefile for Parallel CG Solver
# ================================

# Compiler and flags
MPICC = mpicc
CFLAGS = -Wall -O3 -Iinclude
LDFLAGS = -lm

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/cg_solver

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Header files
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(MPICC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(MPICC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned build artifacts"

# Clean everything including output files
distclean: clean
	rm -f solution.txt *.log
	@echo "Cleaned all generated files"

# Run example (for testing)
run: $(TARGET)
	mpirun -np 4 $(TARGET) -matrix examples/matrix.csr -b examples/b.txt -output solution.txt

# Install (optional - copies to system path)
install: $(TARGET)
	@echo "Installing to /usr/local/bin (may require sudo)"
	cp $(TARGET) /usr/local/bin/cg_solver

# Show help
help:
	@echo "Parallel CG Solver - Makefile targets:"
	@echo "  make         - Build the solver"
	@echo "  make clean   - Remove build artifacts"
	@echo "  make distclean - Remove all generated files"
	@echo "  make run     - Run example with 4 processes"
	@echo "  make install - Install to system (requires sudo)"
	@echo "  make help    - Show this help message"

.PHONY: all clean distclean run install help directories

