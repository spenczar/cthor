#ifndef unittests_h
#define unittests_h
#include <stdio.h>
#include <math.h>

char message[2048];

#define FLOAT_EPSILON 1E-10

#define ut_assert(cond, message) do { if (!(cond)) return message; } while (0)
#define ut_assert_str_eq(a, b) do { if ((a) != (b)) { sprintf(message, "%s != %s", a, b); return message; } } while (0)
#define ut_assert_feq(a, b) do { if (fabs((a) - (b)) > FLOAT_EPSILON) { sprintf(message, "%s != %s: %f != %f", #a, #b, a, b); return message; } } while (0)
#define ut_assert_ieq(a, b) do { if ((a) != (b)) { sprintf(message, "%d != %d", a, b); return message; } } while (0)

#define ut_run_test(test) do { printf("running %s\n", #test); char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

#endif
