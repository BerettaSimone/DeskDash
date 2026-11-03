#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "hardware.h"
#include "config.h"

TFT_eSPI tft = TFT_eSPI();;
XPT2046_Touchscreen ts(TOUCH_CS,TOUCH_INTERRUPT);
SPIClass touchSPI(VSPI);

void hardware_init(){
    tft.init();
    tft.initDMA();
    tft.setRotation(DEFAULT_ROTATION);
    tft.setCursor(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
    tft.fillCircle(30,30,30,TFT_RED);
    touchSPI.begin(TOUCH_CK, TOUCH_MISO,TOUCH_MOSI,TOUCH_CS);
    ts.begin(touchSPI);
}

bool hw_read_touch(TouchData &data){
    if(ts.touched()){
        TS_Point p = ts.getPoint();
        data.x = map(p.x,TOUCH_MIN_VALUE,TOUCH_MAX_VALUE,0,SCREEN_WIDTH);
        data.y = map(p.y,TOUCH_MIN_VALUE,TOUCH_MAX_VALUE,0,SCREEN_HEIGHT);
        data.z = 0;
        return true;
    }
    return false;
}
