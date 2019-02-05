#ifndef TRACKER_H
#define TRACKER_H

#include "audio.h"

typedef struct track_rows_t
{
    char** cols;
    struct track_rows_t* next;
} track_row_t;

track_row_t* tracker_load(const char* fname);
void tracker_play(audio_device_t* device, track_row_t* track);

#endif
