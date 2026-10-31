#pragma once

#include <stdint.h>

struct TouchData {
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

void hardware_init();

void hw_draw_image(int x, int y, int w, int h, uint16_t* color_data);
bool hw_read_touch(TouchData &data);
