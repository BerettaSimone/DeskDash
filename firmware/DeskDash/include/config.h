#pragma once
#include <stdint.h>

// DISPLAY TOUCHSCREEN
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

//MQTT 
constexpr uint8_t MAX_TOPIC_LEN = 64;
constexpr uint16_t MAX_PAYLOAD_LEN = 256;

constexpr char* SUBSCRIBE_TOPIC = "esp32_test/berry/comandi";
constexpr char* PUBLISH_TOPIC = "esp32_test/berry/dati";

constexpr char* MQTT_BROKER = "broker.hivemq.com";
constexpr uint16_t MQTT_PORT = 1883;

