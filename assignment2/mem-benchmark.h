#ifndef MEM_BENCHMARK_H
#define MEM_BENCHMARK_H

#include <stdlib.h>

#define KBYTE 1024
#define MBYTE 1048576
#define GBYTE 1073741824

#define BYTE_MAX 256
#define HWORD_MAX 65536
#define WORD_MAX 4294967295

#define SIZES 3
const size_t sizes[SIZES] = { KBYTE, MBYTE, GBYTE };

void mem_benchmark();
double* byte_copy_benchmark(long iterations);
double* hword_copy_benchmark(long iterations);
double* word_copy_benchmark(long iterations);

#endif
