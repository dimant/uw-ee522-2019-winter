#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "queue.h"

void queue_create(queue_t* queue, uint32_t size)
{
    queue->_queue = (float*)malloc(sizeof(float) * size);
    memset(queue->_queue, 0, sizeof(float) * size);
    queue->_size = size;
    queue->_in = 0;
    queue->_out = 0;
    queue->_remaining = queue->_size;
}

void queue_delete(queue_t* queue)
{
    free(queue->_queue);
    queue->_queue = NULL;
    queue->_size = 0;
    queue->_in = 0;
    queue->_out = 0;
    queue->_remaining = 0;
}

uint32_t queue_put(queue_t* queue, float* input, uint32_t n)
{
    if(input == NULL || n == 0 || queue->_remaining == 0)
    {
        return 0;
    }

    if(n > queue->_remaining)
    {
        n = (uint32_t) queue->_remaining;
    }

    if(n > queue->_size - queue->_in)
    {
        uint32_t len = queue->_size - queue->_in;
        memcpy(queue->_queue + queue->_in, input, sizeof(float) * len);
        memcpy(queue->_queue, input + len, sizeof(float) * (n - len));
    }
    else
    {
        memcpy(queue->_queue + queue->_in, input, sizeof(float) * n);
    }

    queue->_in = (queue->_in + n) % queue->_size;
    queue->_remaining -= n;
  
    return n;
}

uint32_t queue_get(queue_t* queue, float* output, uint32_t n)
{
  if(output == NULL || n == 0 || queue->_remaining == 0)
  {
    return 0;
  }

  uint32_t available = queue->_size - queue->_remaining;

  if(n > available)
  {
    n = available;    
  }

  if(n > queue->_size - queue->_out)
  {
    uint32_t len = queue->_size - queue->_out;
    memcpy(output, queue->_queue + queue->_out, sizeof(float) * len);
    memcpy(output + len, queue->_queue, sizeof(float) * (n - len));
  }
  else
  {
    memcpy(output, queue->_queue + queue->_out, sizeof(float) * n);
  }

  queue->_out = (queue->_out + n) % queue->_size;
  queue->_remaining += n;

  return n;
}
