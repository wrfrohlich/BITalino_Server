#----------------------------------------------------------------------------------------------------------
#
#			Name: Eng. William da Rosa Frohlich
#
#			Project: Processing of BITalino data
#
#			Date: 2020.06.13
#
#----------------------------------------------------------------------------------------------------------
#----------			Libraries			----------

import json
import socket
import psycopg2

#----------------------------------------------------------------------------------------------------------
#----------			Variables			----------

#----------			Variables - Socket			----------
# Create the UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

address =('',18002)
sock.bind(address)

#----------------------------------------------------------------------------------------------------------
#----------			Functions			----------

#----------			Function - Register in database - Raw data			----------
def register_database (name, ecg, eda, time):
	DSN = 'dbname=bitalino user=postgres host=database'		
	connecting = psycopg2.connect(DSN)
	SQL = 'INSERT INTO patients (name, ecg, eda, time) VALUES (%s, %s, %s, %s)'
	cursorsql = connecting.cursor()
	cursorsql.execute(SQL, (name, ecg, eda, time))
	connecting.commit()
	cursorsql.close()

#----------------------------------------------------------------------------------------------------------
#----------			Main Function			----------
while True:
	try:
		packet, addr = sock.recvfrom(1024)
		packet = (str(packet))
		size = len(packet)
		control = int(packet[2:3])
		data = packet[3:(size-1)]
			
		if (control == 1):
			name = data
		elif (control == 2):
			ECG = int(data)
		elif (control == 3):
			EDA = int(data)
		elif (control == 4):
			time = data
		else:
			register_database(name, ECG, EDA, time)
	
	except KeyboardInterrupt:
		break
#----------------------------------------------------------------------------------------------------------