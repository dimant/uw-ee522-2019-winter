#include <assert.h>

#include "export-pin.h"

#include "wiringpi-experiments.h"
#include "bcm2835-experiments.h"
#include "pigpio-experiments.h"

int main(void)
{
	export_pin(19, OUTPUT);

	pgpe_blinkLED(19);

	return 0;
}