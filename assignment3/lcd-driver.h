#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>

void lcd_init();

void lcd_terminate();

void lcd_goto(uint32_t line, uint32_t pos);

void lcd_putc(uint32_t c);

void lcd_puts(char* str);

#endif
