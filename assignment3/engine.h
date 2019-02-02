#ifndef ENGINE_H
#define ENGINE_H

uint32_t eng_collide(uint32_t x, uint32_t y, uint32_t* src, uint32_t src_w, uint32_t src_h);
void eng_render(uint32_t* buffer);
void eng_step();
void eng_start(uint32_t* buffer);
#endif
