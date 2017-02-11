
/*----------------------------------------------------------------------
* SHT15 - Temperature and Humidity
* 
* Based of the wiring code at http://wiring.org.co/learning/basics/humiditytemperaturesht15.html
* 
* Note, you must call initSht with the data and clock pins before
* anything else
* 
* Datasheet:
* https://www.sparkfun.com/datasheets/Sensors/SHT1x_datasheet.pdf
*
* Note, the SHT15 does NOT use I2C, but rather its own protocol that
* must be bitbanged.
----------------------------------------------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include "SHT15.h"

void SHT_sendCommand(int, int, int);
void SHT_waitForResult(int);
int SHT_getData(int, int);
void SHT_skipCrc(int, int);

int SHT_clockPin = 3;  // pin used for clock
int SHT_dataPin  = 2;  // pin used for data

/*
 * Set the pins used for data and clock.  
 * Defaults are data = 2, clock = 3
 */
void initSht(int dataPin, int clockPin){
  SHT_clockPin = clockPin;
  SHT_dataPin = dataPin;
}

/*
 * Print the temperature in Celsius
 */
void printShtTemperatureCelsius(){
  //these can take a bit to get the values (100ms or so)
  float temperature = 0;
  getShtTemperature(&temperature);
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
}

/*
 * Print the temperature in Fahrenheit
 */
void printShtTemperatureFahrenheit(){
  //these can take a bit to get the values (100ms or so)
  float temperature = 0;
  getShtTemperature(&temperature);
  temperature = temperature + 26 * 1.8;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" F");
}

/*
 * Print the humidity percentage
 */
void printShtHumidity(){
  float humidity = 0;
  getShtHumidity(&humidity);
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}

/*
 * Retrieve the temperature in Celsius and place in the
 * passed variable
 */
void getShtTemperature(float* temp){
  //Return Temperature in Celsius
  SHT_sendCommand(B00000011, SHT_dataPin, SHT_clockPin);
  SHT_waitForResult(SHT_dataPin);

  int val = SHT_getData(SHT_dataPin, SHT_clockPin);
  SHT_skipCrc(SHT_dataPin, SHT_clockPin);
  
  *temp = (float)val * 0.01 - 40; //convert to celsius
}

/*
 * Retrieve the humidity and place in the
 * passed variable
 */
void getShtHumidity(float* humid){
  //Return  Relative Humidity
  SHT_sendCommand(B00000101, SHT_dataPin, SHT_clockPin);
  SHT_waitForResult(SHT_dataPin);
  int val = SHT_getData(SHT_dataPin, SHT_clockPin);
  SHT_skipCrc(SHT_dataPin, SHT_clockPin);
  
  *humid = -4.0 + 0.0405 * val + -0.0000028 * val * val; 
}

/*
 * Bitbanging for issuing a command to the SHT15
 */
void SHT_sendCommand(int command, int dataPin, int clockPin){
  // send a command to the SHTx sensor
  // transmission start
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, LOW);

  // shift out the command (the 3 MSB are address and must be 000, the last 5 bits are the command)
  shiftOut(dataPin, clockPin, MSBFIRST, command);

  // verify we get the right ACK
  digitalWrite(clockPin, HIGH);
  pinMode(dataPin, INPUT);

  if (digitalRead(dataPin)) Serial.println("ACK error 0");
  digitalWrite(clockPin, LOW);
  if (!digitalRead(dataPin)) Serial.println("ACK error 1");
}


/*
 * Waiting for ACK from the SHT15
 */
void SHT_waitForResult(int dataPin){
  // wait for the SHTx answer
  pinMode(dataPin, INPUT);

  int ack; //acknowledgement

  //need to wait up to 2 seconds for the value
  for (int i = 0; i < 1000; ++i){
    delay(2);
    ack = digitalRead(dataPin);
    if (ack == LOW) break;
  }

  if (ack == HIGH) Serial.println("ACK error 2");
}

/*
 * Bitbanging for reading data from the SHT15
 */
int SHT_getData(int dataPin, int clockPin){
  // get data from the SHTx sensor

  // get the MSB (most significant bits)
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  byte MSB = shiftIn(dataPin, clockPin, MSBFIRST);

  // send the required ACK
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);

  // get the LSB (less significant bits)
  pinMode(dataPin, INPUT);
  byte LSB = shiftIn(dataPin, clockPin, MSBFIRST);
  return ((MSB << 8) | LSB); //combine bits
}

/*
 * CRC check
 */
void SHT_skipCrc(int dataPin, int clockPin){
  // skip CRC data from the SHTx sensor
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}

