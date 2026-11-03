#pragma once
#include <Arduino.h>
void ui_manager_init(QueueHandle_t wifi_queue, QueueHandle_t mqtt_queue);

void ui_process_mqtt_message(const char* topic, const char* payload);

void ui_set_wifi_state(bool connected);