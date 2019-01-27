#include <assert.h>
#include <stdio.h>

#include "export-pin.h"

#include "wiringpi-experiments.h"
#include "bcm2835-experiments.h"
#include "pigpio-experiments.h"
#include "memgpio-experiments.h"
#include "arith-benchmarks.h"

int main(void)
{
	int i;

	unsigned int sets = 10000;
	unsigned int iterations = 10000;

	for(i = 0; i < 5; i++)
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
