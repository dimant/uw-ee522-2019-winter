#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "draw.h"
#include "assert-macros.h"
#include "memgpio.h"
#include "type-macros.h"

static int draw_initialized = FALSE;

static uint32_t* back_buffer = NULL;
static uint32_t* front_buffer = NULL;
static uint32_t* buffer = NULL;
static uint32_t buffer_w = 0;
static uint32_t buffer_h = 0;

#define CLEAR_BUFFER(b) ASSERT(memset(b, 0, buffer_w * buffer_h * sizeof(b)) != NULL)
#define PUTPIXEL(x, y) buffer[(x) + (y) * Y_MAX] = TRUE;
#define CLRPIXEL(x, y) buffer[(x) + (y) * Y_MAX] = FALSE;


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

uint32_t* draw_swap()
{
    ASSERT(draw_initialized == TRUE);

    if(buffer == front_buffer)
    {
        buffer = back_buffer;
    }
    else
    {
        buffer = front_buffer;
    }

    CLEAR_BUFFER(buffer);

    return buffer;
}

/// draws the outline of a rectangle - horizontal and vertical lines only
uint32_t* draw_rect(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h)
{
    ASSERT(x0 >= 0);
    ASSERT(y0 >= 0);
    ASSERT(w > 0);
    ASSERT(h > 0);

    ASSERT(draw_initialized == TRUE);
    ASSERT(x0 + w - 1 <= buffer_w);
    ASSERT(y0 + h - 1 <= buffer_h);

    uint32_t i;

    for(i = x0; i < x0 + w; i++)
    {
        PUTPIXEL(i, y0);
    }

    for(i = y0; i < y0 + h; i++)
    {
        PUTPIXEL(x0 + w - 1, i);
    }

    for(i = x0 + w - 1; i >= x0; i--)
    {
        PUTPIXEL(i, y0 + h - 1);
    }

    for(i = y0 + h - 1; i >= y0; i--)
    {
        PUTPIXEL(x0, i);
    }

    return buffer;
}

/// draws a line from (x0, y0) to (x1, y1) using Bresenham's line algorithm
/// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
uint32_t* draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
    ASSERT(x0 >= 0);
    ASSERT(x1 > 0);
    ASSERT(y0 >= 0);
    ASSERT(y1 > 0);

    ASSERT(x0 < buffer_w);
    ASSERT(x1 - 1 < buffer_w);
    ASSERT(y0 < buffer_w);
    ASSERT(x1 - 1 < buffer_w);

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
        PUTPIXEL(x, y);
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

uint32_t* draw_hline(uint32_t x0, uint32_t y0, uint32_t l)
{
    ASSERT(y0 > 0);
    ASSERT(y0 < buffer_h);
    ASSERT(x0 > 0);
    ASSERT(x0 + l < buffer_w);

    for(uint32_t x = x0; x < x0 + l; x++)
    {
       PUTPIXEL(x, y0); 
    }

    return buffer;
}

uint32_t* draw_vline(uint32_t x0, uint32_t y0, uint32_t l)
{
    ASSERT(y0 > 0);
    ASSERT(y0 + l < buffer_h);
    ASSERT(x0 > 0);
    ASSERT(x0 < buffer_w);

    for(uint32_t y = y0; y < y0 + l; y++)
    {
       PUTPIXEL(x0, y); 
    }

    return buffer;
}

uint32_t* draw_circle(uint32_t x0, uint32_t y0, uint32_t r) 
{ 
    ASSERT(x0 - r >= 0);
    ASSERT(y0 - r >= 0);
    ASSERT(x0 + r < buffer_w);
    ASSERT(y0 + r < buffer_h);

    int x = (int) r;
    int y = 0; 

    // Printing the initial point on the axes after translation 
    PUTPIXEL(x + (int) x0, y + (int) y0);

    // When radius is zero only a single point will be printed 
    if (r > 0) 
    { 
        PUTPIXEL( x + (int) x0, -y + (int) y0);
        PUTPIXEL( y + (int) x0,  x + (int) y0);
        PUTPIXEL(-y + (int) x0,  x + (int) y0);
    } 

    // Initialising the value of P 
    int P = 1 - (int) r; 
    while (x > y) 
    {  
        y++; 

        // Mid-point is inside or on the perimeter 
        if (P <= 0) 
            P = P + 2*y + 1; 

        // Mid-point is outside the perimeter 
        else
        { 
            x--; 
            P = P + 2*y - 2*x + 1; 
        } 

        // All the perimeter points have already been printed 
        if (x < y) 
            break; 

        // Printing the generated point and its reflection 
        // in the other octants after translation 
        PUTPIXEL( x + (int) x0,  y + (int) y0);
        PUTPIXEL(-x + (int) x0,  y + (int) y0);
        PUTPIXEL( x + (int) x0, -y + (int) y0);
        PUTPIXEL(-x + (int) x0, -y + (int) y0);

        // If the generated point is on the line x = y then  
        // the perimeter points have already been printed 
        if (x != y) 
        { 
            PUTPIXEL( y + (int) x0,  x + (int) y0);
            PUTPIXEL(-y + (int) x0,  x + (int) y0);
            PUTPIXEL( y + (int) x0, -x + (int) y0);
            PUTPIXEL(-y + (int) x0, -x + (int) y0);
        } 
    }  

    return buffer;
} 

//uint32_t* draw_circle(uint32_t x0, uint32_t y0, uint32_t radius)
//{
//    ASSERT(x0 - radius >= 0)
//    ASSERT(y0 - radius >= 0)
//    ASSERT(x0 + radius < buffer_w)
//    ASSERT(y0 + radius < buffer_h)
//
//    uint32_t x = radius-1;
//    uint32_t y = 0;
//    uint32_t dx = 1;
//    uint32_t dy = 1;
//    uint32_t err = dx - (radius << 1);
//    
//    while (x >= y)
//    {
//        PUTPIXEL(x0 + x, y0 + y);
//        PUTPIXEL(x0 + y, y0 + x);
//        PUTPIXEL(x0 - y, y0 + x);
//        PUTPIXEL(x0 - x, y0 + y);
//        PUTPIXEL(x0 - x, y0 - y);
//        PUTPIXEL(x0 - y, y0 - x);
//        PUTPIXEL(x0 + y, y0 - x);
//        PUTPIXEL(x0 + x, y0 - y);
//        
//        if (err <= 0)
//        {
//            y++;
//            err += dy;
//            dy += 2;
//        }
//        else
//        {
//            x--;
//            dx += 2;
//            err += dx - (radius << 1);
//        }
//    }
//
//    return buffer;
//}

uint32_t* draw_copy(uint32_t x, uint32_t y, uint32_t* src, uint32_t src_w, uint32_t src_h)
{
    ASSERT(x >= 0);
    ASSERT(y >= 0);
    ASSERT(src != NULL);
    ASSERT(x + src_w - 1 < buffer_w);
    ASSERT(y + src_h - 1 < buffer_h);

    uint32_t src_y, src_x;
    uint32_t dst_y, dst_x;

    for(src_y = 0, dst_y = y; src_y < src_h; src_y++, dst_y++)
    {
        for(src_x = 0, dst_x = x; src_x < src_w; src_x++, dst_x++)
        {
            switch(src[src_x + src_y * src_w])
            {
                case PXL_CLEAR:
                    CLRPIXEL(dst_x, dst_y);
                    break;
                case PXL_PUT:
                    PUTPIXEL(dst_x, dst_y);
                    break;
                case PXL_KEEP:
                    break;
                default:
                    break;

            }
        }
    }

    return buffer;
}

// TODO go read this:
// https://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching
