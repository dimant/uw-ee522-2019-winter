#ifndef ARITH_BENCHMARKS_H
#define ARITH_BENCHMARKS_H

void arith_benchmark();
double int_mul_benchmark(unsigned int sets, long iterations);
double int_div_benchmark(unsigned int sets, long iterations);
double float_mul_benchmark(unsigned int sets, long iterations);
double float_div_benchmark(unsigned int sets, long iterations);

#endif
