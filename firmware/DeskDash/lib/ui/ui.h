#pragma once

void ui_init();

void ui_process_mqtt_message(const char* topic, const char* payload);