#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "memgpio.h"
#include "error-macros.h"
#include "drawing-macros.h"

/*	cat /proc/iomem ouput:
    00000000-00000000 : /soc/gpio@7e200000 */
#define GPIO_BASE 0x3F200000
#define GPIO_OFFSET_PTR(base, x) *(base + (x / sizeof(uint32_t)))

/*	from bcm2835 peripherals doc
    https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf */
#define GPSET0 0x1c
#define GPSET1 0x20
#define GPCLR0 0x28
#define GPCLR1 0x2c
#define GPLVL0 0x34
#define GPLVL1 0x38

static volatile uint32_t *gpio;
static int fd;

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

void mgp_setMode(uint32_t pin, uint32_t mode)
{
	// per the doc (page 91, Table 6.2 and following), pins are occupying registries of 32 bit
	// 10 pins per register, 3 bits per pin, remaining 2 bits are reserved
	// https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
    uint32_t reg = pin / 10u;
    uint32_t shift = (pin % 10u) * 3u;
    uint32_t clr_mask = 7;
    uint32_t mode_mask = mode;
 	
	// we first clear the 3 bits corresponding to the pin
	// then we apply the requested mode
	// finally we copy back to the register
	gpio[reg] = (gpio[reg] & ~(clr_mask <<shift)) | (mode_mask <<shift);
}

void mgp_xy_set(uint32_t x, uint32_t y)
{
    GPIO_OFFSET_PTR(gpio, GPSET0) = XY_TO_GPIO(x, y);
}

void mgp_bits_set(uint32_t bits)
{
    GPIO_OFFSET_PTR(gpio, GPSET0) = bits;
}

void mgp_bits_clr(uint32_t bits)
{
    GPIO_OFFSET_PTR(gpio, GPCLR0) = bits;
}

int mgp_blinkLED(char pin)
{
    int result = mgp_init();
    IF_THEN_FAIL(0 != result, FAIL_CODE, "Failed initializing memgpio library.");

    GPIO_OFFSET_PTR(gpio, GPSET0) = (uint32_t)(1 << pin);
    for (int x = 0; x < 500; x++) {}  // blocking delay hack using a simple loop
    GPIO_OFFSET_PTR(gpio, GPCLR0) = (uint32_t)(1 << pin);

    mgp_terminate();
    return 0;
}
