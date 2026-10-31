#include "lvgl_port.h"
#include <lvgl.h>
#include "hardware.h"
#include "config.h"

void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    hw_draw_image(area->x1,area->y1,w,h,(uint16_t *)&color_p->full);
    
    lv_disp_flush_ready(disp_drv);
}

void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data){
    TouchData t_data;
  
    if(hw_read_touch(t_data)){
    data->state = LV_INDEV_STATE_PR; 
    data->point.x = t_data.x;
    data->point.y = t_data.y;
  
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void lvgl_init(){
    lv_init();

    static lv_color_t buf[SCREEN_WIDTH*SCREEN_HEIGHT/10];
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf,buf,NULL,SCREEN_WIDTH*SCREEN_HEIGHT/10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

void lvgl_handle(){
  lv_timer_handler();
}