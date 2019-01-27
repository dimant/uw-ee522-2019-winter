#include <assert.h>

#include "export-pin.h"

#include "wiringpi-experiments.h"
#include "bcm2835-experiments.h"
#include "pigpio-experiments.h"
#include "memgpio-experiments.h"

int main(void)
{
    return mgp_blinkLED(19);
}