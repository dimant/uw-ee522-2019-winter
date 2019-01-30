#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "type-macros.h"
#include <assert-macros.h>
#include "memgpio.h"
#include "draw.h"

static int draw_initialized = FALSE;

static uint32_t* back_buffer = NULL;
static uint32_t* front_buffer = NULL;
static uint32_t* buffer = NULL;
static uint32_t buffer_w = 0;
static uint32_t buffer_h = 0;

#define CLEAR_BUFFER(b) ASSERT(memset(b, 0, buffer_w * buffer_h * sizeof(b)) != NULL)

uint32_t* draw_init(uint32_t w, uint32_t h)
{
    ASSERT(w % (8 * sizeof(back_buffer)) == 0);
    ASSERT(h % (8 * sizeof(back_buffer)) == 0);

    buffer_w = w;
    buffer_h = h;

    back_buffer = (uint32_t*) malloc(w*h*sizeof(back_buffer));
    CLEAR_BUFFER(back_buffer);

    front_buffer = (uint32_t*) malloc(w*h*sizeof(front_buffer));
    CLEAR_BUFFER(front_buffer);
    
    buffer = back_buffer;

    draw_initialized = TRUE;

    return buffer;
}

void draw_free()
{
    ASSERT(draw_initialized == TRUE);

    draw_initialized = FALSE;

    free(back_buffer);
    back_buffer = NULL;

    free(front_buffer);
    front_buffer = NULL;

    buffer_w = 0;
    buffer_h = 0;
}

uint32_t* draw_update()
{
    ASSERT(draw_initialized == TRUE);

    uint32_t* draw_buffer = buffer;

    if(buffer == front_buffer)
    {
        buffer = back_buffer;
    }
    else
    {
        buffer = front_buffer;
    }

    CLEAR_BUFFER(buffer);

    return draw_buffer;
}

/// draws the outline of a rectangle - horizontal and vertical lines only
uint32_t* draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    ASSERT(draw_initialized == TRUE);
    ASSERT(x + w - 1 <= buffer_w);
    ASSERT(y + h - 1 <= buffer_h);

    uint32_t i;

    for(i = 0; i < x + w; i++)
    {
        buffer[i + y * buffer_w] = TRUE;
    }

    for(i = 0; i < x + w; i++)
    {
        buffer[i + (y + h - 1) * buffer_w] = TRUE;
    }
    
    for(i = 0; i < y + h; i++)
    {
        buffer[x + i * buffer_w] = TRUE;
    }

    for(i = 0; i < y + h; i++)
    {
        buffer[x + w - 1 + i * buffer_w] = TRUE;
    }

    return buffer;
}

/// draws a line from (x0, y0) to (x1, y1) using Bresenham's line algorithm
/// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
uint32_t* draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
    ASSERT(x0 - 1 < buffer_w);
    ASSERT(x1 < buffer_w);
    ASSERT(y0 - 1 < buffer_w);
    ASSERT(y1 < buffer_w);

    double deltax = x1 - x0;
    double deltay = y1 - y0;

    // Assume deltax != 0 (line is not vertical),
    ASSERT(deltax != 0);

    // note that this division needs to be done in a way that preserves the fractional part
    double deltaerr = fabs(deltay / deltax);
    double error = 0;

    uint32_t y = y0;
    for(uint32_t x = x0; x <= x1; x++)
    {
        buffer[x + y * buffer_w] = TRUE; 
        error += deltaerr;

        if(error > 0.5)
        {
            if(signbit(deltay))
            {
                y -= 1;
            }
            else
            {
                y += 1;
            }

            error -= 1.0;
        }
    }

    return buffer;
}
