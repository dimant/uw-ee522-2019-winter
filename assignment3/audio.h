#ifndef AUDIO_H
#define AUDIO_H

#define _POSIX_C_SOURCE 200809L
#include <stdint.h>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

typedef struct audio_devices_t {
    snd_pcm_t* handle;
    float* buffer;
    uint32_t sampling_rate;
    uint32_t channels;
} audio_device_t;

audio_device_t* audio_init(uint32_t sampling_rate, uint32_t channels);

void audio_terminate(audio_device_t* device);

void audio_write(audio_device_t* device);

void audio_saw(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq);

void audio_triangle(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq);

void audio_sin(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq);

void audio_pulse(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq,
    uint32_t    duty);

void audio_noise(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples);

#endif
