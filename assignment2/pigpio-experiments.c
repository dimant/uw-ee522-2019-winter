#include <pigpio.h>
#include <assert.h>

#include "pigpio-experiments.h"

void pgpe_blinkLED(unsigned char pin)
{
	int result = gpioInitialise();
	assert(result >= 0);

	result = gpioSetMode(pin, PI_OUTPUT);
	assert(result == 0);

	result = gpioWrite(pin, 1);
	assert(result == 0);

	uint32_t delay = gpioDelay(1000 * 1000);
	assert(delay > 100);

	result = gpioWrite(pin, 0);
	assert(result == 0);

	gpioTerminate();
}