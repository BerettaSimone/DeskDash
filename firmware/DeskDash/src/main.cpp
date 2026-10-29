#include "hardware.h"
#include "lvgl_port.h"
#include "ui.h"
#include <Arduino.h>
#include "wifi_manager.h"

#include "env.h"

void LVGL_Task(void *pvParameters);

void WIFI_Task(void *pvParameters);

void setup() {
  Serial.begin(115200);
  hardware_init();
  lvgl_init();
  ui_init();

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
