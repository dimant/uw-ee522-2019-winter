#ifndef SFX_H
#define SFX_H

#include <stdint.h>

typedef struct effects_t
{
    float*      buffer;
    uint32_t    size;
    uint32_t    cursor;
    uint32_t    sampling_rate;
} effect_t;

uint32_t sfx_get_period(
    effect_t*   effect,
    float*      buffer,
    uint32_t    samples);

void sfx_destroy(effect_t* effect);

void sfx_create_dot(
    effect_t* effect,
    uint32_t freq,
    uint32_t wpm);

void sfx_create_dash(
    effect_t* effect,
    uint32_t freq,
    uint32_t wpm);

void sfx_create_pew(effect_t* effect);

#endif