"""
Class representing the IMU3000 sensor. Can also be used as a standalone script to read
from the sensor repeatedly.

Datasheet: https://store.invensense.com/datasheets/invensense/PS-IMU-3000A.pdf
"""
from Adafruit_I2C import Adafruit_I2C
from time import sleep

_I2C_ADDR = 0x68
_REG_PARAMS = 0x16
_CONFIG_VAL = 0x0B
_REG_X = 0x1D
_REG_Y = 0x1F
_REG_Z = 0x21

class imu3000:
	def __init__(self):
		self.sensor = Adafruit_I2C(_I2C_ADDR)
		self.sensor.write8(_REG_PARAMS, _CONFIG_VAL)

	def getX(self):
		return (self.sensor.readS8(_REG_X) << 8 | self.sensor.readS8(_REG_X + 1))

	def getY(self):
		return (self.sensor.readS8(_REG_Y) << 8 | self.sensor.readS8(_REG_Y + 1))

	def getZ(self):
		return (self.sensor.readS8(_REG_Z) << 8 | self.sensor.readS8(_REG_Z + 1))

	def getReadings(self):
		return (self.getX(), self.getY(), self.getZ())


# If run standalone, just print sensor readings
if __name__ == "__main__":
	sensor = imu3000()

	while True:
		x, y, z = sensor.getReadings()
		print "X: " + str(x) + ", Y: " + str(y)  + ", Z: " + str(z)
		sleep(0.25)

