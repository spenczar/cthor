#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "projections.h"
#include "point_sources.h"

static double center_position[3] = {0.9, 0.8, 0.01};
static double center_velocity[3] = {-0.05, 0.05, 0.00001};

#define TIME 1.0
#define N_POINTS 100000
#define N_RUNS 100

double rand_double(void) {
  // Generate a random double in the range [0, 1).
  return (double)rand() / RAND_MAX;
}

double rand_near(double x) {
  // Generate a random double in the range [x - 0.1, x + 0.1).
  return x + (rand_double() - 0.5) / 5.0;
}
void generate_point_sources(struct CartesianPointSources *cartesian, size_t n) {
  cartesian_point_sources_new(cartesian, n);
  for (size_t i = 0; i < n; i++) {
    double x = rand_near(center_position[0]);
    double y = rand_near(center_position[1]);
    double z = rand_near(center_position[2]);
    double t = TIME;
    cartesian_point_sources_push(cartesian, x, y, z, t);
  }
}

double run(struct CartesianPointSources *cartesian, struct GnomonicPointSources *gnomonic) {
  clock_t start = clock();
  cartesian_to_gnomonic(cartesian, center_position, center_velocity, gnomonic);
  clock_t end = clock();
  return (double)(end - start) / CLOCKS_PER_SEC;
}

double mean(double *xs, size_t n) {
  double sum = 0.0;
  for (size_t i = 0; i < n; i++) {
    sum += xs[i];
  }
  return sum / n;
}

int compare_doubles(const void *a, const void *b) {
  double x = *(double *)a;
  double y = *(double *)b;
  if (x < y) {
    return -1;
  } else if (x > y) {
    return 1;
  } else {
    return 0;
  }
}

double median(double *xs, size_t n) {
  double *sorted = malloc(n * sizeof(double));
  for (size_t i = 0; i < n; i++) {
    sorted[i] = xs[i];
  }
  qsort(sorted, n, sizeof(double), compare_doubles);
  double result = sorted[n / 2];
  free(sorted);
  return result;
}

int main(void) {
  struct CartesianPointSources cartesian = CARTESIAN_POINT_SOURCES_ZERO;
  struct GnomonicPointSources gnomonic = GNOMONIC_POINT_SOURCES_ZERO;
  gnomonic_point_sources_new(&gnomonic, N_POINTS);
  generate_point_sources(&cartesian, N_POINTS);

  double runs[N_RUNS];
  for (size_t i = 0; i < N_RUNS; i++) {
    double seconds = run(&cartesian, &gnomonic);
    double milliseconds = seconds * 1000.0;
    runs[i] = milliseconds;
    gnomonic_point_sources_free(&gnomonic);
    gnomonic_point_sources_new(&gnomonic, N_POINTS);
  }
  cartesian_point_sources_free(&cartesian);
  gnomonic_point_sources_free(&gnomonic);
  printf("Mean: %.6fms\n", mean(runs, N_RUNS));
  printf("Median: %.6fms\n", median(runs, N_RUNS));
}
