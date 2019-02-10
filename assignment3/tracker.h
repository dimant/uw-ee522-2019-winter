#ifndef TRACKER_H
#define TRACKER_H

#include "audio.h"

typedef struct track_rows_t
{
    char** cols;
    struct track_rows_t* next;
} track_row_t;


typedef struct tracker_states_t
{
    track_row_t*    first_row;
    track_row_t*    current_row;
    uint32_t        current_period;
} tracker_state_t;

track_row_t* tracker_load(const char* fname);

//void tracker_get_period(
//    tracker_state_t* state,
//    float* buffer,
//    uint32_t samples);

#endif
