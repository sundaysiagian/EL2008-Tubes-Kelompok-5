CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRC = src/main.c src/display.c src/doctor_manager.c src/scheduler.c src/utils.c
TESTS = tests/test_main.c
OBJS = $(SRC:.c=.o)
TARGET = main
TEST_TARGET = test_main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: $(SRC) $(TESTS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(SRC) $(TESTS)
	./$(TEST_TARGET)

clean:
	del /Q *.o $(TARGET).exe $(TEST_TARGET).exe 2>nul || rm -f *.o $(TARGET) $(TEST_TARGET)

display: src/display.c
	$(CC) $(CFLAGS) -o display src/display.c
	./display

.PHONY: all test clean display