#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/joystick.h>

#include "joystick.h"
#include "type-macros.h"

#define JOY_AXIS_X          0
#define JOY_AXIS_Y          1
#define JOY_BUTTON_A        1
#define JOY_BUTTON_B        0
#define JOY_BUTTON_START    8
#define JOY_BUTTON_SELECT   9

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
    uint32_t* axes;
    uint32_t naxes;
    uint32_t* buttons;
    uint32_t nbuttons;
    char* name;
} joystick_t;

void joystick_init(joystick_t* joystick, const char* device)
{
    int result = 0;
    joystick->fd = open(device, O_RDONLY);
    ASSERT(joystick->fd > -1);

    result = fcntl(joystick->fd, F_SETFL, O_NONBLOCK);
    ASSERT(result > -1);

    result = ioctl(joystick->fd, JSIOCGAXES, &joystick->naxes);
    ASSERT(result > -1);

    result = ioctl(joystick->fd, JSIOCGBUTTONS, &joystick->nbuttons);
    ASSERT(result > -1);

    result = ioctl(joystick->fd, JSIOCGNAME(80), &joystick->name);
    ASSERT(result > -1);

    joystick->axes = (int *)calloc(joystick->naxes, sizeof(int));
    ASSERT(joystick->axes != NULL);

    joystick->buttons = (short int *)calloc(joystick->nbuttons, sizeof(char));
    ASSERT(joystick->buttons != NULL);
}

void joystick_terminate(joystick_t* joystick)
{
    close(joystick->fd);

    free(joystick->axes);
    joystick->naxes = 0;

    free(joystick->buttons);
    joystick->buttons = 0;

    joystick->name[0] = '\0';
}

uint32_t joystick_read(joystick_t* joystick)
{
    int result = 0;
    struct js_event js;

    result = read(joystick->fd, &js, sizeof(struct js_event));
    ASSERT(result > -1);

    switch (js.type & ~JS_EVENT_INIT)
    {
        case JS_EVENT_AXIS:
            if (js.number == JOY_AXIS_Y && js.value == 0)
            {
                return JOY_Y_CENTER;
            }
            if (js.number == JOY_AXIS_Y && js.value < 0)
            {
                return JOY_UP;
            }
            if (js.number == JOY_AXIS_Y && js.value > 0)
            {
                return JOY_DOWN;
            }
            if (js.number == JOY_AXIS_X && js.value == 0)
            {
                return JOY_X_CENTER;
            }
            if (js.number == JOY_AXIS_X && js.value > 0)
            {
                return JOY_RIGHT;
            }
            if (js.number == JOY_AXIS_X && js.value < 0)
            {
                return JOY_LEFT;
            }
            break;
        case JS_EVENT_BUTTON:
            if (js.number == JOY_BUTTON_A && js.value > 0)
            {
                return JOY_A_DOWN;
            }
            if (js.number == JOY_BUTTON_A && js.value == 0)
            {
                return JOY_A_UP;
            }
            if (js.number == JOY_BUTTON_B && js.value > 0)
            {
                return JOY_B_DOWN;
            }
            if (js.number == JOY_BUTTON_B && js.value == 0)
            {
                return JOY_B_UP;
            }
            if (js.number == JOY_BUTTON_START && js.value > 0)
            {
                return JOY_START_DOWN;
            }
            if (js.number == JOY_BUTTON_START && js.value == 0)
            {
                return JOY_START_UP;
            }
            if (js.number == JOY_BUTTON_SELECT && js.value > 0)
            {
                return JOY_SELECT_DOWN;
            }
            if (js.number == JOY_BUTTON_SELECT && js.value == 0)
            {
                return JOY_SELECT_UP;
            }
            break;
        default:
            ASSERT(FALSE);
    }
}
