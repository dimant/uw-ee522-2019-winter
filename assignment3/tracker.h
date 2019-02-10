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
    uint32_t        sampling_rate;
    float           chunks_per_ms;
    track_row_t*    first_row;
    track_row_t*    current_row;
    uint32_t        chunk;
} tracker_state_t;

void tracker_state_create(
    tracker_state_t*    state,
    uint32_t            sampling_rate,
    float               chunks_per_ms,
    track_row_t*        first_row);

void tracker_state_delete(tracker_state_t* state);

track_row_t* tracker_load(const char* fname);

void tracker_get_period(
    tracker_state_t* state,
    float* buffer,
    uint32_t samples);

#endif
