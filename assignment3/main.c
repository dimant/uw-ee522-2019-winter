#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "delay.h"
#include "my-clock.h"
#include "memgpio.h"
#include "lcd-driver.h"
#include "audio.h"
#include "morse.h"
#include "type-macros.h"
#include "assert-macros.h"

#include "sfx.h"
#include "game.h"

#define DASH_PIN 26
#define DOT_PIN  19

#define FREQ 770
#define WPM 20

int main(int argc, char* argv[])
{
    audio_t audio_device;
    audio_device.name = "default";
    audio_device.sampling_rate = 16000;
    audio_device.channels = 1;
    
    audio_init(&audio_device);
    mgp_init();
    lcd_init();

    mgp_set_mode(DASH_PIN, GPIO_MODE_INPUT);
    mgp_set_mode(DOT_PIN, GPIO_MODE_INPUT);

    uint32_t dot_ms = morse_dot_duration(WPM);
    uint64_t dot_char_ms = 2 * dot_ms;
    uint32_t dash_ms = morse_dash_duration(WPM);
    uint64_t dash_char_ms = 4 * dot_ms;

    effect_t dot_sound;
    dot_sound.buffer = NULL;
    dot_sound.size = 0;
    dot_sound.sampling_rate = audio_device.sampling_rate;
    sfx_create_sine(&dot_sound, FREQ, dot_ms);

    effect_t dash_sound;
    dash_sound.buffer = NULL;
    dash_sound.size = 0;
    dash_sound.sampling_rate = audio_device.sampling_rate;
    sfx_create_sine(&dash_sound, FREQ, dash_ms);

    uint32_t pins;
    //char* hello = strndup("Hello World!", 12);
    float audio_buffer[audio_device.frames * 10];

    uint64_t start = 1;
    uint64_t end = 0;

    uint32_t morse = 0;

    game_init();

    while (1)
    {
        pins = mgp_get_pins();
        start = my_clock_gettime();
    
        if (start > end)
        {
            if (start - end > dot_ms && morse > 0)
            {
                game_step(morse_decode(&morse));
                morse = 0;
            }

            if (BIT_ISSET(pins, DASH_PIN))
            {
                morse_put(&morse, MORSE_DASH);

                end = start + dash_char_ms;

                while (sfx_get_period(&dash_sound, audio_buffer, audio_device.frames))
                {
                    audio_write(audio_device.handle, audio_buffer, audio_device.frames);
                }
                dash_sound.cursor = 0;
            }
            else if (BIT_ISSET(pins, DOT_PIN))
            {
                morse_put(&morse, MORSE_DOT);

                end = start + dot_char_ms;

                while (sfx_get_period(&dot_sound, audio_buffer, audio_device.frames))
                {
                    audio_write(audio_device.handle, audio_buffer, audio_device.frames);
                }
                dot_sound.cursor = 0;
            }
        }
    
        delay(1);
    }

    lcd_terminate();
    mgp_terminate();
    audio_terminate(&audio_device);
    sfx_destroy(&dash_sound);
    return 0;
}
