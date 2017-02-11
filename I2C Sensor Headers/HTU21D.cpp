/*---------------------------------------------------------------------- 
* HTU 21D - Temperature / Humidity
* 
* Based on code from Sparkfun's GitHub
* 
* Datasheet: 
* https://www.adafruit.com/datasheets/1899_HTU21D.pdf
*
----------------------------------------------------------------------*/
#include <Wire.h>
#include <Arduino.h>
#include "HTU21D.h"

#define HTU_I2C_ADDRESS 0x40

//Register definitions
#define TRIGGER_TEMP_MEASURE_HOLD  0xE3
#define TRIGGER_HUMD_MEASURE_HOLD  0xE5
#define TRIGGER_TEMP_MEASURE_NOHOLD  0xF3
#define TRIGGER_HUMD_MEASURE_NOHOLD  0xF5
#define WRITE_USER_REG  0xE6
#define READ_USER_REG  0xE7
#define SOFT_RESET  0xFE

//Non-external functions
float getHtuTrueTemperature(int);
unsigned int getHtuUncompensatedTemp();
float getHtuTrueHumidity(int);
unsigned int getHtuUncompensatedHumidity();

/*
 * Prints the temperature in Celsius 
 */
void printHtuTemperatureCelsius(){
  float temperature = 0;
  getHtuTemperature(&temperature);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
}

/*
 * Prints the temperature in Fahrenheit
 */
void printHtuTemperatureFahrenheit(){
  float temperature = 0;
  getHtuTemperature(&temperature);
  Serial.print("Temperature: ");
  Serial.print(32 + 1.8*temperature);
  Serial.println(" F");
}

/*
 * Prints the humidity
 */
void printHtuHumidity(){
  float humidity = 0;
  getHtuHumidity(&humidity);
  Serial.print("Humdity: ");
  Serial.println(humidity);
}

/*
 * Retrieves the true temperature and places it in the passed variable
 */
void getHtuTemperature(float* temp){
  *temp = getHtuTrueTemperature(getHtuUncompensatedTemp());
}

/*
 * Retrieves the true humidity and places it in the passed variable
 */
void getHtuHumidity(float* humid){
  *humid = getHtuTrueHumidity(getHtuUncompensatedHumidity());
}

/*
 * Given the raw temperature data, calculate the actual temperature
 */
float getHtuTrueTemperature(int SigTemp)
{
  float tempSigTemp = SigTemp / (float)65536; //2^16 = 65536
  float realTemperature = -46.85 + (175.72 * tempSigTemp); //From page 14

  return(realTemperature);  
}

/*
 * Returns the uncompensated temperature value
 */
unsigned int getHtuUncompensatedTemp()
{
  //Request the temperature
  Wire.beginTransmission(HTU_I2C_ADDRESS);
  Wire.write(TRIGGER_TEMP_MEASURE_NOHOLD);
  Wire.endTransmission();

  //Wait for sensor to complete measurement
  delay(60); //44-50 ms max - we could also poll the sensor

  //Comes back in three bytes, data(MSB) / data(LSB) / CRC
  Wire.requestFrom(HTU_I2C_ADDRESS, 3);

  //Wait for data to become available
  int counter = 0;
  while(Wire.available() < 3)
  {
    counter++;
    delay(1);
    if(counter > 100) return 998; //Error out
  }

  unsigned char msb, lsb, crc;
  msb = Wire.read();
  lsb = Wire.read();
  crc = Wire.read(); //We don't do anything with CRC for now

  unsigned int temperature = ((unsigned int)msb << 8) | lsb;
  temperature &= 0xFFFC; //Zero out the status bits but keep them in place

  return temperature;
}

/*
 * Given the raw humidity data, calculate the actual relative humidity
 */
float getHtuTrueHumidity(int SigRH)
{
  float tempSigRH = SigRH / (float)65536; //2^16 = 65536
  float rh = -6 + (125 * tempSigRH); //From page 14

  return(rh);  
}


/*
 * Read the uncompensated humidity
 */
unsigned int getHtuUncompensatedHumidity()
{
  byte msb, lsb, checksum;

  //Request a humidity reading
  Wire.beginTransmission(HTU_I2C_ADDRESS);
  Wire.write(TRIGGER_HUMD_MEASURE_NOHOLD); //Measure humidity with no bus holding
  Wire.endTransmission();

  //Hang out while measurement is taken. 50mS max, page 4 of datasheet.
  delay(55);

  //Read result
  Wire.requestFrom(HTU_I2C_ADDRESS, 3);

  //Wait for data to become available
  int counter = 0;
  while(Wire.available() < 3)
  {
    counter++;
    delay(1);
    if(counter > 100) return 0; //Error out
  }

  msb = Wire.read();
  lsb = Wire.read();
  checksum = Wire.read();

  unsigned int rawHumidity = ((unsigned int) msb << 8) | (unsigned int) lsb;
  rawHumidity &= 0xFFFC; //Zero out the status bits but keep them in place

  return(rawHumidity);
}

