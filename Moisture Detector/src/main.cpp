#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define SOIL 36

String getMoisture();

const int airValue = 3600;   
const int waterValue = 2700;  
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

void setup() {
  Serial.begin(115200); 
  delay(50);
}

void loop() {
  String moisturePer = getMoisture();
  Serial.println("16 Moisture_Sensor " + moisturePer);
  delay(5000);
}

String getMoisture() {
  String mositurePer = "";

  soilMoistureValue = analogRead(SOIL);
  soilmoisturepercent = map(soilMoistureValue, airValue, waterValue, 0, 100);
  if(soilmoisturepercent >= 100) {
    mositurePer = "100";
  } else if(soilmoisturepercent <= 0){
    mositurePer = "0";
  } else if(soilmoisturepercent > 0 && soilmoisturepercent < 100){
    mositurePer = soilmoisturepercent;
  }

  return mositurePer;
}