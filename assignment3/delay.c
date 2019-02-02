// for nanosleep
#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include "delay.h"

void delay(uint32_t microseconds)
{
    struct timespec sleeper;
    long int uSecs = (long int) microseconds % 1000000;
    long int wSecs = (long int) microseconds / 1000000;

    sleeper.tv_sec  = wSecs;
    sleeper.tv_nsec = uSecs * 1000L;
    nanosleep(&sleeper, NULL);
}
