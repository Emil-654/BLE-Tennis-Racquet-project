# Racquet ESP32
This folder contains the code for the data collection ESP32 located on the racquet. It functions as a peripheral BLE device and GATT server.

## MPU6050.c/.h
I2C driver for the MPU6050 3 Axis Accelerometer and Gyroscope sensor

## gatt_server_main.h
Contains the definition for the service and corresponding characteristics for the profile

## gatt_server_main.c
Main file for the peripheral. BLE is initialized here and configured as a server. It will then begin advertising until a connection is made with the display ESP32. In the RTOS task, the attributes for the 6 characteristics, X,Y,Z value of the acceleration and X,Y,Z value of the angular velocity, are updated with readings from the MPU6050.

## Hardware
SDA was GPIO pin 21 and SCL was GPIO pin 22, these were connected to the corresponding SCL/SDA pins of the MPU6050. A 3x AA battery pack was connected to the VIN 5V and GND pins of the ESP32. The MPU6050 was powered from the 3.3V pin of the ESP32 and GND. 
