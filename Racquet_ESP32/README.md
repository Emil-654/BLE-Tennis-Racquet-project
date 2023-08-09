# Racquet ESP32
This folder contains the code for the data collection ESP32 located on the racquet. It functions as a peripheral BLE device and GATT server.

## MPU6050.c/.h
I2C driver for the MPU6050 3 Axis Accelerometer and Gyroscope sensor

## gatt_server_main.h
Contains the definition for the service and corresponding characteristics for the profile

## gatt_server_main.c
Main file for the peripheral. BLE is initialized here and configured as a server. It will then begin advertising. 

