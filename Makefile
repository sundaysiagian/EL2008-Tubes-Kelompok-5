CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate object file names from source files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Executable name
EXEC = $(BIN_DIR)/schedule_manager

# Default target
all: directories $(EXEC)

# Create required directories
directories:
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -lm

# Run the program
run: all
	$(EXEC)

# Clean generated files
clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

.PHONY: all run clean directories