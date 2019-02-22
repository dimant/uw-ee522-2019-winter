#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>

void lcd_init();

void lcd_terminate();

void lcd_putc(uint32_t c);

#endif
