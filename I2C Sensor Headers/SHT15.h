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
* 
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
----------------------------------------------------------------------*/
#ifndef _SHT_GUARD
void initSht(int, int);
void printShtTemperatureFahrenheit();
void printShtTemperatureCelsius();
void printShtHumidity();
void getShtTemperature(float*);
void getShtHumidity(float*);
#endif
