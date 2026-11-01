// #include "commands.h"
// #include <Arduino.h>
// #include "config.h"


// void WIFI_DISCONNECT(){

// }

// QueueHandle_t cores[NUMBER_OF_CORES];

// void command_handler_init(){
//     for(int i = 0; i<NUMBER_OF_CORES; i++){
//         cores[i] = xQueueCreate(10,sizeof(Command));
//     }
// }


// void queue_for_execution(Command command, uint8_t core){
//     if(core >= NUMBER_OF_CORES){
//         throw "ERRORE";
//         return;
//     }
//     QueueHandle_t core_queue = cores[core];
//     xQueueSend(core_queue,&command,0);

// }

// void execute_all_commands(uint8_t core){
//     if(core >= NUMBER_OF_CORES){
//         throw "ERRORE";
//         return;
//     }

//     Command command;
//     QueueHandle_t core_queue = cores[core];
//     while(xQueueReceive(core_queue,&command,0)){
//         command.cmd.cmd(command.params);
//     }

// }