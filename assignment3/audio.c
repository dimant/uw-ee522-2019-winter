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

    snd_pcm_hw_params_t *params;

    err = snd_pcm_open(
            &(device->handle),
            device->name,
            SND_PCM_STREAM_PLAYBACK,
            0 // or set to SND_PCM_NONBLOCK
            );
    ASSERT(err >= 0);

    snd_pcm_hw_params_alloca(&params);

    err = snd_pcm_hw_params_any(device->handle, params);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_set_access(
            device->handle,
            params,
            SND_PCM_ACCESS_RW_INTERLEAVED);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_set_format(
            device->handle,
            params,
            SND_PCM_FORMAT_FLOAT);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_set_channels(
            device->handle,
            params,
            device->channels);
    ASSERT(err >= 0);

    uint32_t rate = device->sampling_rate;
    err = snd_pcm_hw_params_set_rate_near(
            device->handle,
            params,
            &rate,
            0);
    ASSERT(err >= 0);
    ASSERT(rate == device->sampling_rate);

    device->frames = 32;
    err = snd_pcm_hw_params_set_period_size_near(
            device->handle,
            params,
            &device->frames,
            &subunit_direction);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params(device->handle, params);
    ASSERT(err >= 0);

    err = snd_pcm_hw_params_get_period_size(
            params, 
            &device->frames,
            &subunit_direction);
    ASSERT(err >= 0);

    // period time in micro-seconds
    uint32_t period_time;
    err = snd_pcm_hw_params_get_period_time(
            params,
            &period_time,
            &subunit_direction);
    ASSERT(err >= 0);
}

void audio_write(
    snd_pcm_t* handle,
    float* buffer,
    uint32_t frames)
{
    snd_pcm_sframes_t written_frames = snd_pcm_writei(handle, buffer, frames);
    ASSERT(written_frames >= 0);
    if(written_frames == -EPIPE)
    {
        /* EPIPE means underrun */
        snd_pcm_prepare(handle);
    }
}

void audio_terminate(audio_t* device)
{
    snd_pcm_drain(device->handle);
    snd_pcm_close(device->handle);
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
        uint32_t    period,
        uint32_t    chunk)
{ 
    float step = 1.0f / (float)period;
    uint32_t offset = chunk * samples % period;

    for(uint32_t s = offset; s < samples + offset; s++)
    {
        buffer[s] = step * (float) (s % period);
    }
}

//void audio_triangle(
//        float*      buffer,
//        uint32_t    samples,
//        uint32_t    freq,
//        uint32_t    period)
//{
//    float step = 1.0f / (float) (freq >> 1);
//    uint32_t offset = period * samples % freq;
//
//    for(uint32_t s = offset; s < samples + offset; s++)
//    {
//        buffer[s] = (float) fabs(step * (float) (s % freq) - 1.0f);
//    }
//}
//
//void audio_sin(
//        float*      buffer,
//        uint32_t    samples,
//        uint32_t    freq,
//        uint32_t    period)
//{
//    float fact = 2.0f * (float)M_PI * (float) freq;
//    uint32_t offset = period * samples % freq;
//
//    for(uint32_t s = offset; s < samples + offset; s++)
//    {
//        buffer[s] = (float) sinf(fact * (float) s); 
//    }
//}
//
//void audio_pulse(
//        float*      buffer,
//        uint32_t    samples,
//        uint32_t    freq,
//        uint32_t    period,
//        uint32_t    duty)
//{
//    float _duty = ((float) duty) / 100.0f;
//    uint32_t offset = period * samples % freq;
//    uint32_t sduty = (uint32_t) ((float)freq * _duty);
//
//    for(uint32_t s = offset; s < samples + offset; s++)
//    {
//        if(s % freq > sduty)
//        {
//            buffer[s] = 0.0f;
//        }
//        else
//        {
//            buffer[s] = 1.0f;
//        }
//    }
//}
//
//void audio_noise(
//        float*      buffer,
//        uint32_t    samples)
//{
//    srand((unsigned int) time(NULL));
//
//    for(uint32_t s = 0; s < samples; s++)
//    {
//        buffer[s] = ((float) rand() / (float) RAND_MAX);
//    }
//}

