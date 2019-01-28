#ifndef MEM_BENCHMARK_H
#define MEM_BENCHMARK_H

#include <stdlib.h>

void mem_benchmark();
unsigned long* byte_copy_benchmark(long iterations);
unsigned long* hword_copy_benchmark(long iterations);
unsigned long* word_copy_benchmark(long iterations);

#endif
