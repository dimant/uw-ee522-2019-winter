#include <stdint.h>
#include <math.h>
#include <time.h>
#include <alloca.h>

#include "audio.h"
#include "assert-macros.h"

void audio_init(audio_t* device)
{
    ASSERT(device != NULL);
    ASSERT(device->name != NULL);
    ASSERT(device->sampling_rate > 0);
    ASSERT(device->channels > 0);

    int err;
    int subunit_direction;

    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;

    err = snd_pcm_open(
            &handle,
            device->name,
            SND_PCM_STREAM_PLAYBACK,
            0 // or set to SND_PCM_NONBLOCK
            );
    ASSERT(err >= 0);

    snd_pcm_hw_params_alloca(&params);

    err = snd_pcm_hw_params_any(handle, params);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_set_access(
            handle,
            params,
            SND_PCM_ACCESS_RW_INTERLEAVED);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_set_format(
            handle,
            params,
            SND_PCM_FORMAT_FLOAT);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_set_channels(
            handle,
            params,
            device->channels);
    ASSERT(err >= 0);

    uint32_t rate = device->sampling_rate;
    err = snd_pcm_hw_params_set_rate_near(
            handle,
            params,
            &rate,
            0);
    ASSERT(err >= 0);
    ASSERT(rate == device->sampling_rate);

    device->frames = 32;
    err = snd_pcm_hw_params_set_period_size_near(
            handle,
            params,
            &device->frames,
            &subunit_direction);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params(handle, params);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_get_period_size(
            params, 
            &device->frames,
            &subunit_direction);
    ASSERT(err >= 0);

    // allocate enough space for two back buffers
    device->_buffer_size = device->frames * device->channels; 
    uint32_t queue_size = 2 * device->_buffer_size;
    queue_create(&device->queue, queue_size);

    device->_buffer = (float*) malloc(sizeof(float) * device->_buffer_size);

    // period time in micro-seconds
    uint32_t period_time;
    err = snd_pcm_hw_params_get_period_time(
            params,
            &period_time,
            &subunit_direction);
    ASSERT(err >= 0);
}

void audio_write(audio_t* device)
{
    uint32_t n = queue_get(&device->queue, device->_buffer, device->_buffer_size);
    snd_pcm_sframes_t written_frames = snd_pcm_writei(device->handle, device->_buffer, n / device->channels);
    ASSERT(written_frames >= 0);
    if(written_frames == -EPIPE)
    {
        /* EPIPE means underrun */
        snd_pcm_prepare(device->handle);
    }
}

void audio_terminate(audio_t* device)
{
    snd_pcm_drain(device->handle);
    snd_pcm_close(device->handle);
    queue_delete(&device->queue);
}

void audio_interleave(
    float* output,
    float* left_channel,
    float* right_channel,
    uint32_t samples)
{
    for(uint32_t s = 0; s < samples; s++)
    {
        output[2 * s] = left_channel[s];
        output[2 * s + 1] = right_channel[s];
    }
}

void audio_add(
    float*      result,
    float*      buffer,
    uint32_t    samples,
    float       ratio)
{
    for (uint32_t s = 0; s < samples; s++)
    {
        result[s] = result[s] * ratio + (1.0f - ratio) * buffer[s];
    }
}

void audio_saw(
        float*      buffer,
        uint32_t    samples,
        uint32_t    freq,
        uint32_t    period)
{ 
    float step = 1.0f / (float)freq;
    uint32_t offset = period * samples % freq;

    for(uint32_t s = offset; s < samples + offset; s++)
    {
        buffer[s] = step * (float) (s % freq);
    }
}

void audio_triangle(
        float*      buffer,
        uint32_t    samples,
        uint32_t    freq,
        uint32_t    period)
{
    float step = 1.0f / (float) (freq >> 1);
    uint32_t offset = period * samples % freq;

    for(uint32_t s = offset; s < samples + offset; s++)
    {
        buffer[s] = (float) fabs(step * (float) (s % freq) - 1.0f);
    }
}

void audio_sin(
        float*      buffer,
        uint32_t    samples,
        uint32_t    freq,
        uint32_t    period)
{
    float fact = 2.0f * (float)M_PI * (float) freq;
    uint32_t offset = period * samples % freq;

    for(uint32_t s = offset; s < samples + offset; s++)
    {
        buffer[s] = (float) sinf(fact * (float) s); 
    }
}

void audio_pulse(
        float*      buffer,
        uint32_t    samples,
        uint32_t    freq,
        uint32_t    period,
        uint32_t    duty)
{
    float _duty = ((float) duty) / 100.0f;
    uint32_t offset = period * samples % freq;
    uint32_t sduty = (uint32_t) ((float)freq * _duty);

    for(uint32_t s = offset; s < samples + offset; s++)
    {
        if(s % freq > sduty)
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

