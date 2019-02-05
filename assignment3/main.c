#include <stdio.h>

#include "audio.h"
#include "tracker.h"

int main(int argc, char* argv[])
{
    audio_device_t* device = audio_init(16000, 1);
    
    track_row_t* track = tracker_load("track.txt");
    tracker_play(device, track);

    audio_terminate(device);
    return 0;
}

//#define _POSIX_C_SOURCE 200809L
//#include <alsa/asoundlib.h>
//#include <alsa/pcm.h>
//#include <math.h>
//
//#define SAMPLE_RATE 16000
//#define CHANNELS 2
//
//#ifndef M_PI
//#    define M_PI 3.14159265358979323846
//#endif
//
//static const char *device = "default";                       //soundcard
//float buffer [SAMPLE_RATE * CHANNELS];
//
//
//
//int mainx(void)
//{
//    int err;
//    int k;
//
//    int f = 100;                //frequency
//    int fs = SAMPLE_RATE;             //sampling frequency
//
//    snd_pcm_t *handle;
//
//
//    // ERROR HANDLING
//
//    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
//        printf("Playback open error: %s\n", snd_strerror(err));
//        exit(EXIT_FAILURE);
//    }
//
//    if ((err = snd_pcm_set_params(handle,
//                    SND_PCM_FORMAT_FLOAT,
//                    SND_PCM_ACCESS_RW_INTERLEAVED,
//                    CHANNELS,
//                    SAMPLE_RATE,
//                    1,
//                    100000)) < 0) {   
//        printf("Playback open error: %s\n", snd_strerror(err));
//        exit(EXIT_FAILURE);
//    }
//
//    // SINE WAVE
//    printf("Sine tone at %dHz\n ",f);
//
//    for(k = 0; k < SAMPLE_RATE * CHANNELS; k +=2 )
//    {
//        float x = (float)(2.0 * M_PI * f / fs * (k / 2));
//        buffer[k]   = (float) (sin(x)) * 0.3f; 
//        buffer[k+1] = (float) (cos(x)) * 1.0f; 
//    }
//
//    while(1)
//    {
//        snd_pcm_writei(handle, buffer, SAMPLE_RATE);    //sending values to sound driver
//    }
//
//    snd_pcm_close(handle);
//    return 0;
//
//}
