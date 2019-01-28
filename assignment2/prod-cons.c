#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#include "prod-cons.h"

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

int meals_served;
int meals_made;

clock_t opened_at;

void* restaurant_clock(void* arg)
{
    while (1)
    {
        sleep(3);
        
        pthread_mutex_lock(&mutex);
        closed = TRUE;
        pthread_mutex_unlock(&mutex);
        printf("Restaurant is closed.\nMeals served: %i meals made: %i.\n", meals_served, meals_made);
        return NULL;
    }
}

void* waiter(void* arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (TRUE == closed)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        if (meals < 0)
        {
            pthread_cond_wait(&madeMeal, &mutex);
        }
        meals--;
        meals_served++;

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
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        if (meals > MAX_MEALS)
        {
            pthread_cond_wait(&servedMeal, &mutex);
        }
        meals++;
        meals_made++;

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&madeMeal);
    }
}

void restaurant()
{
    pthread_t chef_thread_id, waiter_thread_id, clock_thread_id;

    pthread_mutex_lock(&mutex);

    closed = FALSE;
    meals = 0;
    meals_served = 0;
    meals_made = 0;
    opened_at = clock();

    pthread_mutex_unlock(&mutex);

    pthread_create(&chef_thread_id, NULL, chef, NULL);
    pthread_create(&waiter_thread_id, NULL, waiter, NULL);
    pthread_create(&clock_thread_id, NULL, restaurant_clock, NULL);

    printf("Restaurant is open.\n");
    pthread_join(chef_thread_id, NULL);
    pthread_join(waiter_thread_id, NULL);
    pthread_join(clock_thread_id, NULL);
}
