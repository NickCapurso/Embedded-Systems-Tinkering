#include <Wire.h>
#include "SevSeg.h"
#include "Common.h"

#define NUM_DIGITS 3

SevSeg sevSeg; //Instantiate a seven segment controller object

void setup() {
  byte digitPins[] = {A0, A1, A2};
  byte segmentPins[] = {5, 6, 7, 8, 9, 10, 11, 4};

  sevSeg.begin(COMMON_ANODE, NUM_DIGITS, digitPins, segmentPins);
  sevSeg.setBrightness(BAR_GRAPH_BRIGHTNESS);
  sevSeg.setNumber(SEVSEG_BLANK_CODE, 0);

  Wire.begin(I2C_ADDR_SEVSEG_2);
  Wire.onReceive(onI2CReceived);
}

void loop() {
  sevSeg.refreshDisplay();
}

void onI2CReceived(int numBytes) {
  byte byteArr[numBytes];
  for (int i = 0; i < numBytes; i++) {
    byteArr[i] = Wire.read();
  }

  displaySevSeg(&sevSeg, byteArrToInt(byteArr, numBytes));
}

