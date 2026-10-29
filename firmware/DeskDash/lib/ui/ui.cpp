#include "ui.h"
#include <lvgl.h>
#include <Arduino.h>

static lv_obj_t * label;

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

  lv_label_set_text(label,"PAUSA");
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

    lv_obj_add_event_cb(lv_scr_act(),screen_event_cb,LV_EVENT_PRESSING,NULL);
    lv_obj_add_event_cb(button,btn_event_cb,LV_EVENT_CLICKED,NULL);
}