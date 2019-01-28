#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t servedMeal = PTHREAD_COND_INITIALIZER;
pthread_cond_t madeMeal = PTHREAD_COND_INITIALIZER;

#define MAX_MEALS 10

volatile int meals;

volatile short closed;

void my_usleep(unsigned int microseconds)
{
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
    usleep(microseconds);
#pragma GCC diagnostic warning "-Wimplicit-function-declaration"
}

void* waiter(void* arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (TRUE == closed)
        {
            return NULL;
        }

        if (meals < 0)
        {
            pthread_cond_wait(&madeMeal, &mutex);
        }
        meals--;
        printf("Served a meal %i meals on the counter.\n", meals);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&servedMeal);
    }
}

void* chef(void* arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        if (TRUE == closed)
        {
            return NULL;
        }

        if (meals > MAX_MEALS)
        {
            pthread_cond_wait(&servedMeal, &mutex);
        }
        meals++;
        printf("Made a meal. %i meals on the counter.\n", meals);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&madeMeal);
    }
}

void restaurant()
{
    pthread_t chef_thread_id, waiter_thread_id;

    closed = FALSE;
    meals = 0;

    pthread_create(&chef_thread_id, NULL, chef, NULL);
    pthread_create(&waiter_thread_id, NULL, chef, NULL);

    pthread_join(chef_thread_id, NULL);
    pthread_join(waiter_thread_id, NULL);

    // sleep for 10 seconds
    my_usleep(10 * 1000 * 1000);

    pthread_mutex_lock(&mutex);
    closed = TRUE;
    pthread_mutex_unlock(&mutex);
}