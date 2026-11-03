#include <lvgl.h>
#include <Arduino.h>
#include "ui_manager.h"
#include "commands.h"

static QueueHandle_t mqtt_inbox;
static QueueHandle_t wifi_inbox;

static lv_point_t last_touch_point;
static lv_obj_t * text_area;
static lv_obj_t * label;
static lv_obj_t * connection_message;
static lv_obj_t * button;
static lv_obj_t * button2;
static bool last_wifi_state = false;

void ui_process_mqtt_message(const char* topic, const char* payload){
  String new_line = "Ricevuto: " + String(payload) +"\n";
  lv_textarea_add_text(text_area,new_line.c_str());
}

void ui_set_wifi_state(bool connected){

  if(connected != last_wifi_state){
    last_wifi_state = connected;

    if(connected){
      lv_label_set_text(connection_message,"WiFi: Connesso");
      lv_obj_set_style_text_color(connection_message,lv_palette_main(LV_PALETTE_GREEN),0);
    } else{
      lv_label_set_text(connection_message,"WiFi: Disconnesso");
      lv_obj_set_style_text_color(connection_message,lv_palette_main(LV_PALETTE_RED),0);
    }
  }
}


static void touch_polling_timer_cb(lv_timer_t* timer){
  lv_indev_t * indev = lv_indev_get_next(NULL);
  if(indev == NULL) return;

  lv_point_t p;
  lv_indev_get_point(indev,&p);

  if(last_touch_point.x == p.x && last_touch_point.y == p.y) return;
  String message = "X: "+String(p.x)+" Y: "+String(p.y);

  lv_label_set_text(label,message.c_str());
}

static void btn_event_cb(lv_event_t *e){
  lv_indev_t * indev = lv_indev_get_act();
  if(indev == NULL) return;

  lv_label_set_text(label,"RESET");

  Command cmd = cmd_create_wifi_disconnect();
  xQueueSend(wifi_inbox,&cmd,0);
}

static void btn2_event_cb(lv_event_t *e){
  lv_indev_t * indev = lv_indev_get_act();
  if(indev == NULL) return;

  Command cmd = cmd_create_mqtt_send("esp32_test/berry/dati","Vediamo se funziona così");
  xQueueSend(mqtt_inbox,&cmd,0);
}

static void draw_home_screen(){
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello World!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);
    lv_obj_align(label,LV_ALIGN_TOP_LEFT,10,10);

    button = lv_btn_create(lv_scr_act());
    lv_obj_align(button,LV_ALIGN_TOP_RIGHT,-10,40);
    lv_obj_t * label1 = lv_label_create(button);
    lv_label_set_text(label1,"Disconnetti");

    button2 = lv_btn_create(lv_scr_act());
    lv_obj_align(button2,LV_ALIGN_TOP_LEFT,10,40);
    lv_obj_t * label2 = lv_label_create(button2);
    lv_label_set_text(label2,"Send Message");

    connection_message = lv_label_create(lv_scr_act());
    lv_label_set_text(connection_message,"WiFi: Connetto...");
    lv_obj_align(connection_message,LV_ALIGN_TOP_RIGHT,-10,10);

    text_area = lv_textarea_create(lv_scr_act());
    lv_obj_align(text_area, LV_ALIGN_CENTER,0,30);
    lv_obj_set_width(text_area,lv_pct(90));
    lv_textarea_add_text(text_area,"\tTOPIC: COMANDI\n");
}

void ui_manager_init(QueueHandle_t wifi_queue, QueueHandle_t mqtt_queue){
    wifi_inbox = wifi_queue;
    mqtt_inbox = mqtt_queue;

    draw_home_screen();
    //eventi
    lv_obj_add_event_cb(button,btn_event_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(button2,btn2_event_cb,LV_EVENT_CLICKED,NULL);
    //timer
    lv_timer_create(touch_polling_timer_cb,50,NULL);

}