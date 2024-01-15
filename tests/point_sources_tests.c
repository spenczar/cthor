#include <stdio.h>

#include "point_sources.h"
#include "unittests.h"

int tests_run = 0;

static char* test_topocentric_point_sources() {
  struct TopocentricPointSources topocentric;
  struct String obscode = string_create("500");

  int status = topocentric_point_sources_new(&topocentric, 10, &obscode);
  ut_assert(status == 0, "topocentric_point_sources_new failed");

  topocentric_point_sources_push(&topocentric, 1.0, 2.0, 3.0);
  topocentric_point_sources_push(&topocentric, 4.0, 5.0, 6.0);
  topocentric_point_sources_push(&topocentric, 7.0, 8.0, 9.0);

  ut_assert(topocentric.ra.length == 3, "wrong length for ra");
  ut_assert(topocentric.ra.data[0] == 1.0, "wrong value for ra[0]");
  ut_assert(topocentric.ra.data[1] == 4.0, "wrong value for ra[1]");
  ut_assert(topocentric.ra.data[2] == 7.0, "wrong value for ra[2]");

  ut_assert(topocentric.dec.length == 3, "wrong length for dec");
  ut_assert(topocentric.dec.data[0] == 2.0, "wrong value for dec[0]");
  ut_assert(topocentric.dec.data[1] == 5.0, "wrong value for dec[1]");
  ut_assert(topocentric.dec.data[2] == 8.0, "wrong value for dec[2]");

  ut_assert(topocentric.t.length == 3, "wrong length for t");
  ut_assert(topocentric.t.data[0] == 3.0, "wrong value for t[0]");
  ut_assert(topocentric.t.data[1] == 6.0, "wrong value for t[1]");
  ut_assert(topocentric.t.data[2] == 9.0, "wrong value for t[2]");

  topocentric_point_sources_free(&topocentric);

  return 0;
}

static char* test_cartesian_point_sources() {
  struct CartesianPointSources cartesian;

  int status = cartesian_point_sources_new(&cartesian, 10);
  ut_assert(status == 0, "cartesian_point_sources_new failed");

  // Push some values
  cartesian_point_sources_push(&cartesian, 1.0, 2.0, 3.0, 4.0);
  cartesian_point_sources_push(&cartesian, 5.0, 6.0, 7.0, 8.0);
  cartesian_point_sources_push(&cartesian, 9.0, 10.0, 11.0, 12.0);

  ut_assert(cartesian.x.length == 3, "wrong length for x");
  ut_assert(cartesian.x.data[0] == 1.0, "wrong value for x[0]");
  ut_assert(cartesian.x.data[1] == 5.0, "wrong value for x[1]");
  ut_assert(cartesian.x.data[2] == 9.0, "wrong value for x[2]");
  ut_assert(cartesian.y.length == 3, "wrong length for y");
  ut_assert(cartesian.y.data[0] == 2.0, "wrong value for y[0]");
  ut_assert(cartesian.y.data[1] == 6.0, "wrong value for y[1]");
  ut_assert(cartesian.y.data[2] == 10.0, "wrong value for y[2]");
  ut_assert(cartesian.z.length == 3, "wrong length for z");
  ut_assert(cartesian.z.data[0] == 3.0, "wrong value for z[0]");
  ut_assert(cartesian.z.data[1] == 7.0, "wrong value for z[1]");
  ut_assert(cartesian.z.data[2] == 11.0, "wrong value for z[2]");
  ut_assert(cartesian.t.length == 3, "wrong length for t");
  ut_assert(cartesian.t.data[0] == 4.0, "wrong value for t[0]");
  ut_assert(cartesian.t.data[1] == 8.0, "wrong value for t[1]");
  ut_assert(cartesian.t.data[2] == 12.0, "wrong value for t[2]");

  cartesian_point_sources_free(&cartesian);
  return 0;
}

static char* test_gnomonic_point_sources() {
  struct GnomonicPointSources gnomonic;

  int status = gnomonic_point_sources_new(&gnomonic, 10);
  ut_assert(status == 0, "gnomonic_point_sources_new failed");

  // Push some values
  gnomonic_point_sources_push(&gnomonic, 1.0, 2.0, 3.0);
  gnomonic_point_sources_push(&gnomonic, 4.0, 5.0, 6.0);
  gnomonic_point_sources_push(&gnomonic, 7.0, 8.0, 9.0);

  ut_assert(gnomonic.x.length == 3, "wrong length for x");
  ut_assert(gnomonic.x.data[0] == 1.0, "wrong value for x[0]");
  ut_assert(gnomonic.x.data[1] == 4.0, "wrong value for x[1]");
  ut_assert(gnomonic.x.data[2] == 7.0, "wrong value for x[2]");
  ut_assert(gnomonic.y.length == 3, "wrong length for y");
  ut_assert(gnomonic.y.data[0] == 2.0, "wrong value for y[0]");
  ut_assert(gnomonic.y.data[1] == 5.0, "wrong value for y[1]");
  ut_assert(gnomonic.y.data[2] == 8.0, "wrong value for y[2]");
  ut_assert(gnomonic.t.length == 3, "wrong length for t");
  ut_assert(gnomonic.t.data[0] == 3.0, "wrong value for t[0]");
  ut_assert(gnomonic.t.data[1] == 6.0, "wrong value for t[1]");
  ut_assert(gnomonic.t.data[2] == 9.0, "wrong value for t[2]");

  gnomonic_point_sources_free(&gnomonic);
  return 0;
}

static char* all_tests() {
  ut_run_test(test_topocentric_point_sources);
  ut_run_test(test_cartesian_point_sources);
  ut_run_test(test_gnomonic_point_sources);
  return 0;
}

int main() {
  char* result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
