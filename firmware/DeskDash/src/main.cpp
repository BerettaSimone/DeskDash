#include "hardware.h"
#include "lvgl_port.h"
#include "ui_manager.h"
#include <Arduino.h>
#include "app_controller.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "command_broker.h"
#include "env.h"


//tasks 
void LVGL_Task(void *pvParameters);
void WIFI_Task(void *pvParameters);
void MQTT_Task(void *pvParameters);

void setup() {
  Serial.begin(115200);

  hardware_init();
  command_broker_init();

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
    vTaskDelay(pdMS_TO_TICKS(50));
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









