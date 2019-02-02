#include <stdio.h>

#define _POSIX_C_SOURCE 200809L

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <math.h>

#define SAMPLING_RATE 16000

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#include "assert-macros.h"

static const char *device = "default";
snd_output_t *output = NULL;
float buffer [SAMPLING_RATE * 2];

int main(void)
{
    int err;
    int j, k;

    int f = 440;                //frequency
    //int fs = SAMPLING_RATE;             //sampling frequency

    snd_pcm_t *handle;
    //snd_pcm_sframes_t frames;


    // ERROR HANDLING

    err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    ASSERT(err >= 0);

    err = snd_pcm_set_params(handle,
                    SND_PCM_FORMAT_FLOAT,
                    SND_PCM_ACCESS_RW_INTERLEAVED,
                    2,
                    SAMPLING_RATE,
                    1,
                    100000);
    ASSERT(err >= 0);

    // SINE WAVE
    printf("Sine tone at %dHz\n",f);

//    for (k = 0; k < SAMPLING_RATE * 2; k += 2)
//    {
//        buffer[k] = (float) (sin(2 * M_PI * f / fs * k));
//        buffer[k + 1] = (float) (sin(2 * M_PI * f / fs * k + M_PI / 2.0));
//    }       

    while(1)
    {
        snd_pcm_writei(handle, buffer, SAMPLING_RATE);    //sending values to sound driver
    }

    snd_pcm_close(handle);
    return 0;
}
