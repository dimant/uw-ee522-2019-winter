#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>

typedef struct queues_t
{
    //private
    float* _queue;
    uint32_t _queue_size;
    uint32_t _queue_in;
    uint32_t _queue_out;
    uint32_t _queue_remaining;
} queue_t;

void queue_create(queue_t* queue, uint32_t size);
void queue_delete(queue_t* queue);
uint32_t queue_put(queue_t* queue, float* input, uint32_t n);
uint32_t queue_get(queue_t* queue, float* output, uint32_t n);

#endif
