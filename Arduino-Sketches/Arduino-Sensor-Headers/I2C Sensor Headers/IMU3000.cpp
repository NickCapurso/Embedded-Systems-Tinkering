/*---------------------------------------------------------------------- 
*IMU 3000 - Gyroscope
* NOTE: the initImu() function must be called before any others.
*
* Datasheet: 
* http://store.invensense.com/datasheets/invensense/PS-IMU-3000A.pdf
*
----------------------------------------------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include "IMU3000.h"


// Gyro I2C address. Note, this is the same as the RTC. The IMU's I2C address
// can be changed by writing ot its WHO_AM_I register.
#define GYRO_I2C_ADDRESS 0x68         

// Address of the register for configuring sampling rate and filtering
#define REG_PARAMS 0x16   

// Address of the X-data register for the gyroscope)
#define REG_GYRO_X 0x1D   

/*
 * Used to setup sampling rate and filtering
 */
void initImu(){
    // Set Gyro settings
    // Sample Rate 1kHz, Filter Bandwidth 42Hz, Gyro Range 500 d/s 
    Wire.beginTransmission(GYRO_I2C_ADDRESS); // start transmission to device 
    Wire.write(REG_PARAMS);                   // send register address
    Wire.write(0x0B);                         // send value to write
    Wire.endTransmission();                   // end transmission
}

/*
 * Retrieve X, Y, and Z gyroscope readings and place them
 * in the passed variables
 */
void getImuReading(int *x, int *y, int *z){
    Wire.beginTransmission(GYRO_I2C_ADDRESS);
    Wire.write(REG_GYRO_X); //Register Address GYRO_XOUT_H
    Wire.endTransmission();

    // New read the 12 data bytes
    Wire.beginTransmission(GYRO_I2C_ADDRESS);
    Wire.requestFrom(GYRO_I2C_ADDRESS,12); // Read 12 bytes
    
    *x = Wire.read() << 8 | Wire.read();
    *y = Wire.read() << 8 | Wire.read();
    *z = Wire.read() << 8 | Wire.read();
    Wire.endTransmission();
}

/*
 * Print the X, Y, and Z gyroscope readings
 * to monitor
 */
void printImuReading(){
   int x,y,z;
   getImuReading(&x, &y, &z);
  
   Serial.print(x);
   Serial.print("\t");
   Serial.print(y);
   Serial.print("\t");
   Serial.println(z); 
}
