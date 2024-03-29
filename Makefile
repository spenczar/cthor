CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

BENCHMARKS_SRC=$(wildcard benchmarks/*.c)
BENCHMARKS=$(patsubst %.c,%,$(BENCHMARKS_SRC))

TARGET=build/libcthor.a
TARGET_BIN=build/cthor

.PHONY: all clean tests benchmarks

all: build/libcthor.a

clean:
	rm -f $(OBJECTS)
	rm -f $(TESTS)
	rm -f $(BENCHMARKS)
	rm -f $(TARGET)
	rm -rf build
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

$(BENCHMARKS): CFLAGS += -Isrc -Ibenchmarks -O3 -march=native -mtune=native -flto
$(BENCHMARKS): $(BENCHMARKS_SRC) $(TARGET)
	$(CC) $(CFLAGS) $@.c -o $@ $(TARGET) -lm 

tests: $(TESTS) 
	./tests/runtests.sh

benchmarks: $(BENCHMARKS)

valgrind-tests: $(TESTS)
	@env VALGRIND=1 ./tests/runtests.sh

tests/%.o: tests/%.c src/%.c $(TARGET)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

format: $(SOURCES) $(TEST_SRC) $(BENCHMARKS_SRC)
	clang-format -i $^
