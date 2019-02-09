#include <stdint.h>
#include <math.h>
#include <time.h>

#include "audio.h"
#include "assert-macros.h"

void audio_init(audio_t* device, const char* name, uint32_t sampling_rate, uint32_t channels)
{
    ASSERT(device != NULL);
    ASSERT(sampling_rate > 0);
    ASSERT(channels > 0);

    int err;

    queue_create(&(device->buffer), 2 * channels * sampling_rate);
    device->sampling_rate = sampling_rate;
    device->channels = channels;

    err = snd_pcm_open(&(device->handle), name, SND_PCM_STREAM_PLAYBACK, 0);
    ASSERT(err >= 0);

    err = snd_pcm_set_params(device->handle,
                    SND_PCM_FORMAT_FLOAT,
                    SND_PCM_ACCESS_RW_INTERLEAVED,
                    channels,
                    sampling_rate,
                    1,
                    100000);

    ASSERT(err >= 0);
}

void audio_write(audio_t* device)
{
    //snd_pcm_writei(device->handle, device->buffer, device->sampling_rate);
}

void audio_terminate(audio_t* device)
{
    snd_pcm_close(device->handle);
    queue_delete(&(device->buffer));
}

void audio_interleave(float* output, float* left_channel, float* right_channel, uint32_t samples)
{
    for(uint32_t s = 0; s < samples; s++)
    {
        output[2 * s] = left_channel[s];
        output[2 * s + 1] = right_channel[s];
    }
}

void audio_saw(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period)
{ 
    float step = 1.0f / (float) period;

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s] = step * (float) (s % period); 
    }
}

void audio_triangle(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period)
{
    float step = 1.0f / (float) (period >> 1);

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s] = (float) fabs(step * (float) (s % period) - 1.0f);
    }
}

void audio_sin(
    float*      buffer,
    uint32_t    samples,
    float       period)
{
    float fact = 2.0f * (float)M_PI * period; 

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s] = (float) sinf(fact * (float) s); 
    }
}

void audio_pulse(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period,
    uint32_t    duty)
{
    float _duty = ((float) duty) / 100.0f;
    uint32_t sduty = (uint32_t) ((float) period * _duty);

    for(uint32_t s = 0; s < samples; s++)
    {
        if(s % period > sduty)
        {
            buffer[s] = 0.0f;
        }
        else
        {
            buffer[s] = 1.0f;
        }
    }
}

void audio_noise(
    float*      buffer,
    uint32_t    samples)
{
    srand((unsigned int) time(NULL));

    for(uint32_t s = 0; s < samples; s++)
    {
        buffer[s] = ((float) rand() / (float) RAND_MAX);
    }
}

