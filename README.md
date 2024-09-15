# MKR-WiFi-MQTT-Sensor
 This Arduino sketch publishes temperature, humidity, and moisture data from an MKRIoT Carrier to an MQTT broker. It connects to WiFi and reconnects to the broker if necessary. Visual feedback is provided on the carrier display.

## Features:

- WiFi & MQTT connection management
- Sensor data reading (temperature, humidity, moisture)
- JSON message formatting
- MQTT publishing

## Requirements:

- Visual Studio PlatformIO extension
- WiFiNINA library
- PubSubClient library
- Arduino_MKRIoTCarrier library

## Configuration file:
The client uses a configuration file config.h with the following format

```cpp
#pragma once

const char * CONNECTION_STRING="HostName=server-ip-address;DeviceID=null;Key=null"; 
const char * WIFI_NAME="Name of your wifi network";
const char * WIFI_PASSWORD="password for your wifi network";
```
Note that in the **CONNECTION_STRING** field, the **DeviceID** and **Key** fields are specific to another project that uses this approach, and for other use cases can be discarded.
