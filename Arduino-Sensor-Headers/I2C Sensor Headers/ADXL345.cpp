/*---------------------------------------------------------------------- 
* ADXL 345 - Accelerometer
*
* Based largely on code from:
* http://codeyoung.blogspot.com/2009/11/adxl345-accelerometer-breakout-board.html
* 
* Note: initAdxl() function must be called before any others.
*
* Datasheet: 
* https://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
*
----------------------------------------------------------------------*/
#include <Wire.h>
#include <Arduino.h>
#include "ADXL345.h"

#define ADXL_I2C_ADDRESS 0x53
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ] ;    //6 bytes buffer for saving data read from the device
char str[512];     

//Non-external functions
void writeTo(int, byte, byte);
void readFrom(int, byte, int, byte*);

/*
 * Sets up the accelerometer
 */
void initAdxl(){
  //Turning on the ADXL345
  writeTo(ADXL_I2C_ADDRESS, 0x2D, 0);      
  writeTo(ADXL_I2C_ADDRESS, 0x2D, 16);
  writeTo(ADXL_I2C_ADDRESS, 0x2D, 8);
}

/*
 * Prints x, y, z values
 */
void printAdxlAccelerometerReadings(){
  int x, y, z;
  getAdxlAccelerometerReadings(&x, &y, &z);

  Serial.print("x: ");
  Serial.print( x );
  Serial.print(" y: ");
  Serial.print( y );
  Serial.print(" z: ");
  Serial.println( z );
}

/*
 * Retrieves x, y, z readings and places them in the passed variables
 */
void getAdxlAccelerometerReadings(int *x, int *y, int *z) {
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  
  readFrom(ADXL_I2C_ADDRESS, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
  
   //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
   //thus we are converting both bytes in to one int
  *x = (((int)buff[1]) << 8) | buff[0];   
  *y = (((int)buff[3])<< 8) | buff[2];
  *z = (((int)buff[5]) << 8) | buff[4];
}

/*
 * Write val to address
 */
void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

/* 
 *  Reads num bytes starting from address register on device in to _buff array
 */
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}


