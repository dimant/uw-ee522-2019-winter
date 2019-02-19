#ifndef MEMGPIO_EXPERIMENTS_H
#define MEMGPIO_EXPERIMENTS_H

#include <stdint.h> 

#define MODE_INPUT  0
#define MODE_OUTPUT 1
#define MODE_ALT0   4
#define MODE_ALT1   5
#define MODE_ALT2   6
#define MODE_ALT3   7
#define MODE_ALT4   3
#define MODE_ALT5   2

#define X_MAX 1024
#define Y_MAX 1024

void mgp_init();
void mgp_terminate();
void mgp_set_mode(uint32_t pin, uint32_t mode);
void mgp_xy_set(uint32_t x, uint32_t y);
void mgp_xy_clr(uint32_t x, uint32_t y);
void mgp_bits_set(uint32_t bits);
void mgp_bits_clr(uint32_t bits);
void mgp_blinkLED(uint32_t pin);

#endif
