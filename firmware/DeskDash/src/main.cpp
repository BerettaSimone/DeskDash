#include "hardware.h"
#include "lvgl_port.h"
#include "ui_manager.h"
#include <Arduino.h>
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

void mqtt_message_received_cb(const char* topic, const char* payload);


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
  mqtt_manager_init(SUBSCRIBE_TOPIC, mqtt_message_received_cb);

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
  Command cmd = cmd_create_ui_process_mqtt_message(topic,payload);

  if(xQueueSend(ui_inbox,&cmd,0)!=pdTRUE){
    Serial.println("[MAIN] Coda piena, pacchetto MQTT ricevuto e scartato!");
  }
}