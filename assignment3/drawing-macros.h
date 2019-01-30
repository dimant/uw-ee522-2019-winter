#ifndef DRAWING_MACROS_H
#define DRAWING_MACROS_H

#define X_BIT0 22
#define X_BIT1 10
#define X_BIT2 9
#define X_BIT3 11
#define X_BIT4 0
#define X_BIT5 5
#define X_BIT6 6
#define X_BIT7 13
#define X_BIT8 19
#define X_BIT9 26

#define X_TO_GPIO(x) (((x & 1) << X_BIT0) | ((x & 2) << X_BIT1) | ((x & 4) << X_BIT2) | ((x & 8) << X_BIT3) | ((x & 16) << X_BIT4) | ((x & 32) << X_BIT5) | ((x & 64) << X_BIT6) | ((x & 128) << X_BIT7) | ((x & 256) << X_BIT8) | ((x & 512) << X_BIT9))

#define Y_BIT0 23
#define Y_BIT1 24
#define Y_BIT2 25
#define Y_BIT3 8
#define Y_BIT4 7
#define Y_BIT5 1
#define Y_BIT6 12
#define Y_BIT7 16
#define Y_BIT8 20
#define Y_BIT9 21

#define Y_TO_GPIO(y) (((y & 1) << Y_BIT0) | ((y & 2) << Y_BIT1) | ((y & 4) << Y_BIT2) | ((y & 8) << Y_BIT3) | ((y & 16) << Y_BIT4) | ((y & 32) << Y_BIT5) | ((y & 64) << Y_BIT6) | ((y & 128) << Y_BIT7) | ((y & 256) << Y_BIT8) | ((y & 512) << Y_BIT9))

#define XY_TO_GPIO(x, y) (uint32_t) (X_TO_GPIO(x) | Y_TO_GPIO(y))

#endif
