#pragma once

void ui_manager_init();

void ui_manager_process_mqtt_message(const char* topic, const char* payload);