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
// actually not a gbyte but 200mb
// because you can't really allocate 2 1gb regions without a segfault
#define GBYTE 209715200

#define BYTE_MAX 256
#define HWORD_MAX 65536
#define WORD_MAX 4294967295

#define SIZES 3
const size_t sizes[SIZES] = { KBYTE, MBYTE, GBYTE };

void mem_benchmark()
{
    int i, cur_size;

    long iterations = 1;

    for (i = 0; i < 1; i++)
    {
        printf(".\n");
        unsigned long* byte_copy_result = byte_copy_benchmark(iterations);
        printf(".\n");
        unsigned long* hword_copy_result = hword_copy_benchmark(iterations);
        printf(".\n");
        unsigned long* word_copy_result = word_copy_benchmark(iterations);
        printf(".\n");

        for (cur_size = 0; cur_size < SIZES; cur_size++)
        {
            printf("Copying %i bytes. Total duration when copying bytes: %lfms hwords: %lfms words: %lfms\n", 
                sizes[cur_size],
                byte_copy_result[cur_size] / 1000.0,
                hword_copy_result[cur_size] / 1000.0,
                word_copy_result[cur_size] / 1000.0);

            free(byte_copy_result);
            free(hword_copy_result);
            free(word_copy_result);
        }
    }
}

unsigned long* byte_copy_benchmark(long iterations)
{
    unsigned long* result = (unsigned long*) malloc(SIZES * sizeof(unsigned long));
    byte* left;
    byte* right;

    int cur_size, cur_operation;

    long start;
    struct timespec gettime_now;

    srand((unsigned int)time(NULL));

    for (cur_size = 0; cur_size < SIZES; cur_size++)
    {
        left = (byte*)malloc(sizes[cur_size]);
        right = (byte*)malloc(sizes[cur_size]);
        size_t operations = sizes[cur_size] / sizeof(byte);

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
        result[cur_size] = (unsigned long) (gettime_now.tv_nsec / 1000 - start / 1000);

        free(left);
        free(right);
    }

    return result;
}

unsigned long* hword_copy_benchmark(long iterations)
{
    unsigned long* result = (unsigned long*)malloc(SIZES * sizeof(unsigned long));

    hword* left;
    hword* right;

    int cur_size, cur_operation;

    long start;
    struct timespec gettime_now;

    srand((unsigned int)time(NULL));

    for (cur_size = 0; cur_size < SIZES; cur_size++)
    {
        left = (hword*)malloc(sizes[cur_size]);
        right = (hword*)malloc(sizes[cur_size]);
        size_t operations = sizes[cur_size] / sizeof(hword);

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
        result[cur_size] = (unsigned long)(gettime_now.tv_nsec / 1000 - start / 1000);

        free(left);
        free(right);
    }

    return result;
}

unsigned long* word_copy_benchmark(long iterations)
{
    unsigned long* result = (unsigned long*)malloc(SIZES * sizeof(unsigned long));

    word* left;
    word* right;

    size_t cur_size, cur_operation;

    long start;
    struct timespec gettime_now;

    srand((unsigned int)time(NULL));

    for (cur_size = 0; cur_size < SIZES; cur_size++)
    {
        left = (word*)malloc(sizes[cur_size]);
        right = (word*)malloc(sizes[cur_size]);
        size_t operations = sizes[cur_size] / sizeof(word);

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
        result[cur_size] = (unsigned long)(gettime_now.tv_nsec / 1000 - start / 1000);

        free(left);
        free(right);
    }

    return result;
}
