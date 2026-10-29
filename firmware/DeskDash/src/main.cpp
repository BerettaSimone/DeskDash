#include <hardware.h>
#include <lvgl_port.h>
#include <ui.h>

#include <Arduino.h>

void LVGL_Task(void *pvParameters);


void setup() {
  Serial.begin(115200);
  hardware_init();
  lvgl_init();
  ui_init();

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
