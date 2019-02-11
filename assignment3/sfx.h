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

void sfx_create(
    effect_t* effect,
    uint32_t sampling_rate);


void sfx_create_pew(effect_t* effect);

#endif