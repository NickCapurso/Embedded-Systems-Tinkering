"""
Reading values from an IMU3000 (gyroscope) sensor and writes them to
an SQLITE database.
"""
import sqlite3 as sqlite
from datetime import datetime
from time import sleep 
from imu3000 import imu3000

_DB_NAME = 'imu_readings.db'
_TABLE_NAME = 'IMU3000'
_DELAY = 0.25 # Seconds

# Prepare IMU sensor
sensor =  imu3000()

# Open/Create DB and begin transaction
dbConnection = sqlite.connect(_DB_NAME);
dbCursor = dbConnection.cursor()

# Create table for readings
dbCursor.execute('CREATE TABLE IF NOT EXISTS ' +  _TABLE_NAME + ' (Timestamp TIMESTAMP, X INT, Y INT, Z Int)')

# Collect 20 readings and save them to DB
for i in range(1, 20):
	x, y, z = sensor.getReadings()

	# Insert changes into DB, but they are not 'saved' until commit() is called (so you can rollback, if needed)
	dbCursor.execute('INSERT INTO ' + _TABLE_NAME + ' VALUES(?,?,?,?)', (datetime.now(), x, y, z))

	print "X: " + str(x) + ", Y: " + str(y)  + ", Z: " + str(z)
	sleep(_DELAY)

print "Finished."
sleep(1)

# Print all readings in the DB
print "---------- View DB ----------"
for results in dbCursor.execute('SELECT * FROM ' + _TABLE_NAME):
	print results

# Apply all changes to DB
dbConnection.commit()

dbConnection.close()
