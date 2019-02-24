#include "sfx.h"

#include <math.h>

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

void sfx_destroy(effect_t* effect)
{
    ASSERT(effect != NULL);

    if (effect->buffer != NULL)
    {
        free(effect->buffer);
        effect->buffer = NULL;
    }

    effect->size = 0;
    effect->cursor = 0;
    effect->sampling_rate = 0;
}

void sfx_create_sine(effect_t* effect, uint32_t freq, uint32_t ms)
{
    ASSERT(ms > 0);
    ASSERT(effect->sampling_rate > 0);
    ASSERT(effect->buffer == NULL);
    ASSERT(effect->size == 0);

    const uint32_t samples = ms * effect->sampling_rate / 1000;
    uint32_t period = effect->sampling_rate / freq;
    float angle = (float)freq / (float)effect->sampling_rate;

    effect->buffer = (float*)malloc(sizeof(float) * samples);

    audio_sin(effect->buffer, samples, angle, period, 0);

    effect->size = samples;
    effect->cursor = 0;
}

void sfx_create_dot(effect_t* effect, uint32_t freq, uint32_t wpm)
{
    ASSERT(effect->sampling_rate > 0);
    ASSERT(effect->buffer == NULL);
    ASSERT(effect->size == 0);

    // dots per second calculated as per
    // http://www.nu-ware.com/NuCode%20Help/index.html?morse_code_structure_and_timing_.htm
    float dps = (float)wpm / 2.4f;
    uint32_t samples = (uint32_t) ceilf((float) effect->sampling_rate / dps);
    uint32_t period = effect->sampling_rate / freq;

    // 1 dot length sine wave, 1 dot length silence
    effect->buffer = (float*)malloc(sizeof(float) * samples * 2);


    effect->size = samples;
    effect->cursor = 0;

    audio_saw(effect->buffer, samples, period, 0);
    //audio_silence(effect->buffer + dot_samples, dot_samples);
}

void sfx_create_dash(effect_t* effect, uint32_t freq, uint32_t wpm)
{
    ASSERT(effect->sampling_rate > 0);
    ASSERT(effect->buffer == NULL);
    ASSERT(effect->size == 0);

    // dots per second calculated as per
    // http://www.nu-ware.com/NuCode%20Help/index.html?morse_code_structure_and_timing_.htm
    float dps = (float)wpm / 2.4f;
    uint32_t samples = 3 * (uint32_t)ceilf((float)effect->sampling_rate / dps);
    uint32_t period = effect->sampling_rate / freq;

    // 3 dot lengths for the dash and 1 dot length of silence
    effect->buffer = (float*)malloc(sizeof(float) * samples);

    effect->size = samples;
    effect->cursor = 0;

    audio_saw(effect->buffer, samples, period, 0);
    //audio_silence(effect->buffer + dot_samples * 3, dot_samples);
}

void sfx_create_pew(effect_t* effect)
{
    ASSERT(effect != NULL);
    if (effect->buffer == NULL)
    {
        // 3 dot lengths for the dash and 1 dot length of silence
        effect->buffer = (float*)malloc(sizeof(float) * effect->sampling_rate);
    }

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
