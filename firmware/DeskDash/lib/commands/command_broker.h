#pragma once
#include <Arduino.h>
#include "commands.h"

extern QueueHandle_t ui_inbox;
extern QueueHandle_t wifi_inbox;
extern QueueHandle_t mqtt_inbox;

void execute_all_queued_commands(QueueHandle_t queue);
void queue_command(QueueHandle_t queue, Command &command);
void command_broker_init();