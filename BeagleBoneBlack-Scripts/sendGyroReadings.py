import Adafruit_BBIO.UART as UART
import serial
from imu3000 import imu3000
from time import sleep

_TIMEOUT = 1

def openUart():
        """
        Opens UART1 (TX = 24, RX = 26). Returns true if successful.
        """

        UART.setup("UART1")
        ser = serial.Serial(port = "/dev/ttyO1", baudrate=9600, timeout=_TIMEOUT)

        # Reset port
        ser.close()
        ser.open()

        return ser

port = openUart()
if not port.isOpen():
	print "Failed to open UART1. Exiting."
	exit()

sensor = imu3000()

while True:
	x, y, z = sensor.getReadings()
	toSend = "X: " + str(x) + ", Y: " + str(y)  + ", Z: " + str(z) + "\n"
	port.write(toSend)	
	sleep(0.5)
