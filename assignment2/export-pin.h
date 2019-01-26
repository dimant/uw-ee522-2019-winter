#ifndef EXPORT_PIN_H
#define EXPORT_PIN_H

enum gpio_export_mode_t
{
	INPUT,
	OUTPUT
};

void export_pin(unsigned short pin, enum gpio_export_mode_t mode);

#endif