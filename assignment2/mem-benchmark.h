#ifndef MEM_BENCHMARK_H
#define MEM_BENCHMARK_H

#include <stdlib.h>

void mem_benchmark();
double* byte_copy_benchmark(long iterations);
double* hword_copy_benchmark(long iterations);
double* word_copy_benchmark(long iterations);

#endif
