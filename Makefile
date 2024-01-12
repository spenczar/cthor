CC = gcc

CFLAGS = -Wall -Wextra -Werror

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
TARGET=build/libcthor.a
TARGET_BIN=build/cthor

.PHONY: all clean tests

all: build/libcthor.a

clean:
	rm -rf build
	rm tests/*_tests
	rm tests/tests.log

$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

build:
	@mkdir -p build
	@mkdir -p bin

$(TESTS): $(TARGET) $(TEST_SRC)
	$(CC) $(CFLAGS) $@.c -o $@ $(TARGET) -Isrc

tests: $(TESTS) 
	./tests/runtests.sh

valgrind-tests: $(TESTS)
	@env VALGRIND=1 ./tests/runtests.sh

tests/%.o: tests/%.c src/%.c $(TARGET)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

