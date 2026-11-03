#pragma once
#include <Arduino.h>

enum WiFi_Mode{
    STATION,
    ACCESS_POINT
};

void wifi_manager_init(QueueHandle_t ui_queue);

void wifi_manager_init(QueueHandle_t ui_queue, char* ssid, char* password);

void wifi_manager_connect(char* ssid, char* password);

bool wifi_manager_is_connected();

void wifi_manager_disconnect();

void wifi_manager_handle();