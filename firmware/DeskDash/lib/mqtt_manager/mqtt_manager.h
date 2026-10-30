#pragma once

void mqtt_manager_init(const char* subscribe_topic);

void mqtt_manager_handle();

void mqtt_manager_publish(const char* topic, const char* payload);

bool mqtt_manager_subscribe(const String &topic);

