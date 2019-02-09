#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "queue.h"

void queue_create(queue_t* queue, uint32_t size)
{
    queue->_queue = (float*)malloc(sizeof(float) * size);
    memset(queue->_queue, 0, sizeof(float) * size);
    queue->_queue_size = size;
    queue->_queue_in = 0;
    queue->_queue_out = 0;
    queue->_queue_remaining = queue->_queue_size;
}

void queue_delete(queue_t* queue)
{
    free(queue->_queue);
    queue->_queue = NULL;
    queue->_queue_size = 0;
    queue->_queue_in = 0;
    queue->_queue_out = 0;
    queue->_queue_remaining = 0;
}

uint32_t queue_put(queue_t* queue, float* input, uint32_t n)
{
    if(input == NULL || n == 0 || queue->_queue_remaining == 0)
    {
        return 0;
    }

    if(n > queue->_queue_remaining)
    {
        n = (uint32_t) queue->_queue_remaining;
    }

    if(n > queue->_queue_size - queue->_queue_in)
    {
        uint32_t len = queue->_queue_size - queue->_queue_in;
        memcpy(queue->_queue + queue->_queue_in, input, sizeof(float) * len);
        memcpy(queue->_queue, input + len, sizeof(float) * (n - len));
    }
    else
    {
        memcpy(queue->_queue + queue->_queue_in, input, sizeof(float) * n);
    }

    queue->_queue_in = (queue->_queue_in + n) % queue->_queue_size;
    queue->_queue_remaining -= n;
  
    return n;
}

uint32_t queue_get(queue_t* queue, float* output, uint32_t n)
{
  if(output == NULL || n == 0 || queue->_queue_remaining == 0)
  {
    return 0;
  }

  uint32_t available = queue->_queue_size - queue->_queue_remaining;

  if(n > available)
  {
    n = available;    
  }

  if(n > queue->_queue_size - queue->_queue_out)
  {
    uint32_t len = queue->_queue_size - queue->_queue_out;
    memcpy(output, queue->_queue + queue->_queue_out, sizeof(float) * len);
    memcpy(output + len, queue->_queue, sizeof(float) * (n - len));
  }
  else
  {
    memcpy(output, queue->_queue + queue->_queue_out, sizeof(float) * n);
  }

  queue->_queue_out = (queue->_queue_out + n) % queue->_queue_size;
  queue->_queue_remaining += n;

  return n;
}
