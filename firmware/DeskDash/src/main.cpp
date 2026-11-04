#include "hardware.h"
#include "lvgl_port.h"
#include "ui_manager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "config.h"
#include "commands.h"
#include "env.h"

//coda di comandi in ingresso per ogni task
QueueHandle_t ui_inbox;
QueueHandle_t wifi_inbox;
QueueHandle_t mqtt_inbox;
void execute_all_queued_commands(QueueHandle_t queue);

//tasks 
void LVGL_Task(void *pvParameters);
void WIFI_Task(void *pvParameters);
void MQTT_Task(void *pvParameters);

//funzioni
void mqtt_message_received_cb(const char* topic, const char* payload);
void mqtt_message_received_weather_topic(const char* payload, char* formatted_string, size_t out_len);

void queue_command(QueueHandle_t queue, Command &command);


void setup() {
  Serial.begin(115200);

  mqtt_inbox = xQueueCreate(5,sizeof(Command));
  wifi_inbox = xQueueCreate(5,sizeof(Command));
  ui_inbox = xQueueCreate(5,sizeof(Command));

  hardware_init();

  xTaskCreatePinnedToCore(WIFI_Task,"WIFI_Task",4096,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(LVGL_Task,"LVGL_Task",8192,NULL,1,NULL,1);
  xTaskCreatePinnedToCore(MQTT_Task,"MQTT_Task",4096,NULL,1,NULL,0);
}

void loop() {
  vTaskDelete(NULL);
}

void LVGL_Task(void *pvParameters){
  lvgl_init();
  ui_manager_init(wifi_inbox,mqtt_inbox);

  while(true){
    execute_all_queued_commands(ui_inbox);
    lvgl_handle();
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void WIFI_Task(void *pvParameters){
  wifi_manager_init(ui_inbox, WIFI_SSID, WIFI_PASSWORD);

  while(true){
    execute_all_queued_commands(wifi_inbox);
    wifi_manager_handle();
  }
}

void MQTT_Task(void *pvParameters){
  mqtt_manager_init(mqtt_message_received_cb);

  while(true){
    execute_all_queued_commands(mqtt_inbox);
    mqtt_manager_handle();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}


void execute_all_queued_commands(QueueHandle_t queue){
  Command cmd;  
  while(xQueueReceive(queue,&cmd,0)){
      if(cmd.action!=nullptr){
        cmd.action(&cmd.payload);
      }
    }
}

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


void queue_command(QueueHandle_t queue, Command &command){
  if(xQueueSend(queue,&command,0)!=pdTRUE){
    Serial.println("[MAIN] Coda piena, comando scartato!");
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
  const char* w_code_string = doc["code_string"];
  int tmp_rounded = (int)round(tmp);
  snprintf(formatted_string,out_len,"%d°C - %s",tmp_rounded,w_code_string);
  return;
}
