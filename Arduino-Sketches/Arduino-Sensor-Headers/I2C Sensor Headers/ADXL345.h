/*---------------------------------------------------------------------- 
* ADXL 345 - Accelerometer
*
* Based largely on code from:
* http://codeyoung.blogspot.com/2009/11/adxl345-accelerometer-breakout-board.html
* 
* Note: initAdxl() function must be called before any others.
*
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
* 
* Datasheet: 
* https://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
*
----------------------------------------------------------------------*/
#ifndef _ADXL_GUARD
void initAdxl();
void printAdxlAccelerometerReadings();
void getAdxlAccelerometerReadings(int*, int*, int*);
#endif
