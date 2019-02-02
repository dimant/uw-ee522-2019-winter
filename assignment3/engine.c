#include <stdlib.h>
#include <stdint.h>

#include "assert-macros.h"
#include "type-macros.h"

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

uint32_t eng_step()
{
    // move stars

    // move ship

    // check death

    return 0;
}

uint32_t eng_start()
{
    while(1)
    {
        eng_step();
        // sleep some time;
    }

    return 0;
}
