#include "commands.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "ui_manager.h"
#include <string.h>


static void exec_wifi_disconnect(void* payload){
    wifi_manager_disconnect();
}

static void exec_mqtt_send(void* payload){
    CommandPayload *p = (CommandPayload*)payload;
    mqtt_manager_publish(p->mqtt_message.topic,p->mqtt_message.payload);
}

static void exec_ui_wifi_update(void* payload) {
    CommandPayload* p = (CommandPayload*)payload;
    ui_set_wifi_state(p->boolean_val);
}

static void exec_ui_show_mqtt_message(void* payload){
    CommandPayload* p = (CommandPayload*)payload;
    ui_log_mqtt_message(p->mqtt_message.topic,p->mqtt_message.payload);
}

static void exec_ui_weather_update(void* payload){
    CommandPayload* p = (CommandPayload*)payload;
    ui_update_weather_info(p->weather_data.info_string);
}




Command cmd_create_wifi_disconnect() {
    Command cmd = {};
    cmd.action = exec_wifi_disconnect;
    return cmd;
}

Command cmd_create_mqtt_send(const char* topic, const char* message) {
    Command cmd = {};
    cmd.action = exec_mqtt_send;
    strlcpy(cmd.payload.mqtt_message.topic, topic, sizeof(cmd.payload.mqtt_message.topic));
    strlcpy(cmd.payload.mqtt_message.payload, message, sizeof(cmd.payload.mqtt_message.payload));
    return cmd;
}

Command cmd_create_ui_wifi_update(bool is_connected) {
    Command cmd = {};
    cmd.action = exec_ui_wifi_update;
    cmd.payload.boolean_val = is_connected;
    return cmd;
}

Command cmd_create_ui_weather_update(const char* weather_info_string){
    Command cmd = {};
    cmd.action = exec_ui_weather_update;
    strlcpy(cmd.payload.weather_data.info_string,weather_info_string,sizeof(cmd.payload.weather_data.info_string));
    return cmd;
}

Command cmd_create_ui_log_mqtt_message(const char* topic, const char* message){
    Command cmd = {};
    cmd.action = exec_ui_show_mqtt_message;
    strlcpy(cmd.payload.mqtt_message.topic, topic, sizeof(cmd.payload.mqtt_message.topic));
    strlcpy(cmd.payload.mqtt_message.payload, message, sizeof(cmd.payload.mqtt_message.payload));
    return cmd;
}