#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void audio_saw(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period,
    uint32_t    chunk);

void audio_pulse(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period,
    uint32_t    chunk,
    uint32_t    duty);

#endif