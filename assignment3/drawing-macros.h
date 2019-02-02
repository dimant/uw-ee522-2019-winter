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

#define BIT_ISSET(var, pos) !!((var) & (1 << (pos)))

#define X_TO_GPIO(x) \
    ((BIT_ISSET(x, 0) << X_BIT0) | \
    ( BIT_ISSET(x, 1) << X_BIT1) | \
    ( BIT_ISSET(x, 2) << X_BIT2) | \
    ( BIT_ISSET(x, 3) << X_BIT3) | \
    ( BIT_ISSET(x, 4) << X_BIT4) | \
    ( BIT_ISSET(x, 5) << X_BIT5) | \
    ( BIT_ISSET(x, 6) << X_BIT6) | \
    ( BIT_ISSET(x, 7) << X_BIT7) | \
    ( BIT_ISSET(x, 8) << X_BIT8) | \
    ( BIT_ISSET(x, 9) << X_BIT9))

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

#define Y_TO_GPIO(x) \
    ((BIT_ISSET(x, 0) << Y_BIT0) | \
    ( BIT_ISSET(x, 1) << Y_BIT1) | \
    ( BIT_ISSET(x, 2) << Y_BIT2) | \
    ( BIT_ISSET(x, 3) << Y_BIT3) | \
    ( BIT_ISSET(x, 4) << Y_BIT4) | \
    ( BIT_ISSET(x, 5) << Y_BIT5) | \
    ( BIT_ISSET(x, 6) << Y_BIT6) | \
    ( BIT_ISSET(x, 7) << Y_BIT7) | \
    ( BIT_ISSET(x, 8) << Y_BIT8) | \
    ( BIT_ISSET(x, 9) << Y_BIT9))

#define XY_TO_GPIO(x, y) (uint32_t) (X_TO_GPIO(x) | Y_TO_GPIO(y))

#endif
