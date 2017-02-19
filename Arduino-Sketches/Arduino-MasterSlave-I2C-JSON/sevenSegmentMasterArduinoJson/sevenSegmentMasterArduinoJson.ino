#include <Wire.h>
#include "ArduinoJson.h"
#include "SevSeg.h"
#include "Common.h"

#define NUM_DIGITS 3

SevSeg sevSeg;
StaticJsonBuffer<300> jsonBuffer;

const int MILES_LED = 2;
const int PTS_LED = 3;
const int CASH_LED = 12;

void setup() {
  byte digitPins[] = {A0, A1, A2};
  byte segmentPins[] = {5, 6, 7, 8, 9, 10, 11, 4};

  sevSeg.begin(COMMON_ANODE, NUM_DIGITS, digitPins, segmentPins);
  sevSeg.setBrightness(BAR_GRAPH_BRIGHTNESS);
  sevSeg.setNumber(SEVSEG_BLANK_CODE, 0);

  Serial.begin(DEFAULT_BAUD);
  Wire.begin();

  pinMode(MILES_LED, OUTPUT);
  pinMode(PTS_LED, OUTPUT);
  pinMode(CASH_LED, OUTPUT);
  clearLeds();
}

void loop() {

  if (Serial.available()) {
    String rcv = Serial.readString();
    char arr[rcv.length()];
    rcv.toCharArray(arr, rcv.length());

    JsonObject& root = jsonBuffer.parseObject(arr);
    if (!root.success())
    {
      Serial.println("JSON parsing failed");
      return;
    }

    String first = String((const char*)root["first"]);
    displaySevSeg(&sevSeg, first.toInt());

    clearLeds();
    String rewardsTypeStr = String((const  char*)root["rewardsType"]);
    int rewardsType = rewardsTypeStr.toInt();

    switch(rewardsType){
      case 1:
        digitalWrite(MILES_LED, HIGH);
        break;
      case 2:
        digitalWrite(PTS_LED, HIGH);
        break;
      case 3:
        digitalWrite(CASH_LED, HIGH);
        break;
    }

    sendToArduino(I2C_ADDR_BARGRAPH_1, String((const char*)root["second"]));
    delay(100);
    sendToArduino(I2C_ADDR_SEVSEG_2, String((const char*)root["third"]));
  }
  sevSeg.refreshDisplay();
}

void clearLeds() {
  digitalWrite(MILES_LED, LOW);
  digitalWrite(PTS_LED, LOW);
  digitalWrite(CASH_LED, LOW);  
}

