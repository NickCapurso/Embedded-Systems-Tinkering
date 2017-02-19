/*---------------------------------------------------------------------- 
* BMP 085 - Barometer
* 
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
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
#ifndef _BMP_GUARD
void initBmp();

int readBmpInt(unsigned char);
char readBmpByte(unsigned char);

void printBmpFahrenheit();
void printBmpCelsius();
void getBmpTemperature(float*);

void printBmpPressure();
void getBmpPressure(long*);

void printBmpStdAtm();
void getBmpStdAtmosphere(float*);

void printBmpAltitude();
void getBmpAltitude(float*);
#endif
