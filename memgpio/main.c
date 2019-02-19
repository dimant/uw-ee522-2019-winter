#include "memgpio-experiments.h"

int main(int argc, char* argv[])
{
    mgp_init();


    for(uint8_t pin = 0; pin < 32; pin++)
    {
        mgp_setMode(pin, MODE_OUTPUT);
    }
    //mgp_bits_set(1 << 27);
    mgp_bits_clr(0xFFFFFFFF);

    while(1){}

    mgp_terminate();

    return 0;
}
