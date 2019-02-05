#include <stdint.h>
#include <math.h>
#include <time.h>

#include "audio.h"
#include "assert-macros.h"

audio_device_t* audio_init(uint32_t sampling_rate, uint32_t channels)
{
    int err;
    audio_device_t* device = (audio_device_t*) malloc(sizeof(audio_device_t));

    device->sampling_rate = sampling_rate;
    device->channels = channels;

    err = snd_pcm_open(&(device->handle), "default", SND_PCM_STREAM_PLAYBACK, 0);
    ASSERT(err >= 0);

    device->buffer = (float*) malloc(sizeof(float) * sampling_rate * channels);

    err = snd_pcm_set_params(device->handle,
                    SND_PCM_FORMAT_FLOAT,
                    SND_PCM_ACCESS_RW_INTERLEAVED,
                    channels,
                    sampling_rate,
                    1,
                    100000);

    ASSERT(err >= 0);

    return device; 
}

void audio_write(audio_device_t* device)
{
    snd_pcm_writei(device->handle, device->buffer, device->sampling_rate);
}

void audio_terminate(audio_device_t* device)
{
    snd_pcm_close(device->handle);
    free(device->buffer);
    free(device);
}

void audio_saw(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq)
{ 
    uint32_t phase = (uint32_t) ceil(device->sampling_rate / freq);
    float* buffer = device->buffer;
    uint32_t channels = device->channels;
    
    float step = 1.0f / (float) phase;

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s * channels + channel] = step * (float) (s % phase); 
    }
}

void audio_triangle(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq)
{
    uint32_t phase = (uint32_t) ceil(device->sampling_rate / freq);
    float* buffer = device->buffer;
    uint32_t channels = device->channels;

    float step = 1.0f / (float) (phase >> 1);

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s * channels + channel] = (float) fabs(step * (float) (s % phase) - 1.0f);
    }
}

void audio_sin(
    audio_device_t* device,
    uint32_t    channel,
    uint32_t    samples,
    uint32_t    freq)
{
    float* buffer = device->buffer;
    uint32_t channels = device->channels;

    float fact = 2.0f * (float)M_PI * (float) freq / (float) device->sampling_rate; 

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s * channels + channel] = (float) sinf(fact * (float) s); 
    }
}

void audio_pulse(
    audio_device_t* device,
    uint32_t    channel, 
    uint32_t    samples,
    uint32_t    freq,
    uint32_t    duty)
{
    uint32_t phase = (uint32_t) ceil(device->sampling_rate / freq);
    float* buffer = device->buffer;
    uint32_t channels = device->channels;
    float _duty = ((float) duty) / 100.0f;
    uint32_t sduty = (uint32_t) ((float) phase * _duty);

    for(uint32_t s = 0; s < samples; s++)
    {
        if(s % phase > sduty)
        {
            buffer[s * channels + channel] = 0.0f;
        }
        else
        {
            buffer[s * channels + channel] = 1.0f;
        }
    }
}

void audio_noise(
    audio_device_t* device,
    uint32_t    channel, 
    uint32_t    samples)
{
    srand((unsigned int) time(NULL));
    float* buffer = device->buffer;
    uint32_t channels = device->channels;

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s * channels + channel] = ((float) rand() / (float) RAND_MAX);
    }
}

