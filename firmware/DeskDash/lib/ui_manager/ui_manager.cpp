#include "ui_manager.h"
#include <lvgl.h>
#include <Arduino.h>
#include "commands.h"

static lv_obj_t * text_area;
static lv_obj_t * label;
static lv_obj_t * connection_message;
static bool last_wifi_state = false;

void ui_manager_process_mqtt_message(const char* topic, const char* payload){
  String new_line = "Ricevuto: " + String(payload) +"\n";
  lv_textarea_add_text(text_area,new_line.c_str());
}


void screen_event_cb(lv_event_t *e){
  lv_indev_t * indev = lv_indev_get_act();
  if(indev == NULL) return;

  lv_point_t p;
  lv_indev_get_point(indev,&p);
  String message = "X: "+String(p.x)+" Y: "+String(p.y);
  lv_label_set_text(label,message.c_str());
}

void btn_event_cb(lv_event_t *e){
  lv_indev_t * indev = lv_indev_get_act();
  if(indev == NULL) return;

  lv_label_set_text(label,"RESET");
  
  // Command c;
  // union Command_Id command_Id;
  // command_Id.cmd = MQTT_SEND;
  // c.cmd = command_Id;

  // queue_for_execution(c,0);
}


void ui_manager_init(){
    // label = lv_label_create(lv_scr_act());
    // lv_label_set_text(label, "Hello World!");
    // lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);
    // lv_obj_align(label,LV_ALIGN_TOP_LEFT,10,10);

    // static lv_obj_t * button = lv_btn_create(lv_scr_act());
    // lv_obj_align(button,LV_ALIGN_TOP_RIGHT,-10,40);
    // lv_obj_t * label1 = lv_label_create(button);
    // lv_label_set_text(label1,"Disconnetti");

    // connection_message = lv_label_create(lv_scr_act());
    // lv_label_set_text(connection_message,"WiFi: Connetto...");
    // lv_obj_align(connection_message,LV_ALIGN_TOP_RIGHT,-10,10);

    // text_area = lv_textarea_create(lv_scr_act());
    // lv_obj_align(text_area, LV_ALIGN_CENTER,0,30);
    // lv_obj_set_width(text_area,lv_pct(90));
    // lv_textarea_add_text(text_area,"\tTOPIC: COMANDI\n");
    // //eventi
    // lv_obj_add_event_cb(lv_scr_act(),screen_event_cb,LV_EVENT_PRESSING,NULL);
    // lv_obj_add_event_cb(button,btn_event_cb,LV_EVENT_CLICKED,NULL);

}

void ui_manager_update_wifi_state(bool actual_state){
  if(actual_state == last_wifi_state){
    return;
  }
  last_wifi_state = actual_state;

  if(actual_state){
    lv_label_set_text(connection_message,"WiFi: Connesso");
  } else {
    lv_label_set_text(connection_message,"WiFi: Disconnesso");
  }
}