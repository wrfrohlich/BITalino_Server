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

After to prepare the enviroment to use the docker compose it is necessary to run the containers to start the application to processing the data, the database and the dashboard. For this, the bash must be change in the path with the files of this project, in sequence using `cd Docker` and `docker-compose up -d` to run the containers. 

#### 2.3 - Using the Database (__PostgreSQL__):

Inside the folder 'database' there are two files, the first file is used to inicialize the database `'bitalino'` and the table `'patients'` with categories `'name'`, `'ecg'` and `'eda'`. The second file is used to check if the database and table was created properly. This database created use the exposed port `18001`, user `postgres` and the password is not defined.

#### 2.4 - Using the Dashboard (__Grafana__):

Inside the folder 'web' there are other two folders. The folder 'datasources' there is the file (datasource.yaml) with the configuration of the database (PostgreSQL) that will be used to get the data to monitoring. The folder 'dashboards' there are two files, the first file (dashboard.yaml) is used to configure the dashboard properly and the second file (Patients.json) is used to have the dashboard entirely configured.

#### 2.5 - Using the Processing (__Python__):

Inside the folder 'processing' there are two files, the first file (start_processing.sh) is used to download the necessary libraries, after the download the second line call the other file (processing.py). The python code has the goal to receive all data from the C++ code via socket, the python code opens UDP sockets as server.

In the first part of the code are defined the libraries, in the second part are difined the global variables of the code, the function `socket.socket(socket.AF_INET, socket.SOCK_DGRAM)` define the protocol and `bind('',18101)` is used to define the IP address and the port of the socket server.


#### 2.5 - Using the Acquisition (__C++__):

The first step to use the application it is define the bluetooth address in the acquisition.cpp file in the variable `BITalino dev("xx:xx:xx:xx:xx:xx")`. In the acquisition.cpp file there are others options to configure the system, for example the channels that will be monitored (channel 1 = ECG; channel 2 = EDA; channel 3 = EMG) or the port of the sockets, all these options are described in the code.

The next step must change the path in the bash to the path with all files, now it is possible generate the executable file using the command `make`. After this, the application is able to use with the command `./acquisition`. Turn on the BITalino and fill  name and last name to start the acquisition. To finish the acquisition it is necessary tap the 'enter' in the keyboard to stop the acquisition and disconnect properly the BITalino.

## 3 - Useful Commands

#### 3.1 - Basic Commands

* List Images: `docker image ls`

* List Containers: `docker container ls -a`

* List Volumes: `docker volume ls`

* List Networks: `docker network ls`

* Remove Unnecessary Volumes: `docker volume prune`

* Remove Unnecessary Networks: `docker network prune`

* Remove Images: `docker image rm xxxxxx`

* Remove Containers: `docker image rm xxxxxx`

* Remove Volumes: `docker image rm xxxxxx`

* Remove Networks: `docker image rm xxxxxx`

#### 3.2 - Advanced Commands
* Up Docker-Compose with Logs: `docker-compose up`

* Up Docker-Compose without Logs: `docker-compose up -d`

* Check Docker-Compose: `docker-compose ps`

* Down Docker-Compose: `docker-compose down`

* Logs of Docker-Compose: `docker-compose logs -t -f`

* Logs of Processing Application: `docker-compose logs -t -f processing`

* Logs of Database Application: `docker-compose logs -t -f database`

* Logs of Dashboard Application: `docker-compose logs -t -f web`

* Check the Creation of Database: `docker-compose exec database psql -U postgres -f ./database/check.sql`

* Check the Database: `docker-compose exec database psql -U postgres -d bitalino -c 'select * from patients'`
