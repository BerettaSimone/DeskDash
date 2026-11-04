#pragma once
#include <stdint.h>

//MQTT 
constexpr uint8_t MAX_TOPIC_LEN = 64;
constexpr uint16_t MAX_PAYLOAD_LEN = 256;

constexpr char* SUBSCRIBE_TOPIC = "esp32_test/berry/comandi";
constexpr char* PUBLISH_TOPIC = "esp32_test/berry/dati";

constexpr char* MQTT_BROKER = "broker.hivemq.com";
constexpr uint16_t MQTT_PORT = 1883;