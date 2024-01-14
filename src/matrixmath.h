#ifndef matrixmath_h
#define matrixmath_h

enum MatrixMathError {
  MATRIX_MATH_ERROR_NONE = 0,
  MATRIX_MATH_ERROR_NOT_INVERTIBLE = -1,
};

/// Multiplies a 3x3 matrix by a 3x1 vector.
void matmul_3x3_3x1(double matrix[3][3], double vec[3], double result[3]);

/// Multiplies a 3x3 matrix by a 3x3 matrix.
void matmul_3x3_3x3(double matrix_a[3][3], double matrix_b[3][3], double result[3][3]);

/// Multiplies a 3x3 matrix by itself.
void matsquare_3x3(double matrix[3][3], double result[3][3]);

/// Multiplies a 3x3 matrix by a scalar factor.
void matscale(double matrix[3][3], double scale);

/// Inverts a 3x3 matrix.
enum MatrixMathError matinv_3x3(double matrix[3][3], double matrix_inv[3][3]);

/// Returns the cross product of two 3-vectors.
void cross(double a[3], double b[3], double result[3]);

/// Returns the dot product of two 3-vectors.
double dot(double a[3], double b[3]);

/// Returns the magnitude of a 3-vector.
double magnitude(double vec[3]);

/// Normalizes a 3-vector in place.
/// Returns 0 on success, -1 on failure.
/// Failure occurs when the vector has zero magnitude.
int normalize(double vec[3]);

#endif
