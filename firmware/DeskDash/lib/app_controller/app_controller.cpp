#include "app_controller.h"
#include "commands.h"
#include "command_broker.h"
#include <ArduinoJson.h>

static void mqtt_message_received_weather_topic(const char* payload, char* formatted_string, size_t out_len);

void mqtt_message_received_cb(const char* topic, const char* payload){
  Command log_message = cmd_create_ui_log_mqtt_message(topic,payload);
  queue_command(ui_inbox,log_message);

  if(strcmp(topic,WEATHER_TOPIC)==0){
    char weather_info[WEATHER_INFO_STR_LEN];

    mqtt_message_received_weather_topic(payload,weather_info,WEATHER_INFO_STR_LEN);
    Command weather_update = cmd_create_ui_weather_update(weather_info);
    queue_command(ui_inbox,weather_update);
  }
}


void mqtt_message_received_weather_topic(const char* payload, char* formatted_string, size_t out_len){
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc,payload,MAX_PAYLOAD_LEN);
  if(error){
    Serial.println("[MAIN] Errore deserializzazione JSON");
    strlcpy(formatted_string, "Errore JSON", out_len); 
    return;
  }
    
  float tmp = doc["tmp"]|0.0f;
  const char* w_code_string = doc["code_string"] | "N/D";
  int tmp_rounded = (int)round(tmp);
  snprintf(formatted_string,out_len,"%d°C - %s",tmp_rounded,w_code_string);
  return;
}
