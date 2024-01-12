#ifndef matrixmath_h
#define matrixmath_h

enum MatrixMathError {
  MATRIX_MATH_ERROR_NONE = 0,
  MATRIX_MATH_ERROR_NOT_INVERTIBLE = -1,
};
void matmul_3x3_3x1(double matrix[3][3], double vec[3], double result[3]);

enum MatrixMathError matinv_3x3(double matrix[3][3], double matrix_inv[3][3]);

#endif
