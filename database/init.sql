CREATE DATABASE bitalino;
\c bitalino

CREATE TABLE patients (
	id serial not NULL,
	name VARCHAR(20) not NULL,
	ecg DECIMAL not NULL,
	eda DECIMAL not NULL,
	start_time VARCHAR(20) not NULL,
	final_time VARCHAR(20) not NULL
);
