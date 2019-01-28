#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "mem-benchmark.h"
#include "my-clock.h"

typedef uint8_t  byte;
typedef uint16_t hword;
typedef uint32_t word;

#define KBYTE 1024
#define MBYTE 1048576
#define GBYTE 1073741824

#define BYTE_MAX 256
#define HWORD_MAX 65536
#define WORD_MAX 4294967295

#define SIZES 3
const size_t sizes[SIZES] = { KBYTE, MBYTE, GBYTE };

void mem_benchmark()
{
    int i, cur_size;

    long iterations = 10000;

    for (i = 0; i < 5; i++)
    {
        double* byte_copy_result = byte_copy_benchmark(iterations);
        double* hword_copy_result = hword_copy_benchmark(iterations);
        double* word_copy_result = word_copy_benchmark(iterations);

        for (cur_size = 0; cur_size < SIZES; cur_size++)
        {
            printf("Copying %i bytes of data in bytes: %fns hwords: %fns words: %fns", 
                sizes[cur_size],
                byte_copy_result[cur_size],
                hword_copy_result[cur_size],
                word_copy_result[cur_size]);
        }
    }
}

double* byte_copy_benchmark(long iterations)
{
    double* result = (double*) malloc(SIZES * sizeof(double));
    byte* left;
    byte* right;

    int cur_size, cur_iteration, cur_operation;

    long start;
    struct timespec gettime_now;

    srand((unsigned int)time(NULL));

    double avg = 0.0;

    for (cur_size = 0; cur_size < SIZES; cur_size++)
    {
        left = (byte*)malloc(sizes[cur_size]);
        right = (byte*)malloc(sizes[cur_size]);
        size_t operations = sizes[cur_size] / sizeof(byte);

        for (cur_iteration = 0; cur_iteration < iterations; cur_iteration++)
        {
            for (cur_operation = 0; cur_operation < operations; cur_operation++)
            {
                left[cur_operation] = (byte)rand() % BYTE_MAX;
                right[cur_operation] = (byte)rand() % BYTE_MAX;
            }

            my_clock_gettime(CLOCK_REALTIME, &gettime_now);
            start = gettime_now.tv_nsec;

            for (cur_operation = 0; cur_operation < operations; cur_operation++)
            {
                left[cur_operation] = right[cur_operation];
            }

            my_clock_gettime(CLOCK_REALTIME, &gettime_now);
            avg += (double) (gettime_now.tv_nsec - start) / (double)operations;
        }

        avg /= (double)iterations;
        result[cur_size] = avg;
    }

    return result;
}

double* hword_copy_benchmark(long iterations)
{
    double* result = (double*)malloc(SIZES * sizeof(double));

    hword* left;
    hword* right;

    int cur_size, cur_iteration, cur_operation;

    long start;
    struct timespec gettime_now;

    srand((unsigned int)time(NULL));

    double avg = 0.0;

    for (cur_size = 0; cur_size < SIZES; cur_size++)
    {
        left = (hword*)malloc(sizes[cur_size]);
        right = (hword*)malloc(sizes[cur_size]);
        size_t operations = sizes[cur_size] / sizeof(hword);

        for (cur_iteration = 0; cur_iteration < iterations; cur_iteration++)
        {
            for (cur_operation = 0; cur_operation < operations; cur_operation++)
            {
                left[cur_operation] = (hword)rand() % HWORD_MAX;
                right[cur_operation] = (hword)rand() % HWORD_MAX;
            }

            my_clock_gettime(CLOCK_REALTIME, &gettime_now);
            start = gettime_now.tv_nsec;

            for (cur_operation = 0; cur_operation < operations; cur_operation++)
            {
                left[cur_operation] = right[cur_operation];
            }

            my_clock_gettime(CLOCK_REALTIME, &gettime_now);
            avg += (double)(gettime_now.tv_nsec - start) / (double)operations;
        }
        avg /= (double)iterations;
        result[cur_size] = avg;
    }

    return result;
}

double* word_copy_benchmark(long iterations)
{
    double* result = (double*)malloc(SIZES * sizeof(double));

    word* left;
    word* right;

    size_t cur_size, cur_iteration, cur_operation;

    long start;
    struct timespec gettime_now;

    srand((unsigned int)time(NULL));

    double avg = 0.0;

    for (cur_size = 0; cur_size < SIZES; cur_size++)
    {
        left = (word*)malloc(sizes[cur_size]);
        right = (word*)malloc(sizes[cur_size]);
        size_t operations = sizes[cur_size] / sizeof(word);

        for (cur_iteration = 0; cur_iteration < iterations; cur_iteration++)
        {
            for (cur_operation = 0; cur_operation < operations; cur_operation++)
            {
                left[cur_operation] = (word) WORD_MAX * ((unsigned int) rand() % RAND_MAX / RAND_MAX);
                right[cur_operation] = (word) WORD_MAX * ((unsigned int) rand() % RAND_MAX / RAND_MAX);
            }

            my_clock_gettime(CLOCK_REALTIME, &gettime_now);
            start = gettime_now.tv_nsec;

            for (cur_operation = 0; cur_operation < operations; cur_operation++)
            {
                left[cur_operation] = right[cur_operation];
            }

            my_clock_gettime(CLOCK_REALTIME, &gettime_now);
            avg += (double)(gettime_now.tv_nsec - start) / (double)operations;
        }
        avg /= (double)iterations;
        result[cur_size] = avg;
    }

    return result;
}