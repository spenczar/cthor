#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


#include "matrixmath.h"
#include "vectors.h"
#include "point_sources.h"

int CT_ERR_INVALID_CENTER = 1;
int CT_ERR_NOT_INVERTIBLE = 2;
int CT_ERR_OUT_OF_MEMORY = 3;

int cartesian_to_gnomonic(struct CartesianPointSources *cartesian, double center[3], struct GnomonicPointSources *gnomonic) {
  /// Projects Cartesian coordinates onto a gnomonic tangent plane.
  /// The center point is the origin of the gnomonic projection.

  // First, change the basis of cartesian to be relative to the center
  // point. This requires building an orthogonal basis, using a vector
  // towards the center point as one of the basis vectors.

  // Check that the center point is not the origin.
  if (center[0] == 0 && center[1] == 0 && center[2] == 0) {
    return CT_ERR_INVALID_CENTER;
  }

  // Normalize the center vector.
  double center_mag = sqrt(center[0] * center[0] + center[1] * center[1] + center[2] * center[2]);
  double center_vec[3] = {center[0] / center_mag, center[1] / center_mag, center[2] / center_mag};

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

