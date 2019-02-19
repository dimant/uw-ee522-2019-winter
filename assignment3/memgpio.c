#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "memgpio.h"
#include "assert-macros.h"

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

void mgp_init()
{
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    ASSERT(fd >= 0)

    /*  for some reason when cross-compiling from visual studio the preprocessor complains about not finding a prototype.
        the prototype is in unistd.h
    */

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
    int pagesize = getpagesize();
#pragma GCC diagnostic warning "-Wimplicit-function-declaration"

    gpio = (uint32_t *) mmap(0, (size_t) pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    ASSERT((uint32_t) gpio >= 0)
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

void mgp_set_mode(uint32_t pin, uint32_t mode)
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

void mgp_set_pins(uint32_t bits)
{
    GPIO_OFFSET_PTR(gpio, GPSET0) = bits;
}

void mgp_clr_pins(uint32_t bits)
{
    GPIO_OFFSET_PTR(gpio, GPCLR0) = bits;
}
