#include <sys/time.h>
#include <stdlib.h>
#include "my-clock.h"

#include "assert-macros.h"

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

uint64_t my_clock_gettime()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    uint64_t milliseconds = (uint64_t) (te.tv_sec * 1000LL + te.tv_usec / 1000);
    return milliseconds;
}
