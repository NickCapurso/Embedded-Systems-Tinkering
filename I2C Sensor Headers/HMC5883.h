/*---------------------------------------------------------------------- 
* HMC 5883L - Magnetometer / Compass
*
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
* 
* Based largely on code by Jordan McConnell / Sparkfun.
* NOTE: the initHmc() function must be called before any others.
*
* Datasheet: 
* https://www.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
*
----------------------------------------------------------------------*/
#ifndef _HMC_GUARD
void initHmc();
void printHmcCompassReadings();
void getHmcCompassReading(int *, int *, int *);
#endif
