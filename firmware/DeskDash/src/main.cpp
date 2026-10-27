#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

TFT_eSPI tft = TFT_eSPI();


// Funzione di callback: dice a LVGL come scrivere i pixel usando TFT_eSPI
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.pushImage(area->x1,area->y1,w,h,(uint16_t *)&color_p->full);
    
    lv_disp_flush_ready(disp_drv);
}

void setup() {
    Serial.begin(115200);
    
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    tft.init();
    tft.setRotation(1);

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

  lv_obj_t * label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello World!");
  lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);
  lv_obj_center(label);
}

void loop() {
  lv_timer_handler();
  delay(5);
}
