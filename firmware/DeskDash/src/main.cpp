#include "hardware.h"
#include "lvgl_port.h"
#include "ui.h"
#include <Arduino.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "config.h"
#include "env.h"

void LVGL_Task(void *pvParameters);

void WIFI_Task(void *pvParameters);

void MQTT_Task(void *pvParameters);

void setup() {
  Serial.begin(115200);
  hardware_init();
  lvgl_init();
  ui_init();
  mqtt_manager_init(SUBSCRIBE_TOPIC);

  xTaskCreatePinnedToCore(
    WIFI_Task,
    "WIFI_Task",
    4096,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    LVGL_Task,
    "LVGL_Task",
    4096,
    NULL,
    1,
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    MQTT_Task,
    "MQTT_Task",
    4096,
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {
  vTaskDelete(NULL);
}

void LVGL_Task(void *pvParameters){
  while(true){
    lvgl_handle();
    vTaskDelay(pdMS_TO_TICKS(5));
  }

}

void WIFI_Task(void *pvParameters){
  while(true){
    if(wifi_manager_is_connected()){
      vTaskDelay(pdMS_TO_TICKS(5000));
    } else {
      wifi_manager_init();
    }
  }
}

void MQTT_Task(void *pvParameters){
  TickType_t last_publish = xTaskGetTickCount();
  int counter = 0;
  
  while(true){
    mqtt_manager_handle();

    if((xTaskGetTickCount()-last_publish)> pdMS_TO_TICKS(5000)){
      String dato = "Ciclo numero: "+ String(counter++);
      mqtt_manager_publish(PUBLISH_TOPIC,dato.c_str());
      last_publish = xTaskGetTickCount();
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
