#ifndef MY_CLOCK_H
#define MY_CLOCK_H

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

int my_clock_gettime(int clockid, struct timespec *tp);

#endif