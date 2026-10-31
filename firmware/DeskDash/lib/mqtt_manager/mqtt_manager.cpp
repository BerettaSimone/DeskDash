#include <MQTT.h>
#include "mqtt_manager.h"
#include "wifi_manager.h"
#include <WiFi.h>
#include "config.h"

static MQTTClient mqtt_client;
static WiFiClient net;

static mqtt_on_message_cb on_message_cb = nullptr;
static const char* saved_topic = nullptr;

void messageReceived(String &topic, String &payload){
    Serial.printf("[MQTT] Ricevuto su %s: %s\n",topic.c_str(),payload.c_str());
    if(on_message_cb != nullptr){
        on_message_cb(topic.c_str(),payload.c_str());
    }
};

void mqtt_manager_init(const char* subscribe_topic, mqtt_on_message_cb callback){
    saved_topic = subscribe_topic;
    on_message_cb = callback;
    mqtt_client.begin(MQTT_BROKER, MQTT_PORT, net);
    mqtt_client.onMessage(messageReceived);
}

void mqtt_manager_handle(){
    mqtt_client.loop();
    delay(10);

    if(!mqtt_client.connected()){
        Serial.print("MQTT_Manager: tentativo di connessione...");

        String clientId = "ESP32_Test_"+String(random(0xffff),HEX);

        if(mqtt_client.connect(clientId.c_str())){
            Serial.println(" CONNESSO!");
            mqtt_client.subscribe(saved_topic);
        } else {
            Serial.println(" !Fallito.");
        }
    }

}

void mqtt_manager_publish(const char* topic, const char* payload){
    if(mqtt_client.connected()){
        mqtt_client.publish(topic,payload);
        Serial.printf("[MQTT] Pubblicato su %s: %s\n",topic,payload);
    } else{
        Serial.println("[MQTT] ! Errore: impossibile pubblicare, client disconnesso.");
    }
}

bool mqtt_manager_subscribe(const String &topic){
    if(mqtt_client.connected()){
        mqtt_client.subscribe(topic);
        Serial.printf("[MQTT] Iscritto al topic: %s \n",topic.c_str());
        return true;
    } else{
        Serial.println("[MQTT] ! Errore: impossibile iscriversi, client disconnesso.");
        return false;
    }
}

bool mqtt_manager_is_connected(){
    return mqtt_client.connected();
}