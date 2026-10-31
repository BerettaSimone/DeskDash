#include "hardware.h"
#include "lvgl_port.h"
#include "ui.h"
#include <Arduino.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "config.h"
#include "env.h"

struct Mqtt_Message{
  char topic[MAX_TOPIC_LEN];
  char payload[MAX_PAYLOAD_LEN];
};

QueueHandle_t mqtt_messages = NULL;

void LVGL_Task(void *pvParameters);

void WIFI_Task(void *pvParameters);

void MQTT_Task(void *pvParameters);

void mqtt_message_received_cb(const char* topic, const char* payload){
  Mqtt_Message new_message;
  strlcpy(new_message.topic,topic,MAX_TOPIC_LEN);
  strlcpy(new_message.payload,payload,MAX_PAYLOAD_LEN);

  if(xQueueSend(mqtt_messages,&new_message,0)!=pdTRUE){
    Serial.println("[MAIN] Coda piena, pacchetto MQTT ricevuto e scartato!");
  }
}

void setup() {
  Serial.begin(115200);
  mqtt_messages = xQueueCreate(3, sizeof(Mqtt_Message));

  hardware_init();
  mqtt_manager_init(SUBSCRIBE_TOPIC, mqtt_message_received_cb);

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
    8192,
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
  lvgl_init();
  ui_init();
  Mqtt_Message received_message;
  
  while(true){
    while(xQueueReceive(mqtt_messages,&received_message,0)){
      ui_process_mqtt_message(received_message.topic,received_message.payload);
    }

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
  while(true){
    mqtt_manager_handle();

   TouchData td;
   if(hw_read_touch(td)){
        String dato  = "X: " + String(td.x) + " Y: " + String(td.y);
        mqtt_manager_publish(PUBLISH_TOPIC, dato.c_str());
   }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
