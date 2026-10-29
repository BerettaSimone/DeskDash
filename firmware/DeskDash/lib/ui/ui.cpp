#include "ui.h"
#include <lvgl.h>
#include <Arduino.h>
#include "wifi_manager.h"

static lv_obj_t * label;
static lv_obj_t * connection_message;
static bool last_wifi_state = false;

void wifi_status_timer_cb(lv_timer_t * timer){
  bool current_state = wifi_manager_is_connected();

  if(current_state != last_wifi_state){
    last_wifi_state = current_state;

    if(current_state){
      lv_label_set_text(connection_message,"WiFi: Connesso");
      lv_obj_set_style_text_color(connection_message,lv_palette_main(LV_PALETTE_GREEN),0);
    } else{
      lv_label_set_text(connection_message,"WiFi: Disconnesso");
      lv_obj_set_style_text_color(connection_message,lv_palette_main(LV_PALETTE_RED),0);
    }
  }
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
  wifi_manager_disconnect();
}


void ui_init(){
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello World!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);
    lv_obj_center(label);

    static lv_obj_t * button = lv_btn_create(lv_scr_act());
    lv_obj_center(button);
    lv_obj_set_pos(button,0,50);
    lv_obj_t * label1 = lv_label_create(button);
    lv_label_set_text(label1,"Clicca qui");

    connection_message = lv_label_create(lv_scr_act());
    lv_label_set_text(connection_message,"WiFi: Connetto...");
    lv_obj_align(connection_message,LV_ALIGN_TOP_RIGHT,-10,10);

    //eventi
    lv_obj_add_event_cb(lv_scr_act(),screen_event_cb,LV_EVENT_PRESSING,NULL);
    lv_obj_add_event_cb(button,btn_event_cb,LV_EVENT_CLICKED,NULL);

    //timer
    lv_timer_create(wifi_status_timer_cb,1000,NULL);
}