//------------------------------------------------------------------------------------
// Libraries

#include "bitalino.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

//------------------------------------------------------------------------------------

#ifdef _WIN32

#include <conio.h>

bool keypressed(void)
{
	return (_kbhit() != 0);
}

//------------------------------------------------------------------------------------

#else // Linux or Mac OS

#include <sys/select.h>

bool keypressed(void)
{
   fd_set   readfds;
   FD_ZERO(&readfds);
   FD_SET(0, &readfds);

   timeval  readtimeout;
   readtimeout.tv_sec = 0;
   readtimeout.tv_usec = 0;

   return (select(FD_SETSIZE, &readfds, NULL, NULL, &readtimeout) == 1);
}

#endif

//------------------------------------------------------------------------------------

int main()
{
//------------------------------------------------------------------------------------
// Variable
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in serv_control;
	struct sockaddr_in serv_name;
	struct sockaddr_in serv_ECG;
//	struct sockaddr_in serv_EEG;
//	struct sockaddr_in serv_EMG;
	struct sockaddr_in serv_EDA;
	struct sockaddr_in serv_time;
	char msg[42], ECG[3], EDA[3], current_time[18], first_name[10], last_name[10], name[20], control[4] = "RUN";
	
	// Control
	serv_control.sin_family = AF_INET;
	serv_control.sin_port = htons(18100);
	serv_control.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_control = sizeof(serv_control);

	// Name
	serv_name.sin_family = AF_INET;
	serv_name.sin_port = htons(18101);
	serv_name.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_name = sizeof(serv_name);
	
	// Time
	serv_time.sin_family = AF_INET;
	serv_time.sin_port = htons(18102);
	serv_time.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_time = sizeof(serv_time);
	
	// ECG
	serv_ECG.sin_family = AF_INET;
	serv_ECG.sin_port = htons(18103);
	serv_ECG.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_ECG = sizeof(serv_ECG);
/*	
	// EEG
	serv_EEG.sin_family = AF_INET;
	serv_EEG.sin_port = htons(18104);
	serv_EEG.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_EEG = sizeof(serv_EEG);	
*/
/*
	// EMG
	serv_EMG.sin_family = AF_INET;
	serv_EMG.sin_port = htons(18105);
	serv_EMG.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_EMG = sizeof(serv_EMG);
*/	
	// EDA
	serv_EDA.sin_family = AF_INET;
	serv_EDA.sin_port = htons(18106);
	serv_EDA.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t m_EDA = sizeof(serv_EDA);
	
	string file_name;
	string start_name = "Database ";
	string end_name = ".txt";
	string s_day, s_month;
	
	time_t date_time;
	
	time(&date_time);
	
	struct tm*get_date = localtime(&date_time);
	struct tm*get_time = localtime(&date_time);
	
	int i_day = get_date->tm_mday;
	int i_month = get_date->tm_mon + 1;
	int i_year = get_date->tm_year + 1900;
	int i_min = get_time->tm_min;
	int i_hour = get_time->tm_hour;
	int i_sec = get_time->tm_sec;
	
	s_day = std::to_string(i_day);
	s_month = std::to_string(i_month);
	
	file_name = start_name + " " + s_month + "-" + s_day + end_name;
	
//------------------------------------------------------------------------------------

	try
	{
		// Starts the file
		ofstream myfile;
		
		// Read the patient's name
		puts("Please, insert the first name of the patient:");
		scanf("%s", first_name);
		
		puts("Please, insert the last name of the patient:");
		scanf("%s", last_name);
		
		snprintf(name, 20, "%s %s", first_name, last_name);
		
		// Waiting for Connection
		puts("Connecting to device. Please, wait a moment.");
		
		// Set the MAC address
		BITalino dev("20:16:12:22:50:18");  // Device MAC address (Windows and Linux)
		
		// Message about the defice connected
		puts("Device Connected. Press Enter to exit.");
		
		// Create the file
		myfile.open (file_name, ios::app);
		
		// Define the categories of the file
		myfile << "NAME;ECG;EDA;TIME\n";
		
		// Close the file
		myfile.close();
		
		// Get device version and show
		std::string ver = dev.version();
		printf("BITalino version: %s\n", ver.c_str());
		
		// Start acquisition of all channels at 1000 Hz
		dev.start(10, { 0, 1, 2, 3, 4, 5});
		dev.trigger({true, false});
		
//------------------------------------------------------------------------------------
// Start Loop

		// Initialize the frames vector with 1 frames
		BITalino::VFrame frames(1);
		do
		{
			// Get frames
			dev.read(frames);
			const BITalino::Frame &f = frames[0];
			
			// Open the file
			myfile.open (file_name, ios::app);
			
			// Get the current time
			date_time;
			time(&date_time);
			
			tm*get_date = localtime(&date_time);
			tm*get_time = localtime(&date_time);
			
			i_day = get_date->tm_mday;
			i_month = get_date->tm_mon + 1;
			i_year = get_date->tm_year + 1900;
			i_min = get_time->tm_min;
			i_hour = get_time->tm_hour;
			i_sec = get_time->tm_sec;
			
			// Show in the prompt the frame
			sprintf(msg, "%s;%d;%d;%d/%d/%d %d:%d:%d", name, f.analog[1], f.analog[2], i_day, i_month, i_year, i_hour, i_min, i_sec);
			sprintf(ECG, "%d", f.analog[1]);
			sprintf(EDA, "%d", f.analog[2]);
			sprintf(current_time, "%d/%d/%d %d:%d:%d", i_day, i_month, i_year, i_hour, i_min, i_sec);
			
			cout << msg << "\n";
			sendto(sockfd,control,strlen(control),0,(struct sockaddr *)&serv_control,m_control);
			
			sendto(sockfd,name,strlen(name),0,(struct sockaddr *)&serv_name,m_name);
			
			sendto(sockfd,ECG,strlen(ECG),0,(struct sockaddr *)&serv_ECG,m_ECG);
			
			sendto(sockfd,EDA,strlen(EDA),0,(struct sockaddr *)&serv_EDA,m_EDA);
			
			sendto(sockfd,current_time,strlen(current_time),0,(struct sockaddr *)&serv_time,m_time);
			
			// Save the data in the file
			myfile << msg << "\n";
			
			// Close the file
			myfile.close();
		
		// Until the key is pressed
		} while (!keypressed());
		
		dev.stop();
		
		strcpy(control, "STOP");
		sendto(sockfd,control,strlen(control),0,(struct sockaddr *)&serv_control,m_control);
		sendto(sockfd,name,strlen(name),0,(struct sockaddr *)&serv_name,m_name);
		sendto(sockfd,ECG,strlen(ECG),0,(struct sockaddr *)&serv_ECG,m_ECG);
		sendto(sockfd,EDA,strlen(EDA),0,(struct sockaddr *)&serv_EDA,m_EDA);
		sendto(sockfd,current_time,strlen(current_time),0,(struct sockaddr *)&serv_time,m_time);
		
//------------------------------------------------------------------------------------
// End Loop

	}
	catch(BITalino::Exception &e)
	{
		
		printf("BITalino exception: %s\n", e.getDescription());
		
	}
	return 0;
}
//------------------------------------------------------------------------------------