#include <WiFi.h>
#include "env.h"

void wifi_manager_init(){
    WiFi.mode(WIFI_STA);

    Serial.println("WiFi_Manager: Avvio connessione al WiFi...");
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    
    Serial.printf("WiFi_Manager: Connessione a %s in corso...",WIFI_SSID);
    while(WiFi.status()!= WL_CONNECTED){
        delay(500);
    }

    Serial.print("\nWiFi_Manager: Connesso! IP: ");
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