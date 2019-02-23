#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memgpio.h"
#include "lcd-driver.h"
#include "delay.h"

#include "audio.h"

#include "sfx.h"

#define DASH_PIN 19
#define DOT_PIN  26

#define FREQ 770
#define WPM 15

// These durations are defined as per:
// http://www.nu-ware.com/NuCode%20Help/index.html?morse_code_structure_and_timing_.htm

inline uint32_t morse_dot_duration(uint32_t wpm)
{
    // dots per second is calculated as wpm * 2.4
    //
    // then divide 1 / (wpm 2.4) to get seconds per dot
    // finally multiply the result by 1000 to get milliseconds
    return (uint32_t)((1.000f / 2.4f) / (float)wpm);
}

inline uint32_t morse_dot_character_duration(uint32_t wpm)
{
    // 1 dot length for the signal, 1 dot length of silence
    return morse_dot_duration(wpm) * 2;
}

inline uint32_t morse_dash_character_duration(uint32_t wpm)
{
    // 3 dot lengths for the signal, 1 dot length of silence
    return morse_dot_duration(wpm) * 4;
}

inline uint32_t morse_word_space_duration(uint32_t wpm)
{
    return morse_dot_duration(wpm) * 7;
}

int main(int argc, char* argv[])
{
    mgp_init();
    lcd_init();

    char* hello = strndup("Hello World!", 12);

    lcd_goto(1, 0);
    
    lcd_puts(hello);

    lcd_terminate();
    mgp_terminate();
    return 0;
}

//int main(int argc, char* argv[])
//{
//    audio_t audio_device;
//    audio_device.name = "default";
//    audio_device.sampling_rate = 16000;
//    audio_device.channels = 1;
//
//    effect_t dash_sound;
//    dash_sound.buffer = NULL;
//    dash_sound.size = 0;
//    dash_sound.sampling_rate = audio_device.sampling_rate;
//    sfx_create_dash(&dash_sound, FREQ, WPM);
//
//    effect_t dot_sound;
//    dot_sound.buffer = NULL;
//    dot_sound.size = 0;
//    dot_sound.sampling_rate = audio_device.sampling_rate;
//    sfx_create_dot(&dot_sound, FREQ, WPM);
//
//    uint32_t pins;
//
//    audio_init(&audio_device);
//    mgp_init();
//    mgp_set_mode(19, GPIO_MODE_INPUT);
//    mgp_set_mode(26, GPIO_MODE_INPUT);
//
//    float audio_buffer[audio_device.frames * 10];
//    
//    while (1)
//    {
//        pins = mgp_get_pins();
//
//        if (BIT_ISSET(pins, DASH_PIN))
//        {
//            while (sfx_get_period(&dash_sound, audio_buffer, audio_device.frames))
//            {
//                audio_write(audio_device.handle, audio_buffer, audio_device.frames);
//            }
//            dash_sound.cursor = 0;
//        }
//        else if (BIT_ISSET(pins, DOT_PIN))
//        {
//            while (sfx_get_period(&dot_sound, audio_buffer, audio_device.frames))
//            {
//                audio_write(audio_device.handle, audio_buffer, audio_device.frames);
//            }
//            dot_sound.cursor = 0;
//        }
//
//        delay(100000);
//    }
//
//    mgp_terminate();
//    audio_terminate(&audio_device);
//    sfx_destroy(&dash_sound);
//    sfx_destroy(&dot_sound);
//}

//int main(int argc, char* argv[])
//{
//    audio_t audio_device;
//    //tracker_state_t tracker_state;
//
//    audio_device.name = "default";
//    audio_device.sampling_rate = 16000;
//    audio_device.channels = 1;
//
//    audio_init(&audio_device);
//
//    //track_row_t* first_row = tracker_load("song1.txt");
//    //const float chunks_per_ms = ((float) audio_device.sampling_rate / 1000.0f) / (float)audio_device.frames;
//    //tracker_state_create(&tracker_state, audio_device.sampling_rate, chunks_per_ms, first_row);
//
//    uint32_t nframes = 1;
//    float* buffer = (float*)malloc(sizeof(float) * audio_device.frames * 10);
//
//    effect_t effect;
//    sfx_create(&effect, audio_device.sampling_rate);
//    sfx_create_pew(&effect);
//
//    while (sfx_get_period(&effect, buffer, audio_device.frames))
//    {
//        //tracker_get_period(&tracker_state, buffer, audio_device.frames);
//        audio_write(audio_device.handle, buffer, nframes * audio_device.frames);
//    }
//
//    audio_terminate(&audio_device);
//
//    return 0;
//}

/* axis and buttons. It's event driven, and has full signed int
*  ranges of the axis (-32768 to 32767). It also lets you pull the joysticks
*  name. The only place this works of that I know of is in the linux 1.x 
*  joystick driver, which is included in the linux 3.2.x kernels
*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <linux/joystick.h>
//
//#define JOY_DEV "/dev/input/js0"
//
//int main()
//{
//    int joy_fd, *axis=NULL; 
//    unsigned int num_of_axis=0, num_of_buttons=0, x;
//    short int *button=NULL, name_of_joystick[80];
//    struct js_event js;
//
//    if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
//    {
//        printf( "Couldn't open joystick\n" );
//        return -1;
//    }
//
//    ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
//    ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
//    ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );
//
//    axis = (int *) calloc( num_of_axis, sizeof( int ) );
//    button = (short int *) calloc( num_of_buttons, sizeof( char ) );
//
//    printf("Joystick detected: %i\n\t%d axis\n\t%d buttons\n\n"
//            , (int) name_of_joystick
//            , num_of_axis
//            , num_of_buttons );
//
//    fcntl( joy_fd, F_SETFL, O_NONBLOCK ); /* use non-blocking mode */
//
//    while( 1 )  /* infinite loop */
//    {
//
//        /* read the joystick state */
//        read(joy_fd, &js, sizeof(struct js_event));
//
//        /* see what to do with the event */
//        switch (js.type & ~JS_EVENT_INIT)
//        {
//            case JS_EVENT_AXIS:
//                axis   [ js.number ] = js.value;
//                break;
//            case JS_EVENT_BUTTON:
//                button [ js.number ] = js.value;
//                break;
//        }
//
//        /* print the results */
//        printf( "X: %6d  Y: %6d  ", axis[0], axis[1] );
//
//        if( num_of_axis > 2 )
//            printf("Z: %6d  ", axis[2] );
//
//        if( num_of_axis > 3 )
//            printf("R: %6d  ", axis[3] );
//
//        for( x=0 ; x<num_of_buttons ; ++x )
//            printf("B%d: %d  ", x, button[x] );
//
//        printf("  \n");
//        fflush(stdout);
//    }
//
//    close( joy_fd );  /* too bad we never get here */
//    return 0;
//}

//
//#include "audio.h"
//#include "tracker.h"
//
//int main(int argc, char* argv[])
//{
//    audio_device_t* device = audio_init(16000, 1);
//    
//    track_row_t* track = tracker_load("track.txt");
//    tracker_play(device, track);
//
//    audio_terminate(device);
//    return 0;
//}

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
//int main(void)
//{
//    int err;
//    int k;
//
//    int f = 440;                //frequency
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
//        buffer[k]   = (float) (sin(x)) * 1.0f; 
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
