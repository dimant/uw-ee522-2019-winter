#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tracker.h"
#include "audio.h"
#include "assert-macros.h"
#include "type-macros.h"

#define MAX_LINE    255
#define MAX_COL     10
#define MAX_TRACKS  8
#define MAX_NOTES   36

#define DELIM       ','

#define COL_FORM        0
#define COL_CHANNEL     1
#define COL_DURATION    2
#define COL_NOTE        3
#define COL_DUTY        4

#define SAMPLES(d, s) (uint32_t)((uint32_t)d * (uint32_t)s/(uint32_t)1000)

// Track
// 
// Form         Channel     Duration    Note    Duty Cycle
// ---------------------------------------------------------
// saw          0           500ms       C2-B2
// triangle     1                       C3-B3
// pulse                                C4-B4   0-100
// sin
// noise

// add
// mul

// Notes:
// ---------------------
//   0     3 C 0    131
//   1     3 C 1    139
//   2     3 D 0    147
//   3     3 D 1    155
//   4     3 E 0    165
//   5     3 F 0    175
//   6     3 F 1    185
//   7     3 G 0    196 
//   8     3 G 1    208
//   9     3 A 0    220
//  10     3 A 1    233
//  11     3 B 0    247
//  12     4 C 0    262
//  13     4 C 1    277
//  14     4 D 0    294
//  15     4 D 1    311
//  16     4 E 0    330
//  17     4 F 0    349
//  18     4 F 1    370
//  19     4 G 0    392
//  20     4 G 1    415
//  21     4 A 0    440
//  22     4 A 1    466
//  23     4 B 0    494
//  24     5 C 0    523
//  25     5 C 1    554
//  26     5 D 0    587
//  27     5 D 1    622
//  28     5 E 0    659
//  29     5 F 0    698
//  30     5 F 1    739
//  31     5 G 0    783
//  32     5 G 1    830
//  33     5 A 0    880
//  34     5 A 1    932
//  35     5 B 0    987

const char* notes[] = {
    "3C0",
    "3C1",
    "3D0",
    "3D1",
    "3E0",
    "3F0",
    "3F1",
    "3G0", 
    "3G1",
    "3A0",
    "3A1",
    "3B0",
    "4C0",
    "4C1",
    "4D0",
    "4D1",
    "4E0",
    "4F0",
    "4F1",
    "4G0",
    "4G1",
    "4A0",
    "4A1",
    "4B0",
    "5C0",
    "5C1",
    "5D0",
    "5D1",
    "5E0",
    "5F0",
    "5F1",
    "5G0",
    "5G1",
    "5A0",
    "5A1",
    "5B0"
};

const uint32_t freqs[] = {
    131,
    139,
    147,
    155,
    165,
    175,
    185,
    196 ,
    208,
    220,
    233,
    247,
    262,
    277,
    294,
    311,
    330,
    349,
    370,
    392,
    415,
    440,
    466,
    494,
    523,
    554,
    587,
    622,
    659,
    698,
    739,
    783,
    830,
    880,
    932,
    987
};

track_row_t* create_row()
{
    track_row_t* row = (track_row_t*) malloc(sizeof(row));
    row->cols = (char**) malloc(sizeof(char*) * MAX_COL);

    for(uint32_t i = 0; i < MAX_COL; i++)
    {
        row->cols[i] = (char*) malloc(sizeof(char) * MAX_LINE);
    }

    row->next = NULL;

    return row;
}

void destroy_row(track_row_t* row)
{
    for(uint32_t i = 0; i < MAX_COL; i++)
    {
        free(row->cols[i]);
    }

    free(row->cols);
    free(row);
}


FILE* tracker_open(const char* fname)
{
    FILE* file = fopen(fname, "r");

    ASSERT(NULL != file);

    return file;
}

void tracker_close(FILE* file)
{
    int result = fclose(file);

    ASSERT(0 == result);
}

track_row_t* tracker_tokenize(char* string, const char delim)
{
    char* cursor = string;
    track_row_t* row = create_row();
    uint32_t col = 0;
    uint32_t i = 0;

    while(cursor - string < MAX_LINE)
    {
        if(*cursor == delim)
        {
            row->cols[col][i] = '\0';
            i = 0;
            col++;
        }
        else
        {
            row->cols[col][i] = *cursor;
            i++;
        }

        cursor++;
    } 

    return row;
}

track_row_t* tracker_load(const char* fname)
{
    FILE* file = tracker_open(fname);

    char string[MAX_LINE];
    char* status = NULL;

    track_row_t* first_row = create_row();
    track_row_t* cur_row = first_row;
    uint32_t len;
    
    while(1)
    {
        status = fgets(string, MAX_LINE, file);
        len = (uint32_t) strnlen(string, MAX_LINE);

        if(string[len - 1] == '\n')
        {
            string[len - 1] = '\0';
        }

        if(string[len - 2] == '\r')
        {
            string[len - 2] = '\0';
        }

        if(NULL == status)
        {
            break;
        }
        
        cur_row->next = tracker_tokenize(string, DELIM);
        cur_row = cur_row->next;
    }

    tracker_close(file);

    destroy_row(first_row);

    return first_row->next;
}

uint32_t note_to_idx(char* note)
{
    uint32_t idx = 0;

    for(idx = 0; idx < MAX_NOTES; idx++)
    {
        if(0 == strcmp(notes[idx], note))
            return idx;
    }

    ASSERT(FALSE);
    return 0;
}

void tracker_play(audio_device_t* device, track_row_t* track)
{
    audio_device_t buffer_device;
    buffer_device.channels = device->channels;
    buffer_device.sampling_rate = device->sampling_rate;
    buffer_device.buffer = (float*)malloc(sizeof(float) * device->sampling_rate);;

    for(track_row_t* cur = track; cur != NULL; cur = cur->next)
    {
        uint32_t channel = (uint32_t) atoi(cur->cols[COL_CHANNEL]);
        uint32_t samples = SAMPLES((uint32_t) atoi(cur->cols[COL_DURATION]), device->sampling_rate);
        uint32_t freq = freqs[note_to_idx(cur->cols[COL_NOTE])];

        if(0 == strcmp("saw", cur->cols[COL_FORM]))
        {
            audio_saw(&buffer_device, channel, samples, freq); 
        }
        else if(0 == strcmp("triangle", cur->cols[COL_FORM]))
        {
            audio_triangle(&buffer_device, channel, samples, freq); 
        }
        else if(0 == strcmp("pulse", cur->cols[COL_FORM]))
        {
            uint32_t duty = (uint32_t) atoi(cur->cols[COL_DUTY]);
            audio_pulse(&buffer_device, channel, samples, freq, duty); 
        }
        else if(0 == strcmp("sin", cur->cols[COL_FORM]))
        {
            audio_sin(&buffer_device, channel, samples, freq); 
        }
        else if(0 == strcmp("noise", cur->cols[COL_FORM]))
        {
            audio_noise(&buffer_device, channel, samples); 
        }
    }

    free(buffer_device.buffer);
}
