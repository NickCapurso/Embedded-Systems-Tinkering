/*---------------------------------------------------------------------- 
*IMU 3000 - Gyroscope
* NOTE: the initImu() function must be called before any others.
*
* Print functions print directly to the Serial Monitor while
* get functions place results in the passed variable(s)
* 
* Datasheet: 
* http://store.invensense.com/datasheets/invensense/PS-IMU-3000A.pdf
*
----------------------------------------------------------------------*/
#ifndef _IMU_GUARD
void initImu();
void getImuReading(int*, int*, int*);
void printImuReading();
#endif
