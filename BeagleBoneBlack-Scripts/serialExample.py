"""
Performs continuous communication with an Arduino (or any Serial device) through UART1.
Sends stdin to UART1 TX (P9_24) and prints UART1 RX (P9_26) to stdout

Type "exit" to quit.
"""

import Adafruit_BBIO.UART as UART
import serial, sys, getopt
from select import select

_DEBUG = False
_TIMEOUT = 1
_EXIT = 'exit\n'


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

def ioWithArduino(port):
	""" 
	Standard In is used to send messages to the Arduino.
	"""
	while True:
		# Want to do a non-blocking read, so we can move on to check for input from Arduino
		# From: http://stackoverflow.com/questions/3471461/raw-input-and-timeout
		rlist, _, _ = select([sys.stdin], [], [], _TIMEOUT)
		
		# Attempt to read from stdin
		if rlist:
			line = sys.stdin.readline()
			if line == _EXIT:
				return
			else:
				if _DEBUG: print "Sending: " + line
				port.write(line)
		
		# Attempt to read from Arduino
		rxLine = port.readline()
		if rxLine:
			if _DEBUG: print "Received: ",

			# Strip off newline
			rxLine = rxLine[:-1] if '\n' in rxLine else rxLine
			print rxLine


# Check command line option to turn debug statements on
if len(sys.argv) > 1:
	if sys.argv[1] == '-d' and sys.argv[2] == '1':
		_DEBUG = True
	else:
		print "Usage: python serialExample.py [-d 1]"
		exit()

# Attempt to open UART1
port = openUart()
if port.isOpen():
	if _DEBUG: print "Serial port has been opened"
else:
	print "Failed to open UART1. Exiting."
	exit()

# Begin communication
ioWithArduino(port)

if _DEBUG: print "Closing serial port"
port.close()
