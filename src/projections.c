#include "projections.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#include "matrixmath.h"

#define FLOAT_EPSILON 1E-10
#define _SQRT_TWO 1.41421356237309504880168872420969807856967187537694807317667973799

int CT_ERR_INVALID_CENTER = 1;
int CT_ERR_NOT_INVERTIBLE = 2;
int CT_ERR_OUT_OF_MEMORY = 3;

static double rad_to_deg(double rad);

static double identity_matrix[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

int normal_vector(double center_pos[3], double center_velocity[3], double normal[3]) {
  // Check that the center point is not the origin.
  if (center_pos[0] == 0 && center_pos[1] == 0 && center_pos[2] == 0) {
    return CT_ERR_INVALID_CENTER;
  }

  // If the velocity is nearly parallel to r, or v's magnitude is
  // nearly zero, then improvise a new velocity parallel to the x-y
  // plane.
  double center_velocity_corrected[3];
  double r_dot_v = dot(center_pos, center_velocity);
  if (fabs(r_dot_v) > 1 - FLOAT_EPSILON || magnitude(center_velocity) < FLOAT_EPSILON) {
    center_velocity_corrected[0] = _SQRT_TWO;
    center_velocity_corrected[1] = _SQRT_TWO;
    center_velocity_corrected[2] = 0.0;
  } else {
    memcpy(center_velocity_corrected, center_velocity, sizeof(double) * 3);
  }

  cross(center_pos, center_velocity_corrected, normal);
  if (normalize(normal) != 0) {
    return CT_ERR_INVALID_CENTER;
  }
  return 0;
}

static int r1_matrix(double center_pos[3], double center_velocity[3], double r1[3][3]) {
  // Compute a vector normal to the orbital plane of the center object
  double n_hat[3];
  if (normal_vector(center_pos, center_velocity, n_hat) != 0) {
    return -1;
  }

  // Find the rotation axis, nu.
  double nu[3];
  double z_axis[3] = {0.0, 0.0, 1.0};
  cross(n_hat, z_axis, nu);

  // If the rotation axis is parallel to the z-axis, then the rotation
  // angle is 0, and the rotation matrix is the identity matrix.
  if (magnitude(nu) < FLOAT_EPSILON) {
    memcpy(r1, identity_matrix, sizeof(double) * 3 * 3);
    return 0;
  }

  // Find the cosine of the rotation angle, theta
  //
  // Since n_hat and z_axis are unit vectors, their dot product is the
  // cosine of the angle between them.
  double cos_theta = dot(n_hat, z_axis);

  // Compute the skew-symmetric matrix, V.
  double V[3][3] = {{0.0, -nu[2], nu[1]}, {nu[2], 0.0, -nu[0]}, {-nu[1], nu[0], 0.0}};

  double V_squared[3][3];
  matsquare_3x3(V, V_squared);

  matscale(V_squared, (1 / (1 + cos_theta)));

  // Compute the rotation matrix, r1.
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      r1[i][j] = identity_matrix[i][j] + V[i][j] + V_squared[i][j];
    }
  }

  return 0;
}

static int r2_matrix(double r1[3][3], double center_pos[3], double r2[3][3]) {
  double r_rotated[3];
  matmul_3x3_3x1(r1, center_pos, r_rotated);
  if (normalize(r_rotated) != 0) {
    return -1;
  }

  r2[0][0] = r_rotated[0];
  r2[0][1] = r_rotated[1];
  r2[0][2] = 0.0;
  r2[1][0] = -r_rotated[1];
  r2[1][1] = r_rotated[0];
  r2[1][2] = 0.0;
  r2[2][0] = 0.0;
  r2[2][1] = 0.0;
  r2[2][2] = 1.0;

  return 0;
}

static int gnomonic_rotation_matrix(double center_pos[3], double center_velocity[3], double r_gnomonic[3][3]) {
  double r1[3][3];
  int status = r1_matrix(center_pos, center_velocity, r1);
  if (status != 0) {
    return status;
  }

  double r2[3][3];
  status = r2_matrix(r1, center_pos, r2);
  if (status != 0) {
    return status;
  }

  matmul_3x3_3x3(r2, r1, r_gnomonic);
  return 0;
}

int cartesian_to_gnomonic(struct CartesianPointSources *cartesian, double center_pos[3], double center_velocity[3],
                          struct GnomonicPointSources *gnomonic) {
  // Check that the gnomonic point sources are empty.
  assert(gnomonic->x.length == 0);

  double rotation_matrix[3][3];
  int status = gnomonic_rotation_matrix(center_pos, center_velocity, rotation_matrix);
  if (status != 0) {
    return status;
  }
  for (size_t i = 0; i < cartesian->x.length; i++) {
    double cartesian_vec[3] = {cartesian->x.data[i], cartesian->y.data[i], cartesian->z.data[i]};
    double rotated_vec[3];
    matmul_3x3_3x1(rotation_matrix, cartesian_vec, rotated_vec);
    gnomonic_point_sources_push(gnomonic, rad_to_deg(rotated_vec[1] / rotated_vec[0]),
                                rad_to_deg(rotated_vec[2] / rotated_vec[0]), cartesian->t.data[i]);
  }

  return 0;
}

static double rad_to_deg(double rad) { return rad * 180.0 / M_PI; }
