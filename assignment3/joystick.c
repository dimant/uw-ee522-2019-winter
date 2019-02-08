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

void joystick_init(joystick_t* device, const char* name)
{
    ASSERT(device != NULL);

    int result = 0;
    device->fd = open(device, O_RDONLY);
    ASSERT(device->fd > -1);

    result = fcntl(device->fd, F_SETFL, O_NONBLOCK);
    ASSERT(result > -1);

    result = ioctl(device->fd, JSIOCGAXES, &device->naxes);
    ASSERT(result > -1);

    result = ioctl(device->fd, JSIOCGBUTTONS, &device->nbuttons);
    ASSERT(result > -1);

    result = ioctl(device->fd, JSIOCGNAME(80), &device->name);
    ASSERT(result > -1);
}

void joystick_terminate(joystick_t* device)
{
    close(device->fd);

    device->naxes = 0;
    device->nbuttons = 0;
    device->name[0] = '\0';
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
