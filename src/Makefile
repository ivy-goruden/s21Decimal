CC = gcc
CFLAGS = -Werror -std=c11
#CFLAGS = -Wall - Wextra - Werror - std = c11

BUILD = ../build/
SRC_DIR = ./
SRC = $(wildcard $(SRC_DIR)*.c)
TEST_SRC = $(wildcard tests/enolagal/*.c)
HEADERS = $(wildcard $(SRC_DIR)*.h)
FUNC_OBJECTS = $(SRC:.c=.o)
INCLUDES = -I $(SRC_DIR)
SANITIZE = -fsanitize=address
TEST_LIBS = -lcheck -lm -lpthread -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
TARGET = s21_test

all: clean build test

s21_decimal.a: $(FUNC_OBJECTS)
	ar r s21_decimal.a $(FUNC_OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

build: s21_decimal.a
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TARGET) $(TEST_LIBS) -L. -l:s21_decimal.a

test: clean build
	./$(TARGET)

sanitize: clean s21_decimal.a
	$(CC) $(CFLAGS) $(SANITIZE) $(TEST_SRC) -o $(BUILD)/sanitize $(TEST_LIBS) -L. -l:s21_decimal.a
	$(BUILD)/sanitize

valgrind: clean build
	valgrind --leak-check=full --tool=memcheck -q ./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f ../src/**/*.[ao] ./*.[ao]
	rm -fr ../build/*

cf:
	clang-format -i ../src/**/*.[ch]
	clang-format -i ../src/*.[ch]

cf-check:
	clang-format -n ../src/**/*.[ch]
	clang-format -n ../src/*.[ch]

gcov_report:
	mkdir -p $(BUILD)
	$(CC) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) -o $(BUILD)/gcov_report $(TEST_LIBS)
	$(BUILD)/gcov_report
	lcov -c -d $(BUILD) -o $(BUILD)/coverage.info
	genhtml -o $(BUILD)/report $(BUILD)/coverage.info
	google-chrome $(BUILD)/report/index.html
