#include <Arduino.h>
#include <U8x8lib.h>

#define LED 25

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up the project!!!");

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  pinMode(LED, OUTPUT);
}

void loop() {
  u8x8.println("Jai Ganesha");

  delay(500);
  digitalWrite(LED, HIGH);

  delay(500);
  digitalWrite(LED, LOW);

  Serial.println("Serial monitor log");
}
