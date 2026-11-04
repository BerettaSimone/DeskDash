#pragma once
#include <stdint.h>

/* ---- MQTT ---- */

// --- dim --- 
constexpr uint8_t MAX_TOPIC_LEN = 64;
constexpr uint16_t MAX_PAYLOAD_LEN = 256;

//--- broker ---
constexpr char* MQTT_BROKER = "broker.hivemq.com";
constexpr uint16_t MQTT_PORT = 1883;

//--- topics ---
constexpr char* WEATHER_TOPIC = "berry/home/pc/weather";
constexpr uint8_t MAX_WEATHER_CODE_STRING_LEN = 10;

constexpr char* SUBSCRIBE_TOPICS[2] = {"esp32_test/berry/comandi", WEATHER_TOPIC};
constexpr uint8_t SUBSCRIBE_TOPICS_LEN = 2;

constexpr char* PUBLISH_TOPIC = "esp32_test/berry/dati";



// ---- others ----

constexpr uint8_t WEATHER_INFO_STR_LEN = 64;