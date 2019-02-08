#include <stdlib.h>
#include <stdint.h>
#include "queue.h"

void queue_create(queue_t* queue, uint32_t size)
{
    queue->_queue = (float*)malloc(sizeof(float) * size);
    queue->_queue_size = (int32_t) size;
    queue->_queue_in = queue->_queue;
    queue->_queue_out = queue->_queue;
    queue->_queue_remaining = queue->_queue_size;
}

void queue_delete(queue_t* queue)
{
    free(queue->_queue);
    queue->_queue_size = 0;
    queue->_queue_in = NULL;
    queue->_queue_out = NULL;
    queue->_queue_remaining = 0;
}

int32_t queue_put(queue_t queue, float* input, uint32_t n)
{
    if(input == NULL || n == 0 || queue._queue_remaining == 0)
    {
        return;
    }

    if(n > queue._queue_remaining)
    {
        n = queue._queue_remaining;
    }

    if(n > queue._queue_size - queue._queue_in)
    {
        int32_t len = queue._queue_size - queue._queue_in;
        memcpy(queue._queue + queue._queue_in, input, len);
        memcpy(queue._queue, input + len, n - len);
    }
    else
    {
        memcpy(queue._queue + queue._queue_in, input, n);
    }

    queue._queue_in = (queue._queue_in + n) % queue._queue_size;
    queue._queue_remaining -= n;

}

int32_t queue_get(queue_t queue, float* output, uint32_t n)
{

}