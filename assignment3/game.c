#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "type-macros.h"
#include "assert-macros.h"
#include "morse.h"
#include "lcd-driver.h"

#define MAX_CHARS 16

static char charset[MAX_CHARS + 1];
static uint32_t cursor;
static uint32_t score;
static uint32_t initialized = FALSE;

void game_print_score(uint32_t value)
{
    ASSERT(value < 16);

    static uint32_t label_printed = FALSE;

    if (FALSE == initialized)
    {
        char* label = strndup("Score:   ", 10);
        lcd_goto(1, 0);
        lcd_puts(label);
        label_printed = TRUE;
    }

    ASSERT(TRUE == label_printed);

    char* string = malloc(sizeof(char) * 9);
    snprintf(string, 9, "%02d", value);

    lcd_goto(1, 7);
    lcd_puts(string);
}

void game_init()
{
    srand((unsigned int)time(NULL));

    for (uint32_t i = 0; i < MAX_CHARS; i++)
    {
        uint32_t c = ((uint32_t)rand()) % MORSE_ALPHABET_LEN;

        charset[i] = morse_alphabet[c];
    }
    charset[MAX_CHARS] = '\0';

    cursor = 0;
    score = 0;

    game_print_score(score);
    lcd_goto(2, 0);
    lcd_puts(charset);

    initialized = TRUE;
}

void game_step(char c)
{
    ASSERT(TRUE == initialized);

    if (cursor >= MAX_CHARS)
    {
        game_init();
    }
    else 
    {
        lcd_goto(2, cursor);

        if (charset[cursor] == c)
        {
            lcd_putc('*');
            score++;
            game_print_score(score);
        }
        else
        {
            lcd_putc(' ');
        }

        cursor++;
    }
}
