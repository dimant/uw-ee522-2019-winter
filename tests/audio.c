#include "audio.h"

void audio_pulse(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period,
    uint32_t    chunk,
    uint32_t    duty)
{
    float _duty = ((float)duty) / 100.0f;
    uint32_t offset = chunk * samples % period;
    uint32_t sduty = (uint32_t)((float)period * _duty);

    for (uint32_t s = offset; s < samples + offset; s++)
    {
        if (s % period >= sduty)
        {
            buffer[s - offset] = 0.0f;
        }
        else
        {
            buffer[s - offset] = 1.0f;
        }
    }
}

void audio_saw(
    float*      buffer,
    uint32_t    samples,
    uint32_t    period,
    uint32_t    chunk)
{
    float step = 1.0f / (float)period;
    uint32_t offset = chunk == 0 ? 0 : period % (chunk * samples);

    for (uint32_t s = offset; s < samples + offset; s++)
    {
        buffer[s - offset] = step * (float)(s % period);
    }
}