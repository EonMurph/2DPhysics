# Compiler
CC = gcc

# Compiler flags
# -Wall: Enable all warnings
# -g: Generate debugging information
# -std=c99: Use the C99 standard
CFLAGS = -Wall -g -std=c99

# Preprocessor flags (for #include paths)
CPPFLAGS = -I/opt/homebrew/include/

# Linker flags (for library paths)
LDFLAGS = -L/opt/homebrew/lib

# Libraries to link against
LDLIBS = -lSDL2 -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Executable name
EXEC = $(BIN_DIR)/main

# Automatically find all .c files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate corresponding object file names in the object directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# Automatically find all .h files in the include directory
INCS = $(wildcard $(INC_DIR)/*.h)

# Phony targets do not represent files
.PHONY: all build clean run

# Default target: build the executable
all: build

# Build the executable
build: $(EXEC)

# Rule to link the executable
$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INC_DIR) -c $< -o $@

# Create directories if they don't exist.
# This is an order-only prerequisite, so it runs only if the directory is missing.
$(OBJ_DIR) $(BIN_DIR) $(INC_DIR):
	mkdir -p $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Clean and then build the executable
rebuild: clean build 

# Run the executable
run: build
	./$(EXEC)
