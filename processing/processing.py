import math
import json
import socket
import datetime
import psycopg2
import numpy as np
import pandas as pd
from biosppy import signals as bio_signals

sock_control = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_name = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_time = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_ECG = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock_EEG = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock_EMG = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_EDA = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

address_control =('',18100)
sock_control.bind(address_control)
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

data_processing = pd.DataFrame(columns =['ECG','EDA','TIME'])

def save_file(name, data_processing):
	name_file = ("Patient - " + name + ".csv")
	data_processing.to_csv(name_file, index=None, header=True, sep=';', encoding='utf-8')
	
def processing(data_processing):
	ecg_signal = bio_signals.ecg.ecg(signal=data_processing['ECG'], sampling_rate=100, show=False)
	eda_signal = bio_signals.eda.eda(signal=data_processing['EDA'], sampling_rate=100, show=False)
	ECG_average = ecg_signal["heart_rate"].mean()
	EDA_average = eda_signal["filtered"].mean()
	start_time = str(data_processing['TIME'][0:1].values)
	size = len(start_time)
	start_time = start_time[2:(size-2)]
	end = len(data_processing)
	final_time = str(data_processing['TIME'][(end-2):(end-1)].values)
	size = len(final_time)
	final_time = final_time[2:(size-2)]
	
	return ECG_average, EDA_average, start_time, final_time 
	
def register_database (name, ecg, eda, start_time, final_time):
	DSN = 'dbname=bitalino user=postgres host=localhost port=18001'		
	connecting = psycopg2.connect(DSN)
	SQL = 'INSERT INTO patients (name, ecg, eda, start_time, final_time) VALUES (%s, %s, %s, %s, %s)'
	cursorsql = connecting.cursor()
	cursorsql.execute(SQL, (name, ecg, eda, start_time, final_time))
	connecting.commit()
	cursorsql.close()


while True:
	try:
		control, addr = sock_control.recvfrom(1024)
		control = (str(control))
		size = len(control)
		control = control[2:(size-1)]
		if (control == "STOP"):
			save_file(name, data_processing)
			ECG_average, EDA_average, start_time, final_time = processing(data_processing)
			register_database(name, ECG_average, EDA_average, start_time, final_time)
			data_processing = pd.DataFrame(columns =['ECG','EDA','TIME'])
		
		name, addr = sock_name.recvfrom(1024)
		name = (str(name))
		size = len(name)
		name = name[2:(size-1)]
			
		ECG, addr = sock_ECG.recvfrom(1024)
		ECG = int(ECG)
	
		EDA, addr = sock_EDA.recvfrom(1024)
		EDA = int(EDA)
	
		current_time, addr = sock_time.recvfrom(1024)
		current_time = (str(current_time))
		size = len(current_time)
		current_time = current_time[2:(size-1)]
	
		new_df = pd.DataFrame({'ECG' : [ECG], 'EDA' : [EDA], 'TIME' : [current_time]})
		data_processing = data_processing.append(new_df)
	
	except KeyboardInterrupt:
		break