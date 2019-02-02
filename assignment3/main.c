#include <stdio.h>

#include "drawing-macros.h"
#include "memgpio.h"

int main(void)
{
    mgp_init();

    mgp_setMode(X_BIT0, MODE_OUTPUT);
    mgp_setMode(X_BIT1, MODE_OUTPUT);
    mgp_setMode(X_BIT2, MODE_OUTPUT);
    mgp_setMode(X_BIT3, MODE_OUTPUT);
    mgp_setMode(X_BIT4, MODE_OUTPUT);
    mgp_setMode(X_BIT5, MODE_OUTPUT);
    mgp_setMode(X_BIT6, MODE_OUTPUT);
    mgp_setMode(X_BIT7, MODE_OUTPUT);
    mgp_setMode(X_BIT8, MODE_OUTPUT);
    mgp_setMode(X_BIT9, MODE_OUTPUT);

    mgp_setMode(Y_BIT0, MODE_OUTPUT);
    mgp_setMode(Y_BIT1, MODE_OUTPUT);
    mgp_setMode(Y_BIT2, MODE_OUTPUT);
    mgp_setMode(Y_BIT3, MODE_OUTPUT);
    mgp_setMode(Y_BIT4, MODE_OUTPUT);
    mgp_setMode(Y_BIT5, MODE_OUTPUT);
    mgp_setMode(Y_BIT6, MODE_OUTPUT);
    mgp_setMode(Y_BIT7, MODE_OUTPUT);
    mgp_setMode(Y_BIT8, MODE_OUTPUT);
    mgp_setMode(Y_BIT9, MODE_OUTPUT);


    while(1)
    {
        for(uint32_t x = 0; x < 1024; x++)
        {
            mgp_xy_set(2, x);
            for (int i = 0; i < 1000; i++) {}  // blocking delay hack using a simple loop
            mgp_xy_clr(2, x);
            for (int i = 0; i < 1000; i++) {}  // blocking delay hack using a simple loop
        }
    }


    mgp_terminate();    
    return 0;
}
