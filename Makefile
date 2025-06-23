CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Daftar semua file source
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate nama file object dari file source
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Nama executable
EXEC = $(BIN_DIR)/schedule_manager

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

# Target bantuan
help:
	@echo Target yang tersedia:
	@echo   all           - Build aplikasi utama
	@echo   run           - Build dan jalankan aplikasi utama
	@echo   clean         - Hapus semua file build
	@echo   help          - Tampilkan bantuan ini

.PHONY: all run clean directories help