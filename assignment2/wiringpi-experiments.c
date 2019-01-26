#include <wiringPi.h>
#include <assert.h>

#include "wiringpi-experiments.h"

void wpie_blinkLED(int pin)
{
	int result = wiringPiSetupSys();
	assert(result == 0);

	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	delay(1000);
	digitalWrite(pin, LOW);
}