# BITalino Server
System for acquisition of the biosignals from BITalino board.

## 1 - Architecture:

#### 1.1 - Topology:

![ScreenShot](https://github.com/wrfrohlich/BITalino_Server/blob/V1.0/figures/Topology.png)

#### 1.2 - Ports:
1.2.1 - Containers:

* 18000 -> Web / Dashboard (Grafana)

* 18001 -> Database (PostgreSQL)

1.2.2 - Communications:

* 18100 -> Control Variable / Socket between Acquisition application and Processing application

* 18101 -> Patient's name / Socket between Acquisition application and Processing application

* 18102 -> Time / Socket between Acquisition application and Processing application

* 18103 -> ECG Variable / Socket between Acquisition application and Processing application

* 18104 -> EEG Variable / Socket between Acquisition application and Processing application

* 18105 -> EMG Variable / Socket between Acquisition application and Processing application

* 18106 -> EDA Variable / Socket between Acquisition application and Processing application

## 2 - How to use this project:

#### 2.1 - Preparing the enviroment:
First of all it is necessary install and configure the libraries.

2.1.1 - Bluetooth:

* Install the bluetooth library: `sudo apt-get install pi-bluetooth`

* Start the configuration: `sudo bluetoothctl`

* Scan to find bluetooth address: `scan on`

* Connect with the device: `pair xx:xx:xx:xx:xx:xx`

2.1.2 - Docker:

!!! Raspberry Pi uses the ARM architecture, so won't be compatible with all containers. Images need to be built from an ARM base !!!

* Install the docker: `curl -sSL https://get.docker.com | sh`

* Add the user: `sudo usermod -aG docker pi`

* Enable the docker to start with the system: `sudo systemctl enable docker`

* Restart the system: `sudo reboot -h now`

2.1.3 - Docker Compose:

* Install the dependencies: `sudo apt-get install -y libffi-dev libssl-dev`

* Install Python 3: `sudo apt-get install -y python3 python3-pip`

* Install Docker Compose: `sudo pip3 install docker-compose`

#### 2.2 - Using the Docker Compose:

After to prepare the enviroment to use the docker compose it is necessary to run the containers to start the database and dashboard

#### 2.3 - Using the Database (__PostgreSQL__):

#### 2.4 - Using the Dashboard (__Grafana__):

#### 2.5 - Using the Processing (__Python__):

#### 2.5 - Using the Acquisition (__C++__):

The first step to use the application it is define the bluetooth address in the acquisition.cpp file in the variable `BITalino dev("xx:xx:xx:xx:xx:xx")`. In the acquisition.cpp file there are others options to configure the system, for example the channels that will be monitored (channel 1 = ECG; channel 2 = EDA; channel 3 = EMG) or the port of the sockets, all these options are described in the code.

The next step must change the path in the bash to the path with all files, now it is possible generate the executable file using the command `make`. After this, the application is able to use with the command `./acquisition`. Turn on the BITalino and fill  name and last name to start the acquisition. To finish the acquisition it is necessary tap the 'enter' in the keyboard to stop the acquisition and disconnect properly the BITalino.

## 3 - Useful Commands

