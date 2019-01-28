#include <time.h>
#include "my-clock.h"

int my_clock_gettime(int clockid, struct timespec *tp)
{
    int result = 0;

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
    result = clock_gettime(clockid, tp);
#pragma GCC diagnostic warning "-Wimplicit-function-declaration"

    return result;
}