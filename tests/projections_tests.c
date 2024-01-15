#include <stdio.h>
#include <stdlib.h>

#include "point_sources.h"
#include "projections.h"
#include "unittests.h"

int tests_run = 0;

static char* test_cartesian_to_gnomonic_projection(void) {
  // Set up some Cartesian coordinates - a few point source detections by W84.
  // These were run through the Python THOR
  struct CartesianPointSources cartesian;
  int status = cartesian_point_sources_new(&cartesian, 10);
  ut_assert(status == 0, "cartesian_point_sources_new failed");

  cartesian_point_sources_push(&cartesian, 2.32724566583692, -0.449382385055792, 0.0866176471970003, 56537.2416032334);
  cartesian_point_sources_push(&cartesian, 2.32728038367672, -0.44938629368127, 0.0856592596632065, 56537.2416032334);
  cartesian_point_sources_push(&cartesian, 2.32729293752894, -0.449243941523289, 0.0860639174895683, 56537.2416032334);

  // Position of 2013 RR165 at MJD=56537.2416032334:
  double center[3] = {2.32545784897911, -0.459940068868785, 0.0788698905258432};
  double center_velocity[3] = {0.00257146073153728, 0.011315544836752, 0.00041171196985311};

  // Project
  struct GnomonicPointSources gnomonic;
  status = gnomonic_point_sources_new(&gnomonic, cartesian.x.length);
  status = cartesian_to_gnomonic(&cartesian, center, center_velocity, &gnomonic);
  ut_assert(status == 0, "cartesian_to_gnomonic failed");

  // Check the results
  ut_assert(gnomonic.x.length == 3, "wrong length for x");
  ut_assert_feq(gnomonic.x.data[0], 0.26488865499153);
  ut_assert_feq(gnomonic.x.data[1], 0.264158742296632);
  ut_assert_feq(gnomonic.x.data[2], 0.267926881709716);
  ut_assert(gnomonic.y.length == 3, "wrong length for y");
  ut_assert_feq(gnomonic.y.data[0], 0.178261157983677);
  ut_assert_feq(gnomonic.y.data[1], 0.155105149861758);
  ut_assert_feq(gnomonic.y.data[2], 0.16476328675463);

  // Times should be unchanged:
  ut_assert(gnomonic.t.length == 3, "wrong length for t");
  ut_assert_feq(gnomonic.t.data[0], cartesian.t.data[0]);
  ut_assert_feq(gnomonic.t.data[1], cartesian.t.data[1]);
  ut_assert_feq(gnomonic.t.data[2], cartesian.t.data[2]);

  // Clean up
  gnomonic_point_sources_free(&gnomonic);
  cartesian_point_sources_free(&cartesian);

  return 0;
}

static char* all_tests() {
  ut_run_test(test_cartesian_to_gnomonic_projection);
  return 0;
}

int main(void) {
  char* result = all_tests();
  if (result != 0) {
    printf("FAILURE: %s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
