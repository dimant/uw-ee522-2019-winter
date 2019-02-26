#ifndef MORSE_H
#define MORSE_H

#include <stdint.h>

#define MORSE_DOT   1
#define MORSE_DASH  2

uint32_t morse_dot_duration(uint32_t wpm);

uint32_t morse_dash_duration(uint32_t wpm);

uint32_t morse_dot_character_duration(uint32_t wpm);

uint32_t morse_dash_character_duration(uint32_t wpm);

uint32_t morse_word_space_duration(uint32_t wpm);

void morse_put(uint32_t* morse, uint32_t symbol);

char morse_decode(uint32_t* morse);

#endif