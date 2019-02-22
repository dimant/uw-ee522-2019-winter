#ifndef MEMGPIO_EXPERIMENTS_H
#define MEMGPIO_EXPERIMENTS_H

#include <stdint.h>

#define BIT_ISSET(var, pos) !!((var) & (1 << (pos)))

#define PIN_BIT(x) (1 << x)

#define GPIO_MODE_INPUT  0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALT0   4
#define GPIO_MODE_ALT1   5
#define GPIO_MODE_ALT2   6
#define GPIO_MODE_ALT3   7
#define GPIO_MODE_ALT4   3
#define GPIO_MODE_ALT5   2

#define GPIO_UP 1
#define GPIO_DN 0

void mgp_init();

void mgp_terminate();

void mgp_set_mode(
    uint32_t pin,
    uint32_t mode);

void mgp_set_upd(
    uint32_t pin,
    uint32_t pud);

void mgp_set_pins(uint32_t bits);

void mgp_clr_pins(uint32_t bits);

uint32_t mgp_get_pins();

#endif
