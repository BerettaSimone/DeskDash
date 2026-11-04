#include <WiFi.h>
#include "time.h"
#include "wifi_manager.h"
#include "commands.h"

static QueueHandle_t ui_inbox;
static WiFi_Mode mode;
static char* last_ssid;
static char* last_password;

const char* POSIX_TZ = "CET-1CEST,M3.5.0/2,M10.5.0/3";
void get_time();

void wifi_manager_init(QueueHandle_t ui_queue){
    ui_inbox = ui_queue;
    WiFi.mode(WIFI_AP);
    mode = ACCESS_POINT;
}

void wifi_manager_init(QueueHandle_t ui_queue, char* ssid, char* password){
    ui_inbox = ui_queue;
    WiFi.mode(WIFI_STA);
    mode = STATION;
    last_ssid = ssid;
    last_password = password;
}

void wifi_manager_connect(char* ssid, char* password){
    if(mode != STATION){
        Serial.println("[WIFI_MANAGER]: errore, il wifi non è in modalità STATION");
    }

    Serial.printf("[WIFI_MANAGER]: Avvio connessione a %s...\n",ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);
    
    Serial.printf("WiFi_Manager: Connessione a %s in corso...",ssid);

    uint8_t attempts=0;

    while(WiFi.status()!= WL_CONNECTED && attempts<10){
        vTaskDelay(pdMS_TO_TICKS(1000));
        attempts++;
    }

    if(attempts>=10){
        Serial.printf("Connessione a %s fallita, attivo modalità ACCESS_POINT",ssid);
        WiFi.mode(WIFI_AP);
        return;
    } 

    last_ssid = ssid;
    last_password = password;

    Command cmd = cmd_create_ui_wifi_update(true);
    xQueueSend(ui_inbox,&cmd,0);
    get_time();
    Serial.print("\nWiFi_Manager: Connesso! IP: ");
    Serial.println(WiFi.localIP());
}

void wifi_manager_handle(){
    if(mode == STATION){
        if(wifi_manager_is_connected()){;
            vTaskDelay(pdMS_TO_TICKS(5000));
        } else {
            Command cmd = cmd_create_ui_wifi_update(false);
            xQueueSend(ui_inbox,&cmd,0);
            wifi_manager_connect(last_ssid, last_password);
        }
    }

    if(mode == ACCESS_POINT){
        Serial.println("[WIFI_MANAGER]: ATTENZIONE, MODALITA' NON CONFIGURATA!!");
    }
}

bool wifi_manager_is_connected(){
    return WiFi.isConnected();
}

void wifi_manager_disconnect(){
    if(WiFi.isConnected()){
        WiFi.disconnect();
    }
}



void get_time(){
    setenv("TZ",POSIX_TZ,1);
    tzset();

    setlocale(LC_TIME, "it_IT.UTF-8");
    configTzTime(POSIX_TZ,"pool.ntp.org","time.nist.gov");
}


