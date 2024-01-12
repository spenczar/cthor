#include "str.h"
#include "unittests.h"
#include <stdio.h>

int tests_run = 0;

static char * test_string_new() {
  struct String string;
  int status = string_new(&string, 10);
  ut_assert(status == 0, "string_new failed");
  ut_assert(string.length == 10, "wrong length");
  ut_assert(string.data != NULL, "data is null");
  string_free(&string);
  return 0;
}

static char * test_string_create() {
  char *data = "hello";
  
  struct String string = string_create(data);
  ut_assert(string.length == 5, "wrong length");
  ut_assert(string.data != NULL, "data is null");
  ut_assert(string.data[0] == 'h', "wrong data");
  ut_assert(string.data[1] == 'e', "wrong data");
  ut_assert(string.data[2] == 'l', "wrong data");
  ut_assert(string.data[3] == 'l', "wrong data");
  ut_assert(string.data[4] == 'o', "wrong data");
  string_free(&string);

  ut_assert(string.data == NULL, "data is not null");
  ut_assert(data != NULL, "data was not copied");
  return 0;
}

static char * all_tests() {
  ut_run_test(test_string_new);
  ut_run_test(test_string_create);
  return 0;
}

int main() {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
