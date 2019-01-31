#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

#define CLEAR_PXL 0
#define DRAW_PXL  1
#define KEEP_PXL  2

uint32_t* draw_init(uint32_t w, uint32_t h);
void draw_free();
uint32_t* draw_swap();
uint32_t* draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
uint32_t* draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
uint32_t* draw_circle(uint32_t x0, uint32_t y0, uint32_t radius);
uint32_t* draw_copy(uint32_t x, uint32_t y, uint32_t* src, uint32_t src_w, uint32_t src_h);

#endif
