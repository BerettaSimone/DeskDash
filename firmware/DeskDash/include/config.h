#pragma once
#include <stdint.h>

constexpr uint8_t DEFAULT_ROTATION = 1;

constexpr uint16_t SCREEN_WIDTH = 320;
constexpr uint16_t SCREEN_HEIGHT = 240;
constexpr uint16_t TOUCH_MIN_VALUE = 200;
constexpr uint16_t TOUCH_MAX_VALUE = 3800;
constexpr uint8_t TOUCH_CS = 33;
constexpr uint8_t TOUCH_INTERRUPT = 36;
constexpr uint8_t TOUCH_CK = 25;
constexpr uint8_t TOUCH_MISO = 39;
constexpr uint8_t TOUCH_MOSI = 32;