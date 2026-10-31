#pragma once

typedef void(*mqtt_on_message_cb)(const char* topic, const char* payload);

void mqtt_manager_init(const char* subscribe_topic, mqtt_on_message_cb on_message_cb);

void mqtt_manager_handle();

void mqtt_manager_publish(const char* topic, const char* payload);

bool mqtt_manager_subscribe(const String &topic);

char* mqtt_manager_get_payload();

bool mqtt_manager_is_connected();