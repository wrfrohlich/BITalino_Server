CREATE DATABASE bitalino;
\c bitalino

CREATE TABLE patients (
	id serial not NULL,
	name VARCHAR(20) not NULL,
	ecg DECIMAL not NULL,
	eda DECIMAL not NULL,
	time VARCHAR(20) not NULL
);
