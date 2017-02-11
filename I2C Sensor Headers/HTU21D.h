/*---------------------------------------------------------------------- 
* HTU 21D - Temperature / Humidity
* 
* Based on code from Sparkfun's GitHub
* 
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
* 
* Datasheet: 
* https://www.adafruit.com/datasheets/1899_HTU21D.pdf
*
----------------------------------------------------------------------*/
#ifndef _HMC_GUARD
void printHtuTemperatureCelsius();
void printHtuTemperatureFahrenheit();
void getHtuTemperature(float*);
void printHtuHumidity();
void getHtuHumidity(float*);
#endif
