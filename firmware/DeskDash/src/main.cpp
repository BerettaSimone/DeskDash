#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();
lv_obj_t * label;
XPT2046_Touchscreen * ts;
SPIClass * touchSPI;

// Funzione di callback: dice a LVGL come scrivere i pixel usando TFT_eSPI
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.pushImage(area->x1,area->y1,w,h,(uint16_t *)&color_p->full);
    
    lv_disp_flush_ready(disp_drv);
}

void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data){
  if(ts->touched()){
   data->state = LV_INDEV_STATE_PR; 

   TS_Point touched = TS_Point();
   touched.x = map(ts->getPoint().x,200,3800,0,320);
   touched.y = map(ts->getPoint().y,200,3800,0,240);
   data->point.x = touched.x;
   data->point.y = touched.y;
  
  } else {
    data->state = LV_INDEV_STATE_REL;
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

  lv_label_set_text(label,"PAUSA");
}

void setup_display(){
  lv_init();
  static lv_color_t buf[320*240/10];
  static lv_disp_draw_buf_t draw_buf;

  lv_disp_draw_buf_init(&draw_buf,buf,NULL,320*240/10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello World!");
  lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);
  lv_obj_center(label);

  static lv_obj_t * button = lv_btn_create(lv_scr_act());
  lv_obj_center(button);
  lv_obj_set_pos(button,0,50);
  lv_obj_t * label1 = lv_label_create(button);
  lv_label_set_text(label1,"Clicca qui");



  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  lv_obj_add_event_cb(lv_scr_act(),screen_event_cb,LV_EVENT_PRESSING,NULL);
  lv_obj_add_event_cb(button,btn_event_cb,LV_EVENT_CLICKED,NULL);

}


void setup_touchscreen(){
  touchSPI=new SPIClass(VSPI);
  touchSPI->begin(25,39,32,33);
  ts = new XPT2046_Touchscreen(33,36);
  ts->begin(*touchSPI);
}

void setup() {
  Serial.begin(115200);
    
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  tft.init();
  tft.setRotation(1);


  setup_display();
  setup_touchscreen();
}

void loop() {
  lv_timer_handler();
  delay(5);
}
