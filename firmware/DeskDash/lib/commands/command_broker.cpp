#include "command_broker.h"

//coda di comandi in ingresso per ogni task
QueueHandle_t ui_inbox;
QueueHandle_t wifi_inbox;
QueueHandle_t mqtt_inbox;

void command_broker_init(){
  mqtt_inbox = xQueueCreate(5,sizeof(Command));
  wifi_inbox = xQueueCreate(5,sizeof(Command));
  ui_inbox = xQueueCreate(5,sizeof(Command));
}

void execute_all_queued_commands(QueueHandle_t queue){
  Command cmd;  
  while(xQueueReceive(queue,&cmd,0)){
      if(cmd.action!=nullptr){
        cmd.action(&cmd.payload);
      }
    }
}

void queue_command(QueueHandle_t queue, Command &command){
  if(xQueueSend(queue,&command,0)!=pdTRUE){
    Serial.println("[MAIN] Coda piena, comando scartato!");
  }
}