#include <stdint.h>
#include <math.h>
#include <time.h>

#include "audio.h"
#include "assert-macros.h"

#define BUFFER_CAPACITY 2

void audio_init(audio_t* device, const char* name, uint32_t sampling_rate, uint32_t channels)
{
    ASSERT(device != NULL);

    int err;

    device->sampling_rate = sampling_rate;
    device->channels = channels;

    err = snd_pcm_open(&(device->handle), name, SND_PCM_STREAM_PLAYBACK, 0);
    ASSERT(err >= 0);

    device->_queue_size = BUFFER_CAPACITY * sampling_rate * channels;
    device->buffer = (float*) malloc(sizeof(float) * device->_queue_size);
    memset(device->buffer, 0, sizeof(float) * device->_queue_size);

    device->_queue_in = NULL;
    device->_queue_out = NULL;

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
    // snd_pcm_writei(device->handle, device->buffer, device->sampling_rate);
}

void audio_play(audio_t* device)
{

}

void audio_terminate(audio_t* device)
{
    snd_pcm_close(device->handle);
    free(device->buffer);
    device->_queue_in = 0;
    device->_queue_out = 0;
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

