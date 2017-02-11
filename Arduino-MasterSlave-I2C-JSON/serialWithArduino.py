"""
Performs continuous communication with an Arduino (or any Serial device) through the serial port.

Type "exit" to quit.
"""

import serial, sys, getopt
from select import select

_DEBUG = False
_DEVICE_FILE = "/dev/cu.usbmodem1411"

_BAUDRATE = 9600
_TIMEOUT = 1
_JSON_CMD = 'json\n'
_EXIT = 'exit\n'

def openSerial():
	ser = serial.Serial(port = _DEVICE_FILE, baudrate=_BAUDRATE, timeout=_TIMEOUT)

	# Reset port before opening
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
                        elif line == _JSON_CMD:
                                with open('testJson.txt', 'r') as myfile:
                                    json = myfile.read()
                                    if _DEBUG: print "Sending: |" + json + "|"
                                    port.write(json)

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

port = openSerial()
if port.isOpen():
	if _DEBUG: print "Serial port " + _DEVICE_FILE + " has been opened"
else:
	print "Failed to open " + _DEVICE_FILE + ". Exiting."
	exit()

# Begin communication
ioWithArduino(port)

if _DEBUG: print "Closing serial port"
port.close()
