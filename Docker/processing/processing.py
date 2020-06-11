import json
import socket
import psycopg2

sock_control = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_name = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_time = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_ECG = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock_EEG = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock_EMG = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_EDA = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

address_name =('',18101)
sock_name.bind(address_name)
address_time =('',18102)
sock_time.bind(address_time)
address_ECG =('',18103)
sock_ECG.bind(address_ECG)
#address_EEG =('',18104)
#sock_EEG.bind(address_ECG)
#address_EMG =('',18105)
#sock_EMG.bind(address_EMG)
address_EDA =('',18106)
sock_EDA.bind(address_EDA)

def register_database (name, ecg, eda, time):
	DSN = 'dbname=bitalino user=postgres host=database'		
	connecting = psycopg2.connect(DSN)
	SQL = 'INSERT INTO patients (name, ecg, eda, time) VALUES (%s, %s, %s, %s)'
	cursorsql = connecting.cursor()
	cursorsql.execute(SQL, (name, ecg, eda, time))
	connecting.commit()
	cursorsql.close()


while True:
	try:
		name, addr = sock_name.recvfrom(1024)
		name = (str(name))
		size = len(name)
		name = name[2:(size-1)]
			
		ECG, addr = sock_ECG.recvfrom(1024)
		ECG = int(ECG)
	
		EDA, addr = sock_EDA.recvfrom(1024)
		EDA = int(EDA)
	
		time, addr = sock_time.recvfrom(1024)
		time = (str(time))
		size = len(time)
		time = time[2:(size-1)]
		
		register_database(name, ECG, EDA, time)
	
	except KeyboardInterrupt:
		break