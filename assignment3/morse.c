#include <stdlib.h>
#include <math.h>

#include "morse.h"

#include "assert-macros.h"

// dot is binary 01
// dash is binary 10
// to encode a sequence, keep shifting left
// e.g: a is .- which is (01 << 1) | 10
// using a 32bit integer, up strings of up to 16 
// characters can be represented which is plenty
//
/// Letter  Morse   Binary      Decimal
//  a       .-      0110        6
//  b       -...    10010101    149
//  c       -.-.    10011001    153
//  d       -..     100101      37
//  e       .       01          1
//  f       ..-.    01011001    89
//  g       --.     101001      41
//  h       ....    01010101    85
//  i       ..      0101        5
//  j       .---    01101010    106
//  k       -.-     100110      38
//  l       .-..    01100101    101
//  m       --      1010        10
//  n       -.      1001        9
//  o       ---     101010      42
//  p       .--.    01101001    105
//  q       --.-    10100110    166
//  r       .-.     011001      25
//  s       ...     010101      21
//  t       -       10          2
//  u       ..-     010110      22
//  v       ...-    01010110    86
//  w       .--     011010      26
//  x       -..-    10010110    150
//  y       -.--    10011010    154
//  z       --..    10100101    165
//  1       .----   0110101010  426
//  2       ..---   0101101010  ‭‬362
//  3       ...--   0101011010  ‭‬346
//  4       ....-   0101010110  342
//  5       .....   0101010101  341
//  6       -....   1001010101  597
//  7       --...   1010010101  661
//  8       ---..   1010100101  677
//  9       ----.   1010101001  681
//  0       -----   1010101010  682


uint32_t morse_map[] = {
    6, 149, 153, 37, 1, 89, 41, 85, 5, 106, 38, 101, 10, 9, 42,
    105, 166, 25, 21, 2, 22, 86, 26, 150, 154, 165, 426, 362, 346,
    342, 341, 597, 661, 677, 681, 682
};

char morse_alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};

// These durations are defined as per:
// http://www.nu-ware.com/NuCode%20Help/index.html?morse_code_structure_and_timing_.htm

uint32_t morse_dot_duration(uint32_t wpm)
{
    // Speed (WPM) = 2.4 * (Dots per second)
    // -> wpm / 2.4 = dps
    // 
    // then divide 1 / (wpm 2.4) to get seconds per dot
    // finally multiply the result by 1000 to get millise conds
    return (uint32_t)ceilf(1000.0f / ((float)wpm / 2.4f));
}

uint32_t morse_dash_duration(uint32_t wpm)
{
    return morse_dot_duration(wpm) * 3;
}

uint32_t morse_dot_character_duration(uint32_t wpm)
{
    // 1 dot length for the signal, 1 dot length of silence
    return morse_dot_duration(wpm) * 2;
}

uint32_t morse_dash_character_duration(uint32_t wpm)
{
    // 3 dot lengths for the signal, 1 dot length of silence
    return morse_dot_duration(wpm) * 4;
}

uint32_t morse_word_space_duration(uint32_t wpm)
{
    return morse_dot_duration(wpm) * 7;
}

void morse_put(uint32_t* morse, uint32_t symbol)
{
    ASSERT(morse != NULL);
    ASSERT(*morse < 700);
    ASSERT(symbol == MORSE_DOT || symbol == MORSE_DASH);

    *morse = symbol | (*morse << 2);
}

char morse_decode(uint32_t* morse)
{
    ASSERT(morse != NULL);
    ASSERT(*morse < 700);
    ASSERT(*morse > 0);

    for (uint32_t c = 0; c < MORSE_ALPHABET_LEN; c++)
    {
        if (*morse == morse_map[c])
        {
            return morse_alphabet[c];
        }
    }

    return 0;
}