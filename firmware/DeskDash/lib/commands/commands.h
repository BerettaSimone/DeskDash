#pragma once
#include "config.h"

typedef void(*CommandAction)(void* payload);

struct MqttMessage{
    char topic[MAX_TOPIC_LEN];
    char payload[MAX_PAYLOAD_LEN];
};

union CommandPayload{
    bool boolean_val;
    MqttMessage mqtt_message;
};

struct Command{
    CommandAction action;
    CommandPayload payload;
};

Command cmd_create_wifi_disconnect();
Command cmd_create_mqtt_send(const char* topic, const char* message);
Command cmd_create_ui_wifi_update(bool is_connected);
Command cmd_create_ui_process_mqtt_message(const char* topic, const char* message);
