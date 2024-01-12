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

$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

build:
	@mkdir -p build
	@mkdir -p bin

tests: LDLIBS += $(TARGET) -Isrc
tests: $(TESTS)
	bash ./tests/runtests.sh

tests/%.o: tests/%.c src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

