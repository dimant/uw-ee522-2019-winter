#include "sfx.h"

#include "audio.h"
#include "type-macros.h"
#include "assert-macros.h"

uint32_t sfx_get_period(effect_t* effect, float* buffer, uint32_t samples)
{
    uint32_t n = samples;
    if (effect->cursor >= effect->size)
    {
        return FALSE;
    }
    if (n > effect->size - effect->cursor)
    {
        n = effect->size - effect->cursor;
    }

    memcpy(buffer, effect->buffer + effect->cursor, sizeof(float) * n);

    effect->cursor += n;
    return TRUE;
}

void sfx_create(effect_t* effect, uint32_t sampling_rate)
{
    ASSERT(sampling_rate > 0);

    effect->buffer = (float*)malloc(sizeof(float) * sampling_rate);
    effect->size = 0;
    effect->cursor = 0;
    effect->sampling_rate = (uint32_t) sampling_rate;
}

void sfx_create_pew(effect_t* effect)
{
    ASSERT(effect != NULL);
    ASSERT(effect->buffer != NULL);

    const uint32_t duration = 250; // ms
    const uint32_t samples = duration * effect->sampling_rate / 1000;
    ASSERT(samples < effect->sampling_rate);
    
    const uint32_t freq_saw = 4;
    const uint32_t period_saw = effect->sampling_rate / freq_saw;

    const uint32_t freq_sin = 554;
    const uint32_t period_sin = effect->sampling_rate / freq_sin;
    
    float buffer[effect->sampling_rate];

    audio_noise(effect->buffer, samples);

    audio_saw(buffer, samples, period_sin, 0);

    audio_add(effect->buffer, buffer, samples, 0.1f);

    audio_saw(buffer, samples, period_saw, 0);

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s] = 1.0f - buffer[s];
    }

    audio_mul(effect->buffer, buffer, samples);

    effect->size = samples;
    effect->cursor = 0;
}
