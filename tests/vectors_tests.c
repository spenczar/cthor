#include <stdio.h>
#include <stdlib.h>

#include "unittests.h"
#include "vectors.h"

int tests_run = 0;

static char *test_vector_new() {
  struct Vec vec;
  vector_new(&vec, 10, sizeof(double));
  ut_assert(vec.length == 0, "wrong length");
  ut_assert(vec.capacity == 10, "wrong capacity");
  ut_assert(vec.item_size == sizeof(double), "wrong item size");
  ut_assert(vec.data != NULL, "data is null");
  vector_free(&vec);
  return 0;
}

static char *test_vector_new_invalid_capacity() {
  struct Vec vec;
  int status = vector_new(&vec, 0, sizeof(double));
  ut_assert(status == -1, "invalid capacity");
  return 0;
}

static char *test_vec_f64_new() {
  struct VecF64 vec;
  vec_f64_new(&vec, 10);
  ut_assert(vec.length == 0, "wrong length");
  ut_assert(vec.capacity == 10, "wrong capacity");
  ut_assert(vec.data != NULL, "data is null");
  vec_f64_free(&vec);
  return 0;
}

static char *test_vec_f64_new_invalid_capacity() {
  struct VecF64 vec;
  int status = vec_f64_new(&vec, 0);
  ut_assert(status == -1, "did not report invalid capacity");
  return 0;
}

static char *test_vec_f64_push() {
  struct VecF64 vec;
  vec_f64_new(&vec, 1);
  vec_f64_push(&vec, 1.0);
  ut_assert(vec.length == 1, "stage 1: wrong length");
  ut_assert(vec.capacity == 1, "stage 1: wrong capacity");
  ut_assert(vec.data[0] == 1.0, "stage 1: wrong value");

  vec_f64_push(&vec, 2.0);
  ut_assert(vec.length == 2, "stage 2: wrong length");
  ut_assert(vec.capacity == 2, "stage 2: wrong capacity");
  ut_assert(vec.data[0] == 1.0, "stage 2: wrong value 0");
  ut_assert(vec.data[1] == 2.0, "stage 2: wrong value 1");

  vec_f64_push(&vec, 3.0);
  ut_assert(vec.length == 3, "stage 3: wrong length");
  ut_assert(vec.capacity == 4, "stage 3: wrong capacity");
  ut_assert(vec.data[0] == 1.0, "stage 3: wrong value 0");
  ut_assert(vec.data[1] == 2.0, "stage 3: wrong value 1");
  ut_assert(vec.data[2] == 3.0, "stage 3: wrong value 2");

  vec_f64_push(&vec, 4.0);
  ut_assert(vec.length == 4, "stage 4: wrong length");
  ut_assert(vec.capacity == 4, "stage 4: wrong capacity");
  ut_assert(vec.data[0] == 1.0, "stage 4: wrong value 0");
  ut_assert(vec.data[1] == 2.0, "stage 4: wrong value 1");
  ut_assert(vec.data[2] == 3.0, "stage 4: wrong value 2");
  ut_assert(vec.data[3] == 4.0, "stage 4: wrong value 3");

  vec_f64_free(&vec);
  return 0;
}

static char *test_vec_f64_get() {
  struct VecF64 vec;
  vec_f64_new(&vec, 8);
  vec_f64_push(&vec, 1.0);
  vec_f64_push(&vec, 2.0);
  vec_f64_push(&vec, 3.0);

  double value;
  int status = vec_f64_get(&vec, 0, &value);
  ut_assert(status == 0, "stage 1: get failed");
  ut_assert(value == 1.0, "stage 1: wrong value");

  status = vec_f64_get(&vec, 1, &value);
  ut_assert(status == 0, "stage 2: get failed");
  ut_assert(value == 2.0, "stage 2: wrong value");

  status = vec_f64_get(&vec, 2, &value);
  ut_assert(status == 0, "stage 3: get failed");
  ut_assert(value == 3.0, "stage 3: wrong value");

  status = vec_f64_get(&vec, 3, &value);
  ut_assert(status == -1, "stage 4: get failed");

  status = vec_f64_get(&vec, -1, &value);
  ut_assert(status == -1, "stage 5: get failed");

  vec_f64_free(&vec);
  return 0;
}

static char *all_tests() {
  ut_run_test(test_vector_new);
  ut_run_test(test_vector_new_invalid_capacity);
  ut_run_test(test_vec_f64_new);
  ut_run_test(test_vec_f64_new_invalid_capacity);
  ut_run_test(test_vec_f64_push);
  ut_run_test(test_vec_f64_get);
  return 0;
}

int main() {
  char *result = all_tests();
  if (result != 0) {
    printf("FAILURE: %s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
