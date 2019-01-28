#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "arith-benchmarks.h"
#include "my-clock.h"

void arith_benchmark()
{
    int i;

    unsigned int sets = 10000;
    long iterations = 10000;

    for (i = 0; i < 5; i++)
    {
        double int_mul_avg = int_mul_benchmark(sets, iterations);
        printf("integer multiplication sets: %i iterations: %i average: %fns\n", sets, iterations, int_mul_avg);

        double int_div_avg = int_div_benchmark(sets, iterations);
        printf("integer division sets: %i iterations: %i average: %fns\n", sets, iterations, int_div_avg);

        double float_mul_avg = float_mul_benchmark(sets, iterations);
        printf("float multiplication sets: %i iterations: %i average: %fns\n", sets, iterations, float_mul_avg);

        double float_div_avg = float_div_benchmark(sets, iterations);
        printf("float division sets: %i iterations: %i average: %fns\n", sets, iterations, float_div_avg);
    }
}

double int_mul_benchmark(unsigned int sets, long iterations)
{
    double avg = 0;
    
    srand((unsigned int) time(NULL));
    unsigned int i, j;

    int left[iterations];
    int right[iterations];

    long start;
    struct timespec gettime_now;

    for(i = 0; i < sets; i++)
    {
        for(j = 0; j < iterations; j++)
        {
            left[j] = rand();
            right[j] = rand();
        }

        my_clock_gettime(CLOCK_REALTIME, &gettime_now);

        start = gettime_now.tv_nsec;
            
        for(j = 0; j < iterations; j++)
        {
            left[j] = left[j] * right[j];
        }
    
        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        avg += (gettime_now.tv_nsec - start)/iterations;
    }

    return avg / sets;
}

double int_div_benchmark(unsigned int sets, long iterations)
{
    double avg = 0;
    
    srand((unsigned int)time(NULL));
    unsigned int i, j;

    int left[iterations];
    int right[iterations];

    long start;
    struct timespec gettime_now;

    for(i = 0; i < sets; i++)
    {
        for(j = 0; j < iterations; j++)
        {
            left[j] = rand();
            right[j] = rand();
        }

        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        start = gettime_now.tv_nsec;
            
        for(j = 0; j < iterations; j++)
        {
            left[j] = left[j] / right[j];
        }
    
        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        avg += (gettime_now.tv_nsec - start)/iterations;
    }

    return avg / sets;
}

double float_mul_benchmark(unsigned int sets, long iterations)
{
    double avg = 0;
    
    srand((unsigned int)time(NULL));
    unsigned int i, j;

    float left[iterations];
    float right[iterations];

    long start;
    struct timespec gettime_now;

    for(i = 0; i < sets; i++)
    {
        for(j = 0; j < iterations; j++)
        {
            left[j] = (float)rand()/(float)(RAND_MAX);
            right[j] = (float)rand()/(float)(RAND_MAX);
        }

        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        start = gettime_now.tv_nsec;
            
        for(j = 0; j < iterations; j++)
        {
            left[j] = left[j] * right[j];
        }
    
        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        avg += (gettime_now.tv_nsec - start)/iterations;
    }

    return avg / sets;
}

double float_div_benchmark(unsigned int sets, long iterations)
{
    double avg = 0;
    
    srand((unsigned int)time(NULL));
    unsigned int i, j;

    float left[iterations];
    float right[iterations];

    long start;
    struct timespec gettime_now;

    for(i = 0; i < sets; i++)
    {
        for(j = 0; j < iterations; j++)
        {
            left[j] = (float)rand()/(float)(RAND_MAX);
            right[j] = (float)rand()/(float)(RAND_MAX);
        }

        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        start = gettime_now.tv_nsec;
            
        for(j = 0; j < iterations; j++)
        {
            left[j] = left[j] / right[j];
        }
    
        my_clock_gettime(CLOCK_REALTIME, &gettime_now);
        avg += (gettime_now.tv_nsec - start)/iterations;
    }

    return avg / sets;
}


