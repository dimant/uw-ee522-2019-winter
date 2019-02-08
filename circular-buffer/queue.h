#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>

typedef struct queues_t
{
    //private
    float* _queue;
    int32_t _queue_size;
    int32_t _queue_in;
    int32_t _queue_out;
    int32_t _queue_remaining;
} queue_t;

#endif