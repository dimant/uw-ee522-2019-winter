#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

#define JOY_UP              0
#define JOY_DOWN            1
#define JOY_Y_CENTER        2
#define JOY_LEFT            3
#define JOY_RIGHT           4
#define JOY_X_CENTER        5
#define JOY_A_DOWN          6
#define JOY_A_UP            7
#define JOY_B_DOWN          8
#define JOY_B_UP            9
#define JOY_START_DOWN      10
#define JOY_START_UP        11
#define JOY_SELECT_DOWN     12
#define JOY_SELECT_UP       12

#define JOY_DEV0 "/dev/input/js0"
#define JOY_DEV0 "/dev/input/js0"

typedef struct joystics_t
{
    int fd;
    uint32_t naxes;
    uint32_t nbuttons;
    char* name;
} joystick_t;

void joystick_init(joystick_t* device, const char* name);
uint32_t joystick_read(joystick_t* device);
void joystick_terminate(joystick_t* device);

#endif
