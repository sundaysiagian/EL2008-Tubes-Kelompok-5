CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
UNITY_DIR = $(TEST_DIR)/unity

# Daftar semua file source
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate nama file object dari file source
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Nama executable
EXEC = $(BIN_DIR)/schedule_manager

# File test - KECUALIKAN test_runner.c dan test_main.c dari wildcard
TEST_SRCS = $(filter-out $(TEST_DIR)/test_runner.c $(TEST_DIR)/test_main.c, $(wildcard $(TEST_DIR)/test_*.c))
UNITY_SRCS = $(UNITY_DIR)/unity.c
TEST_RUNNER = $(TEST_DIR)/test_runner.c
TEST_MAIN = $(TEST_DIR)/test_main.c

# File object test
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))
UNITY_OBJ = $(OBJ_DIR)/unity.o
TEST_RUNNER_OBJ = $(OBJ_DIR)/test_runner.o
TEST_MAIN_OBJ = $(OBJ_DIR)/test_main.o

# Executable test
TEST_EXEC = $(BIN_DIR)/test_runner
TEST_MAIN_EXEC = $(BIN_DIR)/test_main

# Target default
all: directories $(EXEC)

# Buat direktori yang diperlukan
directories:
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Compile setiap file source jadi file object
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link semua file object jadi executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -lm

# Jalankan program
run: all
	$(EXEC)

# Bersihkan file yang dihasilkan
clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

### TESTING ###
### TESTING ###
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.c
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(OBJ_DIR)/test_runner.o: $(TEST_DIR)/test_runner.c
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(OBJ_DIR)/test_main.o: $(TEST_DIR)/test_main.c
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(OBJ_DIR)/unity.o: $(UNITY_DIR)/unity.c
	$(CC) $(CFLAGS) -I$(UNITY_DIR) -c $< -o $@

# Build dan jalankan semua test
test: directories $(TEST_EXEC) $(TEST_MAIN_EXEC)
	@echo "=== Menjalankan Unity Test Suite ==="
	$(TEST_EXEC)
	@echo.
	@echo "=== Menjalankan Workflow Test ==="
	$(TEST_MAIN_EXEC)

# Build test runner (kecualikan main.o untuk hindari multiple main())
$(TEST_EXEC): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_OBJS) $(UNITY_OBJ) $(TEST_RUNNER_OBJ)
	$(CC) $^ -o $@ -lm

# Build workflow test (kecualikan main.o untuk hindari multiple main())
$(TEST_MAIN_EXEC): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_MAIN_OBJ)
	$(CC) $^ -o $@ -lm

# Target test individual
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

# Target bantuan
help:
	@echo Target yang tersedia:
	@echo   all           - Build aplikasi utama
	@echo   run           - Build dan jalankan aplikasi utama
	@echo   test          - Build dan jalankan semua test
	@echo   test-doctor   - Jalankan test doctor manager saja
	@echo   test-scheduler - Jalankan test scheduler saja
	@echo   test-utils    - Jalankan test utils saja
	@echo   test-display  - Jalankan test display saja
	@echo   clean         - Hapus semua file build
	@echo   help          - Tampilkan bantuan ini

.PHONY: all run clean directories test test-doctor test-scheduler test-utils test-display help