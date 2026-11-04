#include <lvgl.h>
#include <Arduino.h>
#include "ui_manager.h"
#include "ui.h"
#include "commands.h"

static QueueHandle_t mqtt_inbox;
static QueueHandle_t wifi_inbox;
static bool last_wifi_state = false;
lv_point_t last_touch_point;


static const char* days[] = {"Domenica","Lunedi","Martedi","Mercoledi","Giovedi","Venerdi","Sabato"};
static const char * months[] = {
    "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", 
    "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"
};

static uint8_t last_hour = 254;
static uint8_t last_min = 254;
static uint8_t last_sec = 254;
static uint8_t last_day = 254;
static uint8_t last_month = 254;

/*
  GUI UPDATE 
*/
void ui_process_mqtt_message(const char* topic, const char* payload){
  String new_line = "Ricevuto: " + String(payload) +"\n";
  lv_obj_t * log = lv_label_create(ui_logPanel);
  lv_label_set_text(log,new_line.c_str());
}

void ui_set_wifi_state(bool connected){

  if(connected != last_wifi_state){
    last_wifi_state = connected;

    if(connected){
      lv_label_set_text(ui_wifiLabel,"WiFi: Connesso");
      lv_obj_set_style_text_color(ui_wifiLabel,lv_palette_main(LV_PALETTE_GREEN),0);
    } else{
      lv_label_set_text(ui_wifiLabel,"WiFi: Disconnesso");
      lv_obj_set_style_text_color(ui_wifiLabel,lv_palette_main(LV_PALETTE_RED),0);
    }
  }
}

/*
  TIMER CALLBACKS
*/
static void touch_polling_timer_cb(lv_timer_t* timer){
  lv_indev_t * indev = lv_indev_get_next(NULL);
  if(indev == NULL) return;

  lv_point_t p;
  lv_indev_get_point(indev,&p);

  if(last_touch_point.x == p.x && last_touch_point.y == p.y) return;
  String message = "X: "+String(p.x)+" Y: "+String(p.y);

  lv_label_set_text(ui_labelTouch,message.c_str());
}

static void update_clock_timer_cb(lv_timer_t * timer){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
    return;
  }
  if(last_month != timeinfo.tm_mon){
    last_month = timeinfo.tm_mon;
    lv_label_set_text(ui_lblMonth, months[last_month]);
  }
  if(last_day != timeinfo.tm_wday){
    last_day = timeinfo.tm_wday;
    lv_label_set_text(ui_lblWday, days[last_day]);
    lv_label_set_text_fmt(ui_lblDay,", %02d",timeinfo.tm_mday);
  }
  if(last_hour!=timeinfo.tm_hour){
    last_hour = timeinfo.tm_hour;
    lv_label_set_text_fmt(ui_lblHour,"%02d",timeinfo.tm_hour);
  }
  if(last_min != timeinfo.tm_min){
    last_min = timeinfo.tm_min;
    lv_label_set_text_fmt(ui_lblMin,"%02d",timeinfo.tm_min);
  }
  if(last_sec != timeinfo.tm_sec){
    last_sec = timeinfo.tm_sec;
    lv_bar_set_value(ui_secondsBar,timeinfo.tm_sec,LV_ANIM_ON);

    if(last_sec%2==0){
      lv_obj_set_style_text_color(ui_lblPoints,lv_color_hex(0x2095f6),LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else{
      lv_obj_set_style_text_color(ui_lblPoints,lv_color_hex(0x003e71),LV_PART_MAIN | LV_STATE_DEFAULT);
    } 
  }
}

/*
  BUTTON EVENTS  
*/
static void btnDisconnect_event_cb(lv_event_t *e){
  lv_indev_t * indev = lv_indev_get_act();
  if(indev == NULL) return;

  lv_label_set_text(ui_labelTouch,"RESET");

  Command cmd = cmd_create_wifi_disconnect();
  xQueueSend(wifi_inbox,&cmd,0);
}

static void btnSend_event_cb(lv_event_t *e){
  lv_indev_t * indev = lv_indev_get_act();
  if(indev == NULL) return;
  String msg;
  if(lv_obj_has_state(ui_cbxValue,LV_STATE_CHECKED)) msg = "SELEZIONATO";
  else msg = "NON SELEZIONATO";

  Command cmd = cmd_create_mqtt_send("esp32_test/berry/dati",msg.c_str());
  xQueueSend(mqtt_inbox,&cmd,0);
}



static void draw_home_screen(){
  ui_init();
}

void ui_manager_init(QueueHandle_t wifi_queue, QueueHandle_t mqtt_queue){
    wifi_inbox = wifi_queue;
    mqtt_inbox = mqtt_queue;

    draw_home_screen();
    //eventi
    lv_obj_add_event_cb(ui_btnDisconnect,btnDisconnect_event_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(ui_btnSend,btnSend_event_cb,LV_EVENT_CLICKED,NULL);
    //timer
    lv_timer_create(touch_polling_timer_cb,50,NULL);
    lv_timer_create(update_clock_timer_cb,500,NULL);


}