# BITalino Server
System for acquisition of the biosignals from BITalino board.

## Architecture:

#### Topology:

`teste`

#### Ports:
Containers:

* 18000 -> Web / Dashboard (Grafana)

* 18001 -> Database (PostgreSQL)

Communications:

* 18100 -> Control Variable / Socket between Acquisition application and Processing application

* 18101 -> Patient's name / Socket between Acquisition application and Processing application

* 18102 -> Time / Socket between Acquisition application and Processing application

* 18103 -> ECG Variable / Socket between Acquisition application and Processing application

* 18104 -> EEG Variable / Socket between Acquisition application and Processing application

* 18105 -> EMG Variable / Socket between Acquisition application and Processing application

* 18106 -> EDA Variable / Socket between Acquisition application and Processing application

## How to use this project:

#### Using the Bluetooth and Acquisition:

#### Using the Database:

#### Using the Grafana:

## Useful Commands
