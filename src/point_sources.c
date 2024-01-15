#include "point_sources.h"

#include <stddef.h>
#include <stdlib.h>

#include "str.h"
#include "vectors.h"

int topocentric_point_sources_new(struct TopocentricPointSources *topocentric, size_t capacity,
                                  struct String *obscode) {
  /// Create a new TopocentricPointSources object with the given
  /// capacity. Takes ownership of obscode.
  if (vec_f64_new(&topocentric->ra, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&topocentric->dec, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&topocentric->t, capacity) != 0) {
    goto fail;
  }
  topocentric->obscode = obscode;
  return 0;

fail:
  topocentric_point_sources_free(topocentric);
  return -1;
}

void topocentric_point_sources_free(struct TopocentricPointSources *topocentric) {
  vec_f64_free(&topocentric->ra);
  vec_f64_free(&topocentric->dec);
  vec_f64_free(&topocentric->t);
  string_free(topocentric->obscode);
}

void topocentric_point_sources_push(struct TopocentricPointSources *topocentric, double ra, double dec, double t) {
  vec_f64_push(&topocentric->ra, ra);
  vec_f64_push(&topocentric->dec, dec);
  vec_f64_push(&topocentric->t, t);
}

int cartesian_point_sources_new(struct CartesianPointSources *cartesian, size_t capacity) {
  if (vec_f64_new(&cartesian->x, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&cartesian->y, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&cartesian->z, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&cartesian->t, capacity) != 0) {
    goto fail;
  }

  return 0;

fail:
  cartesian_point_sources_free(cartesian);
  return -1;
}

void cartesian_point_sources_free(struct CartesianPointSources *cartesian) {
  vec_f64_free(&cartesian->x);
  vec_f64_free(&cartesian->y);
  vec_f64_free(&cartesian->z);
  vec_f64_free(&cartesian->t);
}

void cartesian_point_sources_push(struct CartesianPointSources *cartesian, double x, double y, double z, double t) {
  vec_f64_push(&cartesian->x, x);
  vec_f64_push(&cartesian->y, y);
  vec_f64_push(&cartesian->z, z);
  vec_f64_push(&cartesian->t, t);
}

int gnomonic_point_sources_new(struct GnomonicPointSources *gnomonic, size_t capacity) {
  if (vec_f64_new(&gnomonic->x, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&gnomonic->y, capacity) != 0) {
    goto fail;
  }
  if (vec_f64_new(&gnomonic->t, capacity) != 0) {
    goto fail;
  }

  return 0;

fail:
  gnomonic_point_sources_free(gnomonic);
  return -1;
}

void gnomonic_point_sources_free(struct GnomonicPointSources *gnomonic) {
  vec_f64_free(&gnomonic->x);
  vec_f64_free(&gnomonic->y);
  vec_f64_free(&gnomonic->t);
}

void gnomonic_point_sources_push(struct GnomonicPointSources *gnomonic, double x, double y, double t) {
  vec_f64_push(&gnomonic->x, x);
  vec_f64_push(&gnomonic->y, y);
  vec_f64_push(&gnomonic->t, t);
}
