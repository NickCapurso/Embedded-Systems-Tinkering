/*---------------------------------------------------------------------- 
* BMP 085 - Barometer
*
* Based largely on code by  Jim Lindblom.
* NOTES: 
*   - The initBmp() function must be called before any others.
*   - getBmpTemperature() (or one of the temperature printing functions) 
*   must be called before retrieve pressure/atmospheres/altitude, as it
*   sets ones of the global variables needed for those further calculations
*
* Datasheet: 
* https://www.sparkfun.com/datasheets/Components/General/BST-BMP085-DS000-05.pdf
*
----------------------------------------------------------------------*/

#include <Wire.h>
#include <Arduino.h>
#include "BMP085.h"

#define BMP085_I2C_ADDRESS 0x77
#define STD_ATM 101325 //Standard atmosphere reference

//Non-external functions
float getBmpTrueTemperature(unsigned int);
unsigned int getBmpUncompensatedTemperature();
long getBmpTruePressure(unsigned long);
unsigned long getBmpUncompensatedPressure();

const unsigned char OSS = 0;  // Oversampling Setting

// Calibration values (see datasheet pg 12)
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
long b5; 

/*
* 11 calibration coefficients are stored in the sensor's EEPROM.
* These are stored in global variables and used to calculate pressure 
* and temperatures later. (see datasheet pg 12)
*/
void initBmp()
{
  ac1 = readBmpInt(0xAA);
  ac2 = readBmpInt(0xAC);
  ac3 = readBmpInt(0xAE);
  ac4 = readBmpInt(0xB0);
  ac5 = readBmpInt(0xB2);
  ac6 = readBmpInt(0xB4);
  b1 = readBmpInt(0xB6);
  b2 = readBmpInt(0xB8);
  mb = readBmpInt(0xBA);
  mc = readBmpInt(0xBC);
  md = readBmpInt(0xBE);
}

/* 
*  Read 2 bytes from the BMP085
* First byte will be from 'address'
* Second byte will be from 'address'+1
*/
int readBmpInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_I2C_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_I2C_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

/* 
 * Read 1 byte from the BMP085 at 'address' 
 */
char readBmpByte(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_I2C_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_I2C_ADDRESS, 1);
  while(!Wire.available())
    ;

  return Wire.read();
}

/*
 * Gets the true temperature from the BMP 085 (in Celsius) and places it
 * in the passed variable
 */
void getBmpTemperature(float* temp){
  *temp = getBmpTrueTemperature(getBmpUncompensatedTemperature());
}

/*
 * Prints the temperature after converting it to Fahrenheit
 */
void printBmpFahrenheit(){
  float temp = 0;
  getBmpTemperature(&temp);
  Serial.print("Temp: ");
  Serial.print((32 + 1.8*temp));
  Serial.println(" F");
}

/*
 * Prints the temperature after converting it to Celsius 
 */
void printBmpCelsius(){
  float temp = 0;
  getBmpTemperature(&temp);
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" C");
}

/*
* Calculates the true temperatures using the callibration coefficients and 
* places it in the passed variable
* 
* Equations given on datasheet pg 13
*
* Parameter: the uncompensated / raw temperature reading
*/
float getBmpTrueTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;
}

/*
* Returns the raw / uncompensated temperature (for getTrueTemperature())
*/
unsigned int getBmpUncompensatedTemperature(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_I2C_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = readBmpInt(0xF6);
  return ut;
}

/*
 * Gets the pressure (in Pa) from the BMP 085 and places it
 * in the passed variable
 */
void getBmpPressure(long* pressure){
  *pressure = getBmpTruePressure(getBmpUncompensatedPressure());
}

/*
 * Prints the temperature after converting it to Celsius
 */
void printBmpPressure(){
  long pressure = 0;
  getBmpPressure(&pressure);
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");
}

/*
* Calculates the true pressure (in Pa) using the callibration coefficients.
* Equations given on datasheet pg 13
* 
* b5 is also required so getTrueTemperature(...) must be called first.
*
* Parameter: the uncompensated / raw pressure reading
*/
long getBmpTruePressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

/* 
 * Reads the uncompensated pressure value 
 */
unsigned long getBmpUncompensatedPressure(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_I2C_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = readBmpByte(0xF6);
  lsb = readBmpByte(0xF7);
  xlsb = readBmpByte(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

/*
 * Prints air pressure as a unit of atmospheres
 */
void printBmpStdAtm(){
  float atm = 0;
  getBmpStdAtmosphere(&atm);

  Serial.print("Standard atmosphere: ");
  Serial.println(atm, 4); //Truncated at 4 decimal places
}

/*
 * Retrieves pressure as a unit of the standard atmosphere (at sea level), in atm
 * and places it in the passed variable
 */
void getBmpStdAtmosphere(float* atm){
  float pressure = getBmpTruePressure(getBmpUncompensatedPressure());
  *atm = pressure / STD_ATM;
}

/*
 * Prints altitude
 */
void printBmpAltitude(){
  float alt = 0;
  getBmpAltitude(&alt);

  Serial.print("Altitude: ");
  Serial.print(alt, 2); //Truncated at 2 decimal places
  Serial.println(" M");
}

/*
 * Retrieves altitude based on the current air pressure and
 * places it in the passed variable
 */
void getBmpAltitude(float* alt){
  float A = 0;
  getBmpStdAtmosphere(&A);
  
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;
  
  *alt = C;
}

