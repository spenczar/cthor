#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "matrixmath.h"
#include "vectors.h"

typedef double TimestampMJD;

struct String {
  size_t length;
  char *data;
};

struct TopocentricPointSources {
  struct VecF64 ra;
  struct VecF64 dec;
  struct VecF64 t; // MJD
  struct String *obscode;
};

struct CartesianPointSources {
  // Always heliocentric
  struct VecF64 x;
  struct VecF64 y;
  struct VecF64 z;
  struct VecF64 t;  // MJD
};

struct CartesianPointSource {
  /// A single point source in Cartesian coordinates.
  /// Always heliocentric
  double x;
  double y;
  double z;
  double t;
};

struct GnomonicPointSources {
  // Always heliocentric. Relative to some center point.
  struct VecF64 x;
  struct VecF64 y;
  struct VecF64 t;
};

void gnomonic_point_sources_free(struct GnomonicPointSources *gnomonic) {
  vec_f64_free(&gnomonic->x);
  vec_f64_free(&gnomonic->y);
  vec_f64_free(&gnomonic->t);
}

int gnomonic_point_sources_new(struct GnomonicPointSources *gnomonic, size_t length) {
  if (vec_f64_new(&gnomonic->x, length) != 0) {
    goto fail;
  }
  if (vec_f64_new(&gnomonic->y, length) != 0) {
    goto fail;
  }
  if (vec_f64_new(&gnomonic->t, length) != 0) {
    goto fail;
  }

  return 0;

 fail:
  gnomonic_point_sources_free(gnomonic);
  return -1;
}

int CT_ERR_INVALID_CENTER = 1;
int CT_ERR_NOT_INVERTIBLE = 2;
int CT_ERR_OUT_OF_MEMORY = 3;

int cartesian_to_gnomonic(struct CartesianPointSources *cartesian, struct CartesianPointSource *center, struct GnomonicPointSources *gnomonic) {
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
  int success = matinv_3x3(basis_matrix, basis_matrix_inv);
  if (success != 0) {
    return CT_ERR_NOT_INVERTIBLE;
  }

  // Now, we can project the cartesian coordinates onto the new basis
  // by multiplying by the inverse basis matrix.
  for (size_t i = 0; i < cartesian->x.length; i++) {
    double cartesian_vec[3] = {cartesian->x.data[i], cartesian->y.data[i], cartesian->z.data[i]};
    double new_basis_vec[3];
    matmul_3x3_3x1(basis_matrix_inv, cartesian_vec, new_basis_vec);

    gnomonic->x.data[i] = new_basis_vec[0] / new_basis_vec[2];;
    gnomonic->y.data[i] = new_basis_vec[1] / new_basis_vec[2];
    gnomonic->t.data[i] = cartesian->t.data[i];
  }

  return 0;
}

