#include "matrixmath.h"
#include "unittests.h"
#include <stdio.h>

int tests_run = 0;

static char * test_matmul_3x3_3x1() {
  double matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double vec[3] = {1, 2, 3};
  double result[3];
  matmul_3x3_3x1(matrix, vec, result);
  ut_assert(result[0] == 14, "matmul_3x3_3x1 failed");
  ut_assert(result[1] == 32, "matmul_3x3_3x1 failed");
  ut_assert(result[2] == 50, "matmul_3x3_3x1 failed");
  return 0;
}

static char * test_matinv_3x3() {
  double matrix[3][3] = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
  double result[3][3];

  enum MatrixMathError status = matinv_3x3(matrix, result);

  ut_assert(status == 0, "matrix inverse failed");

  ut_assert(result[0][0] == -24, "matrix inverse failed");
  ut_assert(result[0][1] == 18, "matrix inverse failed");
  ut_assert(result[0][2] == 5, "matrix inverse failed");
  ut_assert(result[1][0] == 20, "matrix inverse failed");
  ut_assert(result[1][1] == -15, "matrix inverse failed");
  ut_assert(result[1][2] == -4, "matrix inverse failed");
  ut_assert(result[2][0] == -5, "matrix inverse failed");
  ut_assert(result[2][1] == 4, "matrix inverse failed");
  ut_assert(result[2][2] == 1, "matrix inverse failed");

  return 0;
}

static char *test_matinv_3x3_noninvertable() {
  double matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double result[3][3];

  enum MatrixMathError status = matinv_3x3(matrix, result);

  ut_assert(status == -1, "matrix inverse failed");

  return 0;
}

static char *test_magnitude() {
  double vector[3] = {3, 4, 12};
  double result = magnitude(vector);

  ut_assert_feq(result, 13.0);

  vector[0] = 1;
  vector[1] = 1;
  vector[2] = 1;
  result = magnitude(vector);
  ut_assert_feq(result, sqrt(3));

  vector[0] = 0;
  vector[1] = 0;
  vector[2] = 0;
  result = magnitude(vector);

  ut_assert_feq(result, 0.0);

  vector[0] = -1;
  vector[1] = -1;
  vector[2] = -1;
  result = magnitude(vector);

  ut_assert_feq(result, sqrt(3));
  return 0;
}

static char *test_normalize() {
  double vector[3] = {3, 4, 12};
  int status = normalize(vector);

  ut_assert(status == 0, "normalize failed");
  ut_assert_feq(vector[0], 3.0 / 13.0);
  ut_assert_feq(vector[1], 4.0 / 13.0);
  ut_assert_feq(vector[2], 12.0 / 13.0);

  vector[0] = 0;
  vector[1] = 0;
  vector[2] = 0;

  status = normalize(vector);
  ut_assert(status == -1, "normalize should have failed");
  return 0;
}

static char * all_tests() {
  ut_run_test(test_matmul_3x3_3x1);
  ut_run_test(test_matinv_3x3);
  ut_run_test(test_matinv_3x3_noninvertable);
  ut_run_test(test_magnitude);
  ut_run_test(test_normalize);  
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

