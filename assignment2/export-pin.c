#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "export-pin.h"

#define MAX_CMD_LEN 255

void export_pin(unsigned short pin, enum gpio_export_mode_t mode)
{
    char buffer[MAX_CMD_LEN];
    const char* format = "gpio export %i %s";
    const char* out = "out";
    const char* in = "in";
    int result = 0;

    switch (mode)
    {
    case INPUT:
        result = snprintf(buffer, MAX_CMD_LEN, format, pin, in);
        assert(result > 0);
        break;
    case OUTPUT:
        result = snprintf(buffer, MAX_CMD_LEN, format, pin, out);
        assert(result > 0);
        break;
    default:
        break;
    }

    result = system(buffer);
    assert(result == 0);
}