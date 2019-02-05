#ifndef AUDIO_H
#define AUDIO_H

#define SAMPLING_RATE 48000
#define CHANNELS 2

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

void audio_init();

void audio_terminate();

float* audio_buffer();

void audio_write();

void audio_saw(
    float*      wave,
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq);

void audio_triangle(
    float*      wave, 
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq);

void audio_sin(
    float*      wave, 
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq);

void audio_cos(
    float*      wave, 
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq);

void audio_pulse(
    float*      wave,
    uint32_t    pitch, 
    uint32_t    samples,
    uint32_t    freq,
    float       duty);

void audio_noise(
    float*      wave,
    uint32_t    pitch, 
    uint32_t    samples);

void audio_add(
    float*      dst,
    uint32_t    dst_pitch,
    float*      src,
    uint32_t    src_pitch,
    uint32_t    samples,
    float       src_ratio);

void audio_ring(
    float*      dst,
    uint32_t    dst_pitch,
    float*      src,
    uint32_t    src_pitch,
    uint32_t    samples);

#endif
