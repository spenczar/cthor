#ifndef point_sources_h
#define point_sources_h

#include <stddef.h>
#include "vectors.h"
#include "str.h"

struct TopocentricPointSources {
  /// Represents a collection of point sources in the sky, relative to
  /// a given observatory. Uses a struct of arrays representation.
  struct VecF64 ra;
  struct VecF64 dec;
  struct VecF64 t; // MJD
  struct String *obscode;
};

int topocentric_point_sources_new(struct TopocentricPointSources *topocentric, size_t capacity, struct String *obscode);
void topocentric_point_sources_free(struct TopocentricPointSources *topocentric);
void topocentric_point_sources_push(struct TopocentricPointSources *topocentric, double ra, double dec, double t);

struct CartesianPointSources {
  /// Represents a collection of point sources in the sky, relative to
  /// the sun. 
  struct VecF64 x;
  struct VecF64 y;
  struct VecF64 z;
  struct VecF64 t;  // MJD
};

int cartesian_point_sources_new(struct CartesianPointSources *cartesian, size_t capacity);
void cartesian_point_sources_free(struct CartesianPointSources *cartesian);
void cartesian_point_sources_push(struct CartesianPointSources *cartesian, double x, double y, double z, double t);

struct GnomonicPointSources {
  /// Represents a collection of point sources, placed on a gnomonic
  /// projection.
  struct VecF64 x;
  struct VecF64 y;
  struct VecF64 t;
};

int gnomonic_point_sources_new(struct GnomonicPointSources *gnomonic, size_t capacity);
void gnomonic_point_sources_free(struct GnomonicPointSources *gnomonic);
void gnomonic_point_sources_push(struct GnomonicPointSources *gnomonic, double x, double y, double t);


#endif
