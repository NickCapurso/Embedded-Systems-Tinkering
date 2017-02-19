/*----------------------------------------------------------------------
* Datasheet for our DS1307 Real Time Clock:
* http://datasheets.maximintegrated.com/en/ds/DS1307.pdf
*
* These RTC uses binary-coded decimal to represent the time and date.
* https://en.wikipedia.org/wiki/Binary-coded_decimal
----------------------------------------------------------------------*/

#include <Wire.h>
#include "RTC.h"
#define RTC_I2C_ADDRESS 0x68

/* 
* Convert decimal to binary coded decimal (i.e. 92 => 1001 0010)
* Works for 0 - 99 (decimal)
*/
byte decToBcd(byte val)
{
  return( (val/10 << 4) + (val%10) );
}

/*
* Convert binary coded decimal to decimal (i.e. 1001 0010 => 92)
*/
byte bcdToDec(byte val)
{
  return(( (val>>4)*10) + (val%16) );
}

/*
* Used to manually set the time on the RTC in case it is wrong or needs
* initialization. 
*
* The following data is supplied as parameters:
*  - Second, Minute, Hour (in this order)
*  - Day of the week (1 = Sunday, ..., 7 = Saturday)
*  - Day of the month (1 to 31)
*  - Month (1 to 12)
*  - Year (0 to 99)
*
* All parameters are converted to BCD internally.
*
* For data  registers see pg. 8: http://datasheets.maximintegrated.com/en/ds/DS1307.pdf
*/
void setRtcTime(byte second, byte minute, byte hour, byte dayOfWeek, 
  byte dayOfMonth, byte month, byte year){
  
  //Prepare to transmit to RTC
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  
  Wire.write(0);  // Tell the RTC where we want to start reading or writing data.
                  // The first timekeeping register (at address 0) is the seconds register.
                  // Then the registers follow the order of the params list
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  
  Wire.endTransmission();
}

/*
* Reads from the timekeeping registers on the RTC to get the time and date. The
* results get placed in the passed variables.
*
* For data  registers see pg. 8: http://datasheets.maximintegrated.com/en/ds/DS1307.pdf
*/
void getRtcTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek,
  byte *dayOfMonth, byte *month, byte *year) {
    
  //Prepare to transmit to RTC
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // Tell the RTC where we want to start reading or writing data.
                 // The first timekeeping register (at address 0) is the seconds register.
                 // Then the registers follow the order of the params list                
  Wire.endTransmission();
  
  // Request seven bytes of data from RTC starting from register at address 0
  // Note, the RTC transfers data MSB first.
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.requestFrom(RTC_I2C_ADDRESS, 7);  
  *second = bcdToDec(Wire.read() & 0x7f); //The MSB in the seconds register is unrelated
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);   //The 3 MSBs relate to AM/PM and 24 hour time
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
  Wire.endTransmission();
}

/*
* Retrieve the time (using getTime) and print it to the Serial Monitor.
*/
void printRtcTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from RTC
  getRtcTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  //Print the hour
  Serial.print(hour, DEC);
  Serial.print(":");
  
  //Print the minute (with a leading 0 if needed)
  if (minute<10)  
    Serial.print("0");

  Serial.print(minute, DEC);
  Serial.print(":");
  
  //Print the second (with a leading 0 if needed)
  if (second<10)
    Serial.print("0");
  
  Serial.print(second, DEC);
  Serial.print(" ");
  
  //Print the date
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  
  //Print the day of the week
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

