#ifndef AUDIO_H
#define AUDIO_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdint.h>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include "queue.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

typedef struct audio_t {
    snd_pcm_t*  handle;
    uint32_t    sampling_rate;
    uint32_t    channels;
    queue_t     buffer;
} audio_t;

void audio_init(
    audio_t* device,
    const char* name,
    uint32_t sampling_rate,
    uint32_t channels);

void audio_terminate(audio_t* device);

void audio_write(audio_t* device);

void audio_interleave(
    float* output,
    float* left_channel,
    float* right_channel,
    uint32_t samples);

void audio_saw(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period);

void audio_triangle(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period);

void audio_sin(
    float*      buffer,
    uint32_t    samples,
    float       period);

void audio_pulse(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period,
    uint32_t    duty);

void audio_noise(
    float*      buffer,
    uint32_t    samples);

#endif
