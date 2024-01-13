#include "matrixmath.h"

void matmul_3x3_3x1(double matrix[3][3], double vec[3], double result[3]) {
  /// Multiplies a 3x3 matrix by a 3x1 vector.
  result[0] = matrix[0][0] * vec[0] + matrix[0][1] * vec[1] + matrix[0][2] * vec[2];
  result[1] = matrix[1][0] * vec[0] + matrix[1][1] * vec[1] + matrix[1][2] * vec[2];
  result[2] = matrix[2][0] * vec[0] + matrix[2][1] * vec[1] + matrix[2][2] * vec[2];
}

int matinv_3x3(double matrix[3][3], double matrix_inv[3][3]) {
  /// Inverts a 3x3 matrix.
  /// Returns 0 on success, -1 on failure.
  double d1 = matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1];
  double d2 = matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0];
  double d3 = matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0];
  
  double det = matrix[0][0] * d1 - matrix[0][1] * d2 + matrix[0][2] * d3;

  if (det == 0) {
    // Matrix is not invertible.
    return MATRIX_MATH_ERROR_NOT_INVERTIBLE;
  }
  
  double invdet = 1 / det;

  matrix_inv[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) * invdet;
  matrix_inv[0][1] = (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]) * invdet;
  matrix_inv[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) * invdet;
  matrix_inv[1][0] = (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]) * invdet;
  matrix_inv[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) * invdet;
  matrix_inv[1][2] = (matrix[1][0] * matrix[0][2] - matrix[0][0] * matrix[1][2]) * invdet;
  matrix_inv[2][0] = (matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1]) * invdet;
  matrix_inv[2][1] = (matrix[2][0] * matrix[0][1] - matrix[0][0] * matrix[2][1]) * invdet;
  matrix_inv[2][2] = (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]) * invdet;

  return MATRIX_MATH_ERROR_NONE;
}

void cross_3(double a[3], double b[3], double result[3]) {
  /// Returns the cross product of two 3-vectors.
  result[0] = a[1] * b[2] - a[2] * b[1];
  result[1] = a[2] * b[0] - a[0] * b[2];
  result[2] = a[0] * b[1] - a[1] * b[0];
}
