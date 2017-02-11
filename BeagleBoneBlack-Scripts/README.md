[Back to index](https://github.com/NickCapurso/Embedded-Systems-Tinkering)

# BeagleBone Black Scripts
A few Python scripts I've used to demonstrate I2C and UART communication with a [Beagle Bone Black](https://beagleboard.org/black).
* **serialExample.py** - Meant the emulate the "serial monitor" from an Arduino (i.e. typing and hitting enter sends a message over UART TX; messages received on RX are automatically printed). Type "exit" to exit.
* **imu3000.py** - Provides a simple interface to read data values from an [IMU-3000 Gyroscope](https://store.invensense.com/datasheets/invensense/PS-IMU-3000A.pdf). Can also be used as a standalone script to simply read and print gyroscopic values.
* **sendGyroReadings.py** - A little of both of the above two: reads gyroscope readings from the IMU-3000 and sends them out over UART TX in regular intervals to be received by another device.
* **sensorDb.py** - Reads gyroscope readings from the IMU-3000 and writes them to an SQLITE3 database.


