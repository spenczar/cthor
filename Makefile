CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
TARGET=build/libcthor.a
TARGET_BIN=build/cthor

.PHONY: all clean tests

all: build/libcthor.a

clean:
	rm -f $(OBJECTS)
	rm -rf build
	rm -f tests/*_tests
	rm -f tests/tests.log

# Add -lm for math.h
src/projections.o: LDLIBS += -lm

$(TARGET): LDLIBS += -lm
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

build:
	@mkdir -p build
	@mkdir -p bin

build/lib: build
	@mkdir -p build/lib

$(TESTS): CFLAGS += -Isrc -Itests -DDEBUG -g
$(TESTS): $(TARGET) $(TEST_SRC) 
	$(CC) $(CFLAGS) $@.c -o $@ $(TARGET) -lm 

tests: $(TESTS) 
	./tests/runtests.sh

valgrind-tests: $(TESTS)
	@env VALGRIND=1 ./tests/runtests.sh

tests/%.o: tests/%.c src/%.c $(TARGET)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

