/*---------------------------------------------------------------------- 
* HMC 5883L - Magnetometer / Compass
*
* Based largely on code by Jordan McConnell / Sparkfun.
* NOTE: the initHmc() function must be called before any others.
*
* Datasheet: 
* https://www.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
*
----------------------------------------------------------------------*/

#include <Arduino.h>
#include <Wire.h>
#include "HMC5883.h"

#define HMC_I2C_ADDR 0x1E 

/**
 * Put the HMC5883 IC into the correct operating mode
 */
void initHmc(){
  Wire.beginTransmission(HMC_I2C_ADDR); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

/*
 * Prints the X, Y, and Z compass readings.
 */
void printHmcCompassReadings(){
  int x, y, z;
  getHmcCompassReading(&x, &y, &z);

  Serial.print("X:\t");
  Serial.print(x);
  Serial.print("\tY:\t");
  Serial.print(y);
  Serial.print("\tZ:\t");
  Serial.println(z);
}

/*
 * Retrieves X, Y, and Z compass readings and places them
 * in the passed variables
 * See datasheet pg 11
 */
void getHmcCompassReading(int *x, int *y, int *z){
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC_I2C_ADDR);
  Wire.write(0x03); //Select register 3, X MSB register
  Wire.endTransmission();
  
 
 //Read data from each axis, 2 registers per axis. MSB is read first.
  Wire.requestFrom(HMC_I2C_ADDR, 6);
  if(6<=Wire.available()){
    *x = Wire.read()<<8; //X msb
    *x |= Wire.read(); //X lsb
    *z = Wire.read()<<8; //Z msb (The Z register comes before Y)
    *z |= Wire.read(); //Z lsb
    *y = Wire.read()<<8; //Y msb
    *y |= Wire.read(); //Y lsb
  }
}

