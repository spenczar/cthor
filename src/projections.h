#ifndef projections_h
#define projections_h

#include "point_sources.h"

/// Project Cartesian coordinates onto a gnomonic tangent plane.
///
/// The center point is the origin of the gnomonic projection. The
/// center point is in (x, y, z) coordinates, in the same frame as the
/// Cartesian coordinates.
int cartesian_to_gnomonic(struct CartesianPointSources *cartesian, double center[3], struct GnomonicPointSources *gnomonic);

#endif
