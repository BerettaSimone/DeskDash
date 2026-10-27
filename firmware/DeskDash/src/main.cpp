#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#define TFT_GREY 0x5AEB

TFT_eSPI tft = TFT_eSPI();

void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  tft.fillScreen(TFT_GREY);

  tft.setCursor(0,0, 4);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Hello World!");

  delay(10000);
}
