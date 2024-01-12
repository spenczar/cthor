#include <stdio.h>
#include <stdlib.h>
#include "unittests.h"
#include "projections.h"
#include "point_sources.h"

int tests_run = 0;

static char* test_cartesian_to_gnomonic_projection() {

  // Set up some Cartesian coordinates:
  struct CartesianPointSources cartesian;
  int status = cartesian_point_sources_new(&cartesian, 10);
  ut_assert(status == 0, "cartesian_point_sources_new failed");

  cartesian_point_sources_push(&cartesian, 1.0, 2.0, 3.0, 4.0);
  cartesian_point_sources_push(&cartesian, 5.0, 6.0, 7.0, 8.0);
  cartesian_point_sources_push(&cartesian, 9.0, 10.0, 11.0, 12.0);

  // Pick a center
  double center[3] = {5.0, 6.0, 7.0};

  // Project
  struct GnomonicPointSources gnomonic;
  status = gnomonic_point_sources_new(&gnomonic, cartesian.x.length);
  status = cartesian_to_gnomonic(&cartesian, center, &gnomonic);
  ut_assert(status == 0, "cartesian_to_gnomonic failed");

  // Check the results
  // TODO: Do these by hand, and check that the results are correct.
  // TODO: Better unit test assertions for floating point numbers.
  ut_assert(gnomonic.x.length == 3, "wrong length for x");
  ut_assert(gnomonic.x.data[0] == -0.13622291021671836808, "wrong value for x[0]");
  ut_assert(gnomonic.x.data[1] == -8.46844895759339858735765083616e-17, "wrong value for x[1]");
  ut_assert(gnomonic.x.data[2] == 0.02844214608920491230, "wrong value for x[2]");
  ut_assert(gnomonic.y.length == 3, "wrong length for y");
  ut_assert(gnomonic.y.data[0] == 0.25976689737960406701, "wrong value for y[0]");
  ut_assert(gnomonic.y.data[1] == 0.0, "wrong value for y[1]");
  ut_assert(gnomonic.y.data[2] == -0.05423704450782939473, "wrong value for y[2]");

  // Times should be unchanged:
  ut_assert(gnomonic.t.length == 3, "wrong length for t");
  ut_assert(gnomonic.t.data[0] == cartesian.t.data[0], "wrong value for t[0]");
  ut_assert(gnomonic.t.data[1] == cartesian.t.data[1], "wrong value for t[1]");
  ut_assert(gnomonic.t.data[2] == cartesian.t.data[2], "wrong value for t[2]");

  // Clean up
  gnomonic_point_sources_free(&gnomonic);
  cartesian_point_sources_free(&cartesian);
  
  return 0;
}

static char * all_tests() {
  ut_run_test(test_cartesian_to_gnomonic_projection);
  return 0;
}

int main() {
  char *result = all_tests();
  if (result != 0) {
    printf("FAILURE: %s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
  
}
