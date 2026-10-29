#include <hardware.h>
#include <lvgl_port.h>
#include <ui.h>

#include <Arduino.h>


void setup() {
  Serial.begin(115200);
  hardware_init();
  lvgl_init();
  ui_init();
}

void loop() {
  lvgl_handle();
  delay(5);
}
