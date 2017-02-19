#ifndef COMMON_H
#define COMMON_H

#include "SevSeg.h"

#define I2C_ADDR_SEVSEG_1 0x10
#define I2C_ADDR_SEVSEG_2 0x20
#define I2C_ADDR_BARGRAPH_1 0x30

#define DEFAULT_BAUD 9600

#define SEVSEG_BLANK_CODE 1000
#define BARGRAPH_MAX_BARS 10
#define BAR_GRAPH_BRIGHTNESS 90

#define BYTE_MASK 0xFF


void sendToArduino(int addr, String data) {
  int intVal = data.toInt();
  Wire.beginTransmission(addr);
  while(intVal) {
    Wire.write(intVal & BYTE_MASK);
    intVal = intVal >> 8;
  }
  Wire.endTransmission();
}

void displaySevSeg(SevSeg *sevseg, int num) {
  if (num < 0 ) sevseg->setNumber(SEVSEG_BLANK_CODE, 0);
  else sevseg->setNumber(num, 0);
}

void displayBarGraph(int *ledPins, int barLevel) {
      if(barLevel > BARGRAPH_MAX_BARS) barLevel = BARGRAPH_MAX_BARS;
      if(barLevel < 0) barLevel = 0;

    for (int ledPtr = 0; ledPtr < BARGRAPH_MAX_BARS; ledPtr++) {
      if (ledPtr < barLevel) digitalWrite(ledPins[ledPtr], HIGH);
      else digitalWrite(ledPins[ledPtr], LOW);
    }
}

int byteArrToInt(byte *arr, int arrLen) {
  int intVal = 0;
  for (int i = arrLen-1; i >= 0; i--) {
    intVal = intVal << 8 | arr[i];
  }
  return intVal;
}
#endif
