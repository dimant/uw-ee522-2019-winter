#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdint.h>   // for uint32_t - 32-bit unsigned integer
#include <string.h>

#include "memgpio-experiments.h"

/*	cat /proc/iomem ouput:
    00000000-00000000 : /soc/gpio@7e200000 
*/
#define GPIO_BASE 0x3F200000
#define GPIO_OFFSET_PTR(base, x) *(base + (x / sizeof(uint32_t)))

/*	from bcm2835 peripherals doc
    https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
*/
#define GPSET0 0x1c
#define GPSET1 0x20
#define GPCLR0 0x28
#define GPCLR1 0x2c
#define GPLVL0 0x34
#define GPLVL1 0x38

static volatile uint32_t *gpio;
static int fd;

#define FAIL_CODE 1

#define IF_THEN_FAIL(expr, code, message) { if(expr) { fprintf(stderr, message); return code; } }
#define IF_THEN_FAIL_FMT(expr, code, message, ...) { if(expr) { fprintf(stderr, message, ##__VA_ARGS__); return code; } }

int mgp_init()
{
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    IF_THEN_FAIL_FMT(fd < 0, FAIL_CODE, "Unable to open /dev/mem: %s\n", strerror(errno));

    /*  for some reason when cross-compiling from visual studio the preprocessor complains about not finding a prototype.
        the prototype is in unistd.h
    */
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
    int pagesize = getpagesize();
#pragma GCC diagnostic warning "-Wimplicit-function-declaration"

    gpio = (uint32_t *) mmap(0, (size_t) pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    IF_THEN_FAIL_FMT(((uint32_t)gpio) < 0, FAIL_CODE, "Memory mapping failed: %s\n", strerror(errno));

    return 0;
}

void mgp_terminate()
{
    int pagesize = getpagesize();

    /* *gpio is a volatile pointer but munmap's signature accepts a void* */
#pragma GCC diagnostic ignored "-Wcast-qual"
    munmap((void*)gpio, (size_t) pagesize);
#pragma GCC diagnostic warning "-Wcast-qual"
    close(fd);
}

int mgp_blinkLED(unsigned char pin)
{
    int result = mgp_init();
    IF_THEN_FAIL(0 != result, FAIL_CODE, "Failed initializing memgpio library.");

    GPIO_OFFSET_PTR(gpio, GPSET0) = 1 << pin;
    for (int x = 0; x < 500; x++) {}  // blocking delay hack using a simple loop
    GPIO_OFFSET_PTR(gpio, GPCLR0) = 1 << pin;

    mgp_terminate();
    return 0;
}