#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

typedef struct queues_t
{
    //private
    float* _queue;
    uint32_t _size;
    uint32_t _in;
    uint32_t _out;
    uint32_t _remaining;
} queue_t;

void queue_create(queue_t* queue, uint32_t size);
void queue_delete(queue_t* queue);
uint32_t queue_put(queue_t* queue, float* input, uint32_t n);
uint32_t queue_get(queue_t* queue, float* output, uint32_t n);

#endif
