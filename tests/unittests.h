#ifndef unittests_h
#define unittests_h
#include <stdio.h>
#include <math.h>

#define ut_assert(cond, message) do { if (!(cond)) return message; } while (0)
#define ut_run_test(test) do { printf("running %s\n", #test); char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

#endif
