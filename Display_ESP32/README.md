# Display ESP32

This folder contains the code for the data display ESP32. It functions as a central BLE device and GATT client.  

## ESPSSD1306.c/.h

I2C driver for the SSD1306 OLED display. Contains helper functions along with a number font and title.

## gatt_client_main.c

Main file for the display. BLE is initialized here and configured as a client. It will begin scanning for servers and then will connect to the right GATT server.  An X is displayed until this connection happens. Services and characteristics will then be obtained from the server. In this project there are 6 characteristics: X,Y,Z value of the acceleration and X,Y,Z value of the angular velocity. In the RTOS task, each characteristic is then read as signed 16 bit integers, converted to floats, digits extracted from the floats and then displayed. Upon disconnection, the X is displayed until a reconnection.
