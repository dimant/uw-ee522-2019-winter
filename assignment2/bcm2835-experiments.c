#include <bcm2835.h>
#include <assert.h>

#include "bcm2835-experiments.h"

void bcme_blinkLED(unsigned char pin)
{
    int result = 0;

    result = bcm2835_init();
    assert(result == 1);

    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, HIGH);
    bcm2835_delay(1000);
    bcm2835_gpio_write(pin, LOW);
    
    result = bcm2835_close();
    assert(result == 1);
}