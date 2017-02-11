#include <Wire.h>
#include "RTC.h"
#include "IMU3000.h"
#include "BMP085.h"
#include "HMC5883.h"
#include "SHT15.h"
#include "HTU21D.h"
#include "ADXL345.h"
#define MILLISECOND 1
#define ONE_SECOND MILLISECOND * 1000

void setup() {
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
  Wire.begin();

  // Setup for I2C sensors
  //  initBmp();
  //  setRtcTime(0, 4, 4, 7, 20, 2, 16);
  //  initImu();
  //  initHmc();
  //  initSht(2,3);
  initAdxl();
}

void loop() {
  delay(adxl345());
}

/*
  TMP36GZ sensors (clear bag)

  Measure the temperature in Celsius or Fahrenheit from
  the TMP36GZ sensors (3 pins - gnd, vout, 3.3-5V).

  If looking at the "flat" side (side with words):
     - Leftmost pin = 5V
     - Middle pin = Vout
     - Rightmost pin = GND

  http://www.learningaboutelectronics.com/images/TMP36-pinout.png
*/
int tmp36gz(int analogPin, boolean fahrenheit) {
  int rawIn = analogRead(analogPin);

  int temp = 25 + ((rawIn / 1023.0) * 5000 - 750) / 10;

  Serial.print("Temp: ");
  Serial.print(fahrenheit ? (32 + 1.8 * temp) : temp);
  Serial.println(fahrenheit ? " F" : " C");

  return ONE_SECOND;
}

/*
  Adafruit pulse sensor. Purple line = Vout.
  Recommends a fast sensor reading (millisecond or less)
  in order to capture pulse "spikes"

  http://pulsesensor.com/pages/pulse-sensor-amped-arduino-v1dot1
*/
int pulseSensor(int analogPin) {
  int rawIn = analogRead(analogPin);

  Serial.print("Pulse sensor raw: ");
  Serial.println(rawIn);

  return MILLISECOND;
}

/*
  The large (0.5 in) FSR.

  Resistance changes as you press it. More
  pressure == lower resistance.

  5V on one pin and GND on the other
   -- However, all the voltage will drop over
      the FSR (because it is the only load)
      so you hook up a second, fixed resistor
      in series to create a voltage divider
      and the amount dropped will be proportional
      to the force applied on the FSR.

  http://bildr.org/2012/11/force-sensitive-resistor-arduino/
*/
int largeForceSensitiveRes(int analogPin) {
  int rawIn = analogRead(analogPin);

  Serial.print("Force resistor: ");
  Serial.println(rawIn);

  return ONE_SECOND;
}

/*
  Adafruit hall effect sensor. (looks like the TMP36GZ,
  but smaller). In the pink labeled bags.

  Detects whether a magnet is near.

  Pinout look at the side with the writing:
   - Left pin = 5V
   - Middle pin = GND
   - Right pin = Vout (need pullup resistor)

  With a pullup resistor, vout goes low when a magnet
  is near.

  https://www.adafruit.com/products/158
*/
int hallEffectSensor(int digitalPin_pullup) {
  int value = digitalRead(digitalPin_pullup);

  Serial.print("Magnetic field? ");
  Serial.println(value == LOW ? "Yes" : "No");

  return ONE_SECOND;
}


/*
  DS1307 Real Time Clock.

  Uses the RTC.h
*/
int rtc() {
  printRtcTime();
  return ONE_SECOND;
}


/*
  IMU3000 Gyroscope.

  Uses the IMU3000.h
*/
int imu3000() {
  printImuReading();
  return 500 * MILLISECOND;
}

/*
   BMP085 Barometer.

   Uses BMP085.h
*/
int bmp085() {
  printBmpFahrenheit();
  printBmpPressure();
  printBmpStdAtm();
  printBmpAltitude();
  Serial.println();
  return ONE_SECOND;
}

/**
   HMC5883L Magnetomer.

   Uses HMC5883.h
*/
int hmc5883() {
  printHmcCompassReadings();
  return ONE_SECOND;
}

/**
*  SHT15 Temperature / Humidity Sensor.
*
*   Uses SHT15.h
*/
int sht15() {
  printShtTemperatureFahrenheit();
  printShtHumidity();
  return ONE_SECOND;
}

/*
*   HTU21D Temperature / Humidity Sensor.
*
*   Uses HTU21D.h
*/
int htu21d() {
  printHtuTemperatureCelsius();
  printHtuTemperatureFahrenheit();
  printHtuHumidity();
  return ONE_SECOND;
}

/*
*   ADXL345 Accelerometer
*
*   Uses ADXL345.h
*/
int adxl345() {
  printAdxlAccelerometerReadings();
  return 500 * MILLISECOND;
}

