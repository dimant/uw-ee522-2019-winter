#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <math.h>
#include <time.h>

#include "audio.h"
#include "assert-macros.h"

static const char *device = "default";
snd_output_t* output = NULL;
static snd_pcm_t* handle;

static float buffer[SAMPLING_RATE * CHANNELS];

void audio_init()
{
    int err;

    err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    ASSERT(err >= 0);

    err = snd_pcm_set_params(handle,
                    SND_PCM_FORMAT_FLOAT,
                    SND_PCM_ACCESS_RW_INTERLEAVED,
                    CHANNELS,
                    SAMPLING_RATE,
                    1,
                    100000);
    ASSERT(err >= 0);
}

float* audio_buffer()
{
    return buffer;
}

void audio_write()
{
    snd_pcm_writei(handle, buffer, SAMPLING_RATE);
}

void audio_terminate()
{
    snd_pcm_close(handle);
}

void audio_saw(
    float*      wave,
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq)
{ 
    uint32_t phase = SAMPLING_RATE / freq;
    
    float step = 1.0f / (float) phase;

    for(uint32_t s = 0; s < samples; s++)
    {
        wave[s * pitch] = step * (float) (s % phase); 
    }
}

void audio_triangle(
    float*      wave, 
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq)
{
    uint32_t phase = SAMPLING_RATE / freq;
    float step = 1.0f / (float) (phase >> 1);

    for(uint32_t s = 0; s < samples; s++)
    {
        wave[s * pitch] = (float) fabs(step * (float) (s % phase) - 1.0f);
    }
}

void audio_sin(
    float*      wave, 
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq)
{
    for(uint32_t s = 0; s < samples; s ++)
    {
        float x = (float)(2.0f * M_PI * (float) freq / (float) SAMPLING_RATE * (float) s);
        wave[s * pitch] = (float) sinf(x); 
    }
}

void audio_cos(
    float*      wave, 
    uint32_t    pitch,
    uint32_t    samples,
    uint32_t    freq)
{
    for(uint32_t s = 0; s < samples; s ++)
    {
        float x = (float)(2.0f * M_PI * (float) freq / (float) SAMPLING_RATE * (float) s);
        wave[s * pitch] = (float) cosf(x); 
    }
}

void audio_pulse(
    float*      wave,
    uint32_t    pitch, 
    uint32_t    samples,
    uint32_t    freq,
    float       duty)
{
    uint32_t phase = SAMPLING_RATE / freq;
    uint32_t sduty = (uint32_t) ((float) phase * duty);

    for(uint32_t s = 0; s < samples; s++)
    {
        if(s % phase > sduty)
        {
            wave[s * pitch] = 0.0f;
        }
        else
        {
            wave[s * pitch] = 1.0f;
        }
    }
}

void audio_noise(
    float*      wave,
    uint32_t    pitch, 
    uint32_t    samples)
{
    srand((unsigned int) time(NULL));

    for(uint32_t s = 0; s < samples; s++)
    {
        wave[s * pitch] = ((float) rand() / (float) RAND_MAX);
    }
}

void audio_add(
    float*      dst,
    uint32_t    dst_pitch,
    float*      src,
    uint32_t    src_pitch,
    uint32_t    samples,
    float       src_ratio)
{
    for(uint32_t s = 0; s < samples; s++)
    {
        dst[s * dst_pitch] = 
            (dst[s * dst_pitch] * (1.0f - src_ratio)) + 
            (src[s * src_pitch] * src_ratio);
    }
}

void audio_ring(
    float*      dst,
    uint32_t    dst_pitch,
    float*      src,
    uint32_t    src_pitch,
    uint32_t    samples)
{
    for(uint32_t s = 0; s < samples; s++)
    {
        dst[s * dst_pitch] = 
            dst[s * dst_pitch] * 
            src[s * src_pitch];
    }
}


