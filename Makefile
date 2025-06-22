CC = gcc
# Add dependency generation flags
CFLAGS = -Wall -g -MMD -MP

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
# Get a list of dependency files that will be generated
DEPS = $(OBJ:.o=.d)
EXECUTABLE = doctor_scheduler

all: directories $(EXECUTABLE)

directories:
    @mkdir -p $(OBJ_DIR)
    @mkdir -p data

$(EXECUTABLE): $(OBJ)
    $(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
    rm -rf $(OBJ_DIR)
    rm -f $(EXECUTABLE)

run: all
    ./$(EXECUTABLE)

# Include the dependency files. The '-' means make will not complain if the files don't exist.
-include $(DEPS)