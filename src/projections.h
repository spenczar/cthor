#ifndef projections_h
#define projections_h

#include "point_sources.h"

/// Computes a vector normal to a plane defined by a vector to a position and
/// a velocity vector.
///
/// The resulting vector has a magnitude of 1.0.
///
/// Returns 0 on success, or an error code on failure.
int normal_vector(double center_pos[3], double center_velocity[3], double normal[3]);

/// Builds an orthonormal basis from a radial position vector and a velocity vector.
int build_orthonormal_basis(double center_pos[3], double center_velocity[3], double basis[3][3]);

/// Project a set of Cartesian point sources onto a gnomonic plane.
///
/// The gnomonic plane is defined by a center point and a velocity
/// vector.
///
/// The center point is the origin of the gnomonic projection. The
/// center point is in (x, y, z) coordinates, in the same frame as the
/// Cartesian coordinates.
///
/// The velocity's magnitude does not matter; only its direction
/// matters, since the direction defines the plane's x and y axes.
///
/// The result is written to the gnomonic argument, which must be
/// initialized by the caller, and must have zero length.
///
/// Returns 0 on success, or an error code on failure.
int cartesian_to_gnomonic(struct CartesianPointSources *cartesian, double center[3], double center_velocity[3], struct GnomonicPointSources *gnomonic);

#endif
