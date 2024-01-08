#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


typedef double TimestampMJD;

struct String {
  size_t length;
  char *data;
};

void ct_matrix_multiply_3x3_3x1(double matrix[3][3], double vec[3], double result[3]) {
  /// Multiplies a 3x3 matrix by a 3x1 vector.
  result[0] = matrix[0][0] * vec[0] + matrix[0][1] * vec[1] + matrix[0][2] * vec[2];
  result[1] = matrix[1][0] * vec[0] + matrix[1][1] * vec[1] + matrix[1][2] * vec[2];
  result[2] = matrix[2][0] * vec[0] + matrix[2][1] * vec[1] + matrix[2][2] * vec[2];
}

int ct_matrix_invert_3x3(double matrix[3][3], double matrix_inv[3][3]) {
  /// Inverts a 3x3 matrix.
  /// Returns 0 on success, -1 on failure.
  double d1 = matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1];
  double d2 = matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0];
  double d3 = matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0];
  
  double det = matrix[0][0] * d1 - matrix[0][1] * d2 + matrix[0][2] * d3;

  if (det == 0) {
    // Matrix is not invertible.
    return -1;
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

  return 0;
}

struct VecF64 {
  size_t length;
  double *data;
};

void ct_vec_f64_free(struct VecF64 *vec) {
  if (vec->data != NULL) {
    free(vec->data);
    vec->data = NULL;
  }
}

int ct_vec_f64_new(struct VecF64 *vec, size_t length) {
  vec->length = length;
  vec->data = malloc(length * sizeof(double));
  if (vec->data == NULL) {
    return -1;
  }
  return 0;
}

struct VecTimestampMJD {
  size_t length;
  TimestampMJD *data;
};

void ct_vec_timestamp_mjd_free(struct VecTimestampMJD *vec) {
  if (vec->data != NULL) {
    free(vec->data);
    vec->data = NULL;
  }
}

int ct_vec_timestamp_mjd_new(struct VecTimestampMJD *vec, size_t length) {
  vec->length = length;
  vec->data = malloc(length * sizeof(TimestampMJD));
  if (vec->data == NULL) {
    return -1;
  }
  return 0;
}

struct TopocentricPointSources {
  struct VecF64 ra;
  struct VecF64 dec;
  struct VecTimestampMJD t;
  struct String *obscode;
};

struct CartesianPointSources {
  // Always heliocentric
  struct VecF64 x;
  struct VecF64 y;
  struct VecF64 z;
  struct VecTimestampMJD t;
};

struct CartesianPointSource {
  /// A single point source in Cartesian coordinates.
  /// Always heliocentric
  double x;
  double y;
  double z;
  TimestampMJD t;
};

struct GnomonicPointSources {
  // Always heliocentric. Relative to some center point.
  struct VecF64 x;
  struct VecF64 y;
  struct VecTimestampMJD t;
};

void ct_gnomonic_point_sources_free(struct GnomonicPointSources *gnomonic) {
  ct_vec_f64_free(&gnomonic->x);
  ct_vec_f64_free(&gnomonic->y);
  ct_vec_timestamp_mjd_free(&gnomonic->t);
}

int ct_gnomonic_point_sources_new(struct GnomonicPointSources *gnomonic, size_t length) {
  if (ct_vec_f64_new(&gnomonic->x, length) != 0) {
    goto fail;
  }
  if (ct_vec_f64_new(&gnomonic->y, length) != 0) {
    goto fail;
  }
  if (ct_vec_timestamp_mjd_new(&gnomonic->t, length) != 0) {
    goto fail;
  }

  return 0;

 fail:
  ct_gnomonic_point_sources_free(gnomonic);
  return -1;
}

int CT_ERR_INVALID_CENTER = 1;
int CT_ERR_NOT_INVERTIBLE = 2;
int CT_ERR_OUT_OF_MEMORY = 3;

int ct_cartesian_to_gnomonic(struct CartesianPointSources *cartesian, struct CartesianPointSource *center, struct GnomonicPointSources *gnomonic) {
  /// Projects Cartesian coordinates onto a gnomonic tangent plane.
  /// The center point is the origin of the gnomonic projection.

  // First, change the basis of cartesian to be relative to the center
  // point. This requires building an orthogonal basis, using a vector
  // towards the center point as one of the basis vectors.

  // Check that the center point is not the origin.
  if (center->x == 0 && center->y == 0 && center->z == 0) {
    return CT_ERR_INVALID_CENTER;
  }

  // Normalize the center vector.
  double center_mag = sqrt(center->x * center->x + center->y * center->y + center->z * center->z);
  double center_vec[3] = {center->x / center_mag, center->y / center_mag, center->z / center_mag};

  // Cross the center vector with the x-axis to get the second basis vector.
  double x_axis[3] = {1, 0, 0};

  double second_basis_vec[3] = {
    center_vec[1] * x_axis[2] - center_vec[2] * x_axis[1],
    center_vec[2] * x_axis[0] - center_vec[0] * x_axis[2],
    center_vec[0] * x_axis[1] - center_vec[1] * x_axis[0]
  };

  // Cross the center vector with the second basis vector to get the third basis vector.
  double third_basis_vec[3] = {
    center_vec[1] * second_basis_vec[2] - center_vec[2] * second_basis_vec[1],
    center_vec[2] * second_basis_vec[0] - center_vec[0] * second_basis_vec[2],
    center_vec[0] * second_basis_vec[1] - center_vec[1] * second_basis_vec[0]
  };

  // Now, we can project the cartesian coordinates onto the new basis.
  // To do that, start by inverting the basis matrix.
  double basis_matrix[3][3] = {
    {second_basis_vec[0], third_basis_vec[0], center_vec[0]},
    {second_basis_vec[1], third_basis_vec[1], center_vec[1]},
    {second_basis_vec[2], third_basis_vec[2], center_vec[2]}
  };

  double basis_matrix_inv[3][3];
  int success = ct_matrix_invert_3x3(basis_matrix, basis_matrix_inv);
  if (success != 0) {
    return CT_ERR_NOT_INVERTIBLE;
  }

  // Now, we can project the cartesian coordinates onto the new basis
  // by multiplying by the inverse basis matrix.
  for (size_t i = 0; i < cartesian->x.length; i++) {
    double cartesian_vec[3] = {cartesian->x.data[i], cartesian->y.data[i], cartesian->z.data[i]};
    double new_basis_vec[3];
    ct_matrix_multiply_3x3_3x1(basis_matrix_inv, cartesian_vec, new_basis_vec);

    gnomonic->x.data[i] = new_basis_vec[0] / new_basis_vec[2];;
    gnomonic->y.data[i] = new_basis_vec[1] / new_basis_vec[2];
    gnomonic->t.data[i] = cartesian->t.data[i];
  }

  return 0;
}

// main: convert a few coords to test functionality
int main(int argc, char **argv) {
  struct CartesianPointSources cartesian;
  struct CartesianPointSource center;
  struct GnomonicPointSources gnomonic;

  int ok;
  if (ct_vec_f64_new(&cartesian.x, 3) != 0) {
    printf("out of memory\n");
    goto fail;
  }
  if (ct_vec_f64_new(&cartesian.y, 3) != 0) {
    printf("out of memory\n");
    goto fail;
  }
  if (ct_vec_f64_new(&cartesian.z, 3) != 0) {
    printf("out of memory\n");
    goto fail;
  }
  if (ct_vec_timestamp_mjd_new(&cartesian.t, 3) != 0) {
    printf("out of memory\n");
    goto fail;
  }

  if (ct_gnomonic_point_sources_new(&gnomonic, 3) != 0) {
    printf("out of memory\n");
    goto fail;
  }

  cartesian.x.data[0] = 0;
  cartesian.y.data[0] = 0;
  cartesian.z.data[0] = 1;
  cartesian.t.data[0] = 0;

  cartesian.x.data[1] = 0;
  cartesian.y.data[1] = 1;
  cartesian.z.data[1] = 0;
  cartesian.t.data[1] = 0;

  cartesian.x.data[2] = 1;
  cartesian.y.data[2] = 0;
  cartesian.z.data[2] = 0;
  cartesian.t.data[2] = 0;

  center.x = 3;
  center.y = 9;
  center.z = 3;
  center.t = 0;

  ok = ct_cartesian_to_gnomonic(&cartesian, &center, &gnomonic);
  if (ok != 0) {
    printf("error: %d\n", ok);
    goto fail;
  }

  printf("x: %f, y: %f, t: %f\n", gnomonic.x.data[0], gnomonic.y.data[0], gnomonic.t.data[0]);
  printf("x: %f, y: %f, t: %f\n", gnomonic.x.data[1], gnomonic.y.data[1], gnomonic.t.data[1]);
  printf("x: %f, y: %f, t: %f\n", gnomonic.x.data[2], gnomonic.y.data[2], gnomonic.t.data[2]);

  ct_gnomonic_point_sources_free(&gnomonic);
  ct_vec_f64_free(&cartesian.x);
  ct_vec_f64_free(&cartesian.y);
  ct_vec_f64_free(&cartesian.z);
  ct_vec_timestamp_mjd_free(&cartesian.t);

  return 0;

 fail:
  ct_gnomonic_point_sources_free(&gnomonic);
  ct_vec_f64_free(&cartesian.x);
  ct_vec_f64_free(&cartesian.y);
  ct_vec_f64_free(&cartesian.z);
  ct_vec_timestamp_mjd_free(&cartesian.t);
  return 1;
}
	
