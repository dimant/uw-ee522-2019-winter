#ifndef MEMGPIO_EXPERIMENTS_H
#define MEMGPIO_EXPERIMENTS_H

#include <stdint.h>

#define MODE_INPUT  0
#define MODE_OUTPUT 1

int mgp_blinkLED(char pin);


int mgp_init();

void mgp_terminate();

void mgp_setMode(uint8_t pin, uint8_t mode);

void mgp_bits_set(uint32_t bits);

void mgp_bits_clr(uint32_t bits);

#endif
