#include "lcd-driver.h"

#include <stdlib.h>

#include "memgpio.h"
#include "type-macros.h"
#include "assert-macros.h"
#include "delay.h"

/// Pinout
// LCD function BCM
//  1  GND      GND
//  2  V+       5v
//  3  Vcc      GND
//  4  RS       21
//  5  R/W      20
//  6  E        16
//  7  DB0      12
//  8  DB1       1
//  9  DB2       7
// 10  DB3       8
// 11  DB4      25
// 12  DB5      24
// 13  DB6      23
// 14  DB7      18
// 15  LED+     5v
// 16  LED-     GND

#define LCD_PIN_DB7 18
#define LCD_PIN_DB6 23
#define LCD_PIN_DB5 24
#define LCD_PIN_DB4 25
#define LCD_PIN_DB3  8
#define LCD_PIN_DB2  7
#define LCD_PIN_DB1  1
#define LCD_PIN_DB0 12
#define LCD_PIN_E   16
#define LCD_PIN_RW  20
#define LCD_PIN_RS  21

#define LCD_CMD_CLR     1
#define LCD_CMD_HOME    2

#define LCD_CMD_ENTRY   (1 << 2)
#define LCD_CMD_ENTRY_I (1 << 1)  // 1: increment / 0: decrement cursor position
#define LCD_CMD_ENTRY_S 1         // 1: shift display / 0: no shift

#define LCD_CMD_ON      (1 << 3)  // turn on / off
#define LCD_CMD_ON_DISP (1 << 2)  // display
#define LCD_CMD_ON_CURS (1 << 1)  // cursor
#define LCD_CMD_ON_BLIN 1         // blinking of cursor or character

#define LCD_CMD_SHIFT   (1 << 4)
#define LCD_CMD_SHIFT_S (1 << 3)  // 1: move cursor / 0: move display
#define LCD_CMD_SHIFT_R (1 << 2)  // 1: right / 0: left

#define LCD_CMD_FUN     (1 << 5)
#define LCD_CMD_FUN_DL  (1 << 4)  // data length 1: 4 bit / 0: 8 bit
#define LCD_CMD_FUN_N   (1 << 3)  // 1: 2 lines / 0: 1 line
#define LCD_CMD_FUN_F   (1 << 2)  // 1: 5x10 dots / 5x7 dots

#define LCD_CMD_CGRAM   (1 << 6) // remaining 6 bits describe CGRAM address

#define LCD_CMD_DDRAM   (1 << 7) // remaining 7 bits describe DDRAM address

#define LCD_CMD_RS      (1 << 8)

// line1: 0x00..0x0F
// line2: 0x40..0x4F
#define LCD_LINE_1      0x00
#define LCD_LINE_2      0x40

#define LCD_TO_GPIO(x) \
    ((BIT_ISSET(x, 0) << LCD_PIN_DB0) | \
    ( BIT_ISSET(x, 1) << LCD_PIN_DB1) | \
    ( BIT_ISSET(x, 2) << LCD_PIN_DB2) | \
    ( BIT_ISSET(x, 3) << LCD_PIN_DB3) | \
    ( BIT_ISSET(x, 4) << LCD_PIN_DB4) | \
    ( BIT_ISSET(x, 5) << LCD_PIN_DB5) | \
    ( BIT_ISSET(x, 6) << LCD_PIN_DB6) | \
    ( BIT_ISSET(x, 7) << LCD_PIN_DB7) | \
    ( BIT_ISSET(x, 8) << LCD_PIN_RS))

uint32_t lcd_entry(uint32_t increment, uint32_t shift)
{
    uint32_t cmd = LCD_CMD_ENTRY;

    if (increment == TRUE)
    {
        cmd |= LCD_CMD_ENTRY_I;
    }

    if (shift == TRUE)
    {
        cmd |= LCD_CMD_ENTRY_S;
    }

    return cmd;
}

uint32_t lcd_on(uint32_t display, uint32_t cursor, uint32_t blinking)
{
    uint32_t cmd = LCD_CMD_ON;

    if (display == TRUE)
    {
        cmd |= LCD_CMD_ON_DISP;
    }

    if (cursor == TRUE)
    {
        cmd |= LCD_CMD_ON_CURS;
    }

    if (blinking == TRUE)
    {
        cmd |= LCD_CMD_ON_BLIN;
    }

    return cmd;
}

uint32_t lcd_shift(uint32_t cursor, uint32_t right)
{
    uint32_t cmd = LCD_CMD_SHIFT;

    if (cursor == TRUE)
    {
        cmd |= LCD_CMD_SHIFT_S;
    }

    if (right == TRUE)
    {
        cmd |= LCD_CMD_SHIFT_R;
    }

    return cmd;
}

uint32_t lcd_fun(uint32_t datalen, uint32_t lines, uint32_t font)
{
    uint32_t cmd = LCD_CMD_FUN;

    // enable 4 bit mode
    if (datalen == TRUE)
    {
        cmd |= LCD_CMD_FUN_DL;
    }

    // 2 lines
    if (lines == TRUE)
    {
        cmd |= LCD_CMD_FUN_N;
    }

    // 5 x 10 font
    if (font == TRUE)
    {
        cmd |= LCD_CMD_FUN_F;
    }

    return cmd;
}

uint32_t lcd_cgram(uint32_t address)
{
    uint32_t cmd = LCD_CMD_CGRAM;

    // only consider last 6 bits
    cmd |= (address & 0x3F);

    return cmd;
}

uint32_t lcd_ddram(uint32_t address)
{
    uint32_t cmd = LCD_CMD_DDRAM;

    // only consider last 7 bits
    cmd |= (address & 0x7F);

    return cmd;
}

uint32_t lcd_isbusy()
{
    uint32_t gpio = mgp_get_pins();
    return gpio | PIN_BIT(LCD_PIN_RW);
}

void lcd_enable()
{
    mgp_clr_pins(PIN_BIT(LCD_PIN_E));
    delay(1);
    mgp_set_pins(PIN_BIT(LCD_PIN_E));
    delay(1);
    mgp_clr_pins(PIN_BIT(LCD_PIN_E));
    delay(100);
}

void lcd_execute(uint32_t cmd)
{
    uint32_t gpio = (uint32_t) LCD_TO_GPIO(cmd);

    mgp_set_pins(gpio);

    lcd_enable();

    mgp_clr_pins(gpio);
}

void lcd_goto(uint32_t line, uint32_t pos)
{
    ASSERT(line == 1 || line == 2);
    ASSERT(pos < 16);
    uint32_t addr = 0;

    // line1: 0x00..0x0F
    // line2: 0x40..0x4F
    if (line == 2)
    {
        addr += 0x40;
    }

    addr += pos;

    lcd_execute(lcd_ddram(addr));
    delay(50);
}

void lcd_putc(uint32_t c)
{
    ASSERT(c < 256);

    lcd_execute(LCD_CMD_RS | c);
    delay(50);
}

void lcd_init()
{
    uint32_t pins[] = { 
        LCD_PIN_RS,
        LCD_PIN_RW,
        LCD_PIN_E,
        LCD_PIN_DB0,
        LCD_PIN_DB1,
        LCD_PIN_DB2,
        LCD_PIN_DB3,
        LCD_PIN_DB4,
        LCD_PIN_DB5,
        LCD_PIN_DB6,
        LCD_PIN_DB6
    };

    for (uint32_t pin = 0; pin < 11; pin++)
    {
        mgp_set_upd(pins[pin], GPIO_UP);
        mgp_set_mode(pins[pin], GPIO_MODE_OUTPUT);
    }

    mgp_clr_pins(
        PIN_BIT(LCD_PIN_RS)  |
        PIN_BIT(LCD_PIN_E)   |
        PIN_BIT(LCD_PIN_DB0) |
        PIN_BIT(LCD_PIN_DB1) |
        PIN_BIT(LCD_PIN_DB2) |
        PIN_BIT(LCD_PIN_DB3) |
        PIN_BIT(LCD_PIN_DB4) |
        PIN_BIT(LCD_PIN_DB5) |
        PIN_BIT(LCD_PIN_DB6) |
        PIN_BIT(LCD_PIN_DB7));

    delay(40000);

    lcd_execute(0x33);
    delay(4500);

    lcd_execute(0x32);
    delay(150);

    lcd_execute(0x30);
    delay(50);

    // increment cursor position, no display shift
    lcd_execute(lcd_entry(1, 0));
    delay(50);

    // display on, cursor off, blink off
    lcd_execute(lcd_on(1, 0, 0));
    delay(50);

    // 8 bit, 2 lines, 5x8 font
    lcd_execute(lcd_fun(1, 1, 0));
    delay(50);

    // clear display
    lcd_execute(LCD_CMD_CLR);
    delay(2000);

    // go to home
    lcd_execute(LCD_CMD_HOME);
    delay(2000);
}

void lcd_terminate()
{
    lcd_execute(LCD_CMD_CLR);
    delay(2000);
    lcd_execute(LCD_CMD_HOME);
    delay(2000);
    lcd_execute(lcd_on(0, 0, 0));
}
