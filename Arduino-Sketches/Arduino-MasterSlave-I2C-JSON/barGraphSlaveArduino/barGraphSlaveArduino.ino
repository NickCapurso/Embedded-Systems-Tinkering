/**
 * Based on examples from http://www.arduino.cc/en/Tutorial/BarGraph
 */
#include <Wire.h>
#include "Common.h"

#define LED_COUNT 10

const int VISA_LED = 12;
const int MASTERCARD_LED = 13;

int ledPins[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

void setup() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(VISA_LED, OUTPUT);
  pinMode(MASTERCARD_LED, OUTPUT);
  clearLeds();
  
  Wire.begin(I2C_ADDR_BARGRAPH_1);
  Wire.onReceive(onI2CReceived);
}

void loop() {
  for(int i = 0; i < LED_COUNT; i++) {
     digitalWrite(ledPins[i], HIGH);
     delay(50);
  }

  for(int i = 0; i < LED_COUNT; i++) {
     digitalWrite(ledPins[i], LOW);
     delay(50);
  }
}

void onI2CReceived(int numBytes) {
  int led = Wire.read();
  clearLeds();
  if (led == 1) 
    digitalWrite(MASTERCARD_LED, HIGH);
  else if (led == 2)
    digitalWrite(VISA_LED, HIGH);

  //Works, but decided not to use
  //displayBarGraph(ledPins, Wire.read());
}

void clearLeds() {
  digitalWrite(VISA_LED, LOW);
  digitalWrite(MASTERCARD_LED, LOW);
}

