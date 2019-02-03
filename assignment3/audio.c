#define _POSIX_C_SOURCE 200809L
#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <math.h>

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
