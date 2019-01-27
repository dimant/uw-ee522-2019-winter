#include <time.h>
#include <stdlib.h>

double int_mul_benchmark(unsigned int sets, unsigned int iterations)
{
	double avg = 0;
	
	srand(time(NULL));
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

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start = gettime_now.tv_nsec;
			
		for(j = 0; j < iterations; j++)
		{
			left[j] = left[j] * right[j];
		}
	
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		avg += (gettime_now.tv_nsec - start)/iterations;
	}

	return avg / sets;
}

double int_div_benchmark(unsigned int sets, unsigned int iterations)
{
	double avg = 0;
	
	srand(time(NULL));
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

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start = gettime_now.tv_nsec;
			
		for(j = 0; j < iterations; j++)
		{
			left[j] = left[j] / right[j];
		}
	
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		avg += (gettime_now.tv_nsec - start)/iterations;
	}

	return avg / sets;
}

double float_mul_benchmark(unsigned int sets, unsigned int iterations)
{
	double avg = 0;
	
	srand(time(NULL));
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

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start = gettime_now.tv_nsec;
			
		for(j = 0; j < iterations; j++)
		{
			left[j] = left[j] * right[j];
		}
	
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		avg += (gettime_now.tv_nsec - start)/iterations;
	}

	return avg / sets;
}



double float_div_benchmark(unsigned int sets, unsigned int iterations)
{
	double avg = 0;
	
	srand(time(NULL));
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

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start = gettime_now.tv_nsec;
			
		for(j = 0; j < iterations; j++)
		{
			left[j] = left[j] / right[j];
		}
	
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		avg += (gettime_now.tv_nsec - start)/iterations;
	}

	return avg / sets;
}


