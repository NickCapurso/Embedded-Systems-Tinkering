/*----------------------------------------------------------------------
* Datasheet for our DS1307 Real Time Clock:
* http://datasheets.maximintegrated.com/en/ds/DS1307.pdf
*
* These RTC uses binary-coded decimal to represent the time and date.
* https://en.wikipedia.org/wiki/Binary-coded_decimal
* 
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
----------------------------------------------------------------------*/
#ifndef _RTC_GUARD
#include <Arduino.h>
byte decToBcd(byte);
byte bcdToDec(byte);
void setRtcTime(byte, byte, byte, byte, byte, byte, byte);
void getRtcTime(byte*, byte*, byte*, byte*, byte*, byte*, byte*);
void printRtcTime();
#endif
