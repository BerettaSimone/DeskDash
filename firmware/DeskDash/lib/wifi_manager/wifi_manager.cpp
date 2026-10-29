#include <WiFi.h>
#include "env.h"

void wifi_manager_init(){
    WiFi.mode(WIFI_STA);

    Serial.println("Avvio connessione al WiFi...");
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    
    while(WiFi.status()!= WL_CONNECTED){
        Serial.println("connessione in corso...");
    }

    Serial.print("Connesso!, IP: ");
    Serial.println(WiFi.localIP());
}

bool wifi_manager_is_connected(){
    return WiFi.isConnected();
}

void wifi_manager_disconnect(){
    if(WiFi.isConnected()){
        WiFi.disconnect();
    }
}