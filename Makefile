CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
UNITY_DIR = $(TEST_DIR)/unity

# List all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate object file names from source files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Executable name
EXEC = $(BIN_DIR)/schedule_manager

# Test files - EXCLUDE test_runner.c and test_main.c from wildcard
TEST_SRCS = $(filter-out $(TEST_DIR)/test_runner.c $(TEST_DIR)/test_main.c, $(wildcard $(TEST_DIR)/test_*.c))
UNITY_SRCS = $(UNITY_DIR)/unity.c
TEST_RUNNER = $(TEST_DIR)/test_runner.c
TEST_MAIN = $(TEST_DIR)/test_main.c

# Test object files
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))
UNITY_OBJ = $(OBJ_DIR)/unity.o
TEST_RUNNER_OBJ = $(OBJ_DIR)/test_runner.o
TEST_MAIN_OBJ = $(OBJ_DIR)/test_main.o

# Test executables
TEST_EXEC = $(BIN_DIR)/test_runner
TEST_MAIN_EXEC = $(BIN_DIR)/test_main

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

# Test compilation rules
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.c
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(OBJ_DIR)/test_runner.o: $(TEST_DIR)/test_runner.c
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(OBJ_DIR)/test_main.o: $(TEST_DIR)/test_main.c
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(OBJ_DIR)/unity.o: $(UNITY_DIR)/unity.c
	$(CC) $(CFLAGS) -I$(UNITY_DIR) -c $< -o $@

# Build and run all tests
test: directories $(TEST_EXEC) $(TEST_MAIN_EXEC)
	@echo "=== Running Unity Test Suite ==="
	$(TEST_EXEC)
	@echo.
	@echo "=== Running Workflow Test ==="
	$(TEST_MAIN_EXEC)

# Build test runner (excludes main.o to avoid multiple main() functions)
$(TEST_EXEC): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_OBJS) $(UNITY_OBJ) $(TEST_RUNNER_OBJ)
	$(CC) $^ -o $@ -lm

# Build workflow test (excludes main.o to avoid multiple main() functions)
$(TEST_MAIN_EXEC): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_MAIN_OBJ)
	$(CC) $^ -o $@ -lm

# Individual test targets
test-doctor: directories $(OBJ_DIR)/test_doctor_manager.o $(UNITY_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ -o $(BIN_DIR)/test_doctor_manager -lm
	$(BIN_DIR)/test_doctor_manager

test-scheduler: directories $(OBJ_DIR)/test_scheduler.o $(UNITY_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ -o $(BIN_DIR)/test_scheduler -lm
	$(BIN_DIR)/test_scheduler

test-utils: directories $(OBJ_DIR)/test_utils.o $(UNITY_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ -o $(BIN_DIR)/test_utils -lm
	$(BIN_DIR)/test_utils

test-display: directories $(OBJ_DIR)/test_display.o $(UNITY_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $^ -o $(BIN_DIR)/test_display -lm
	$(BIN_DIR)/test_display

# Help target
help:
	@echo Available targets:
	@echo   all           - Build main application
	@echo   run           - Build and run main application
	@echo   test          - Build and run all tests
	@echo   test-doctor   - Run doctor manager tests only
	@echo   test-scheduler - Run scheduler tests only
	@echo   test-utils    - Run utils tests only
	@echo   test-display  - Run display tests only
	@echo   clean         - Remove all build files
	@echo   help          - Show this help

.PHONY: all run clean directories test test-doctor test-scheduler test-utils test-display help