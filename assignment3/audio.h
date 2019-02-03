#ifndef AUDIO_H
#define AUDIO_H

#define SAMPLING_RATE 48000
#define CHANNELS 2

void audio_init();
void audio_terminate();
float* audio_buffer();
void audio_write();

#endif
