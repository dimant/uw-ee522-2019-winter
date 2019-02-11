#include <stdlib.h>
#include <stdint.h>

#include "assert-macros.h"
#include "type-macros.h"
#include "draw.h"
#include "memgpio.h"
#include "delay.h"

#define PXL_DELAY 1

//static uint32_t** stars;
//static uint32_t   nstars;

//static uint32_t* sprite_ship;
//static uint32_t  sprite_ship_w;
//static uint32_t  sprite_ship_h;
//static uint32_t  sprite_ship_x;
//static uint32_t  sprite_ship_y;

volatile uint32_t button_up;
volatile uint32_t button_down;

uint32_t eng_collide(
    uint32_t* buffer, uint32_t buffer_w, uint32_t buffer_h, 
    uint32_t x, uint32_t y, 
    uint32_t* src, uint32_t src_w, uint32_t src_h)
{
    ASSERT(x >= 0)
    ASSERT(y >= 0)
    ASSERT(src != NULL)
    ASSERT(x + src_w - 1 < buffer_w)
    ASSERT(y + src_h - 1 < buffer_h)

    uint32_t src_y, src_x;
    uint32_t dst_y, dst_x;

    for(src_y = 0, dst_y = y; src_y < src_h; src_y++, dst_y++)
    {
        for(src_x = 0, dst_x = x; src_x < src_w; src_x++, dst_x++)
        {
            if(src[src_x + src_y * src_w] & buffer[dst_x + dst_y * buffer_w])
            {
               return TRUE; 
            }
        }
    }

    return FALSE;
}

void eng_render(uint32_t* buffer)
{
    for(uint32_t y = 0; y < Y_MAX; y++)
    {
        for(uint32_t x = 0; x < X_MAX; x++)
        {
            if(buffer[x + y * Y_MAX])
            {
                mgp_xy_set(x, y);
                delay(PXL_DELAY);
                mgp_xy_clr(x, y);
            }
        }
    }
}

void eng_step()
{
    // move stars

    // move ship

    // check death
}

void eng_start()
{
    // start sound thread
    //      allocate zeros
    //      mix in bg music
    //      mix in sfx
    //      write

    // allocate render buffer 1 & 2
    // start render thread
    //      render

    while(1)
    {
        // read input
        // do step
        // fire sfx events if any
        // render to front buffer
        // swap with back buffer
    }
}
